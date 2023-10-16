import tomllib
import os, sys, math, glob
import argparse
import datetime
import numpy as np
import subprocess
import shlex

## Flux function must have the form func(E,**kwargs)
## Please make sure that you sanitize your inputs like below when you write your own
## Also, physical fluxes should have units of 1/(MeV cm^2 s sr) to be consistent with the rest of the programs

def BrokenPowerLaw(E,**kwargs):
## Broken power law spectrum. 
# Background MeV Spectra found here: https://iopscience.iop.org/article/10.3847/1538-4357/acab69
    try:
    ## Constant before flux function has units of 1/(MeV cm^2 s sr)
        A = kwargs["A"]
        if(A<=0):
            print("Invalid constant")
            sys.exit()
    except:
        print("Invalid constant")
        sys.exit()
    try:
        Eref = kwargs["Eref"]
    except:
        print("Eref not specified")
        sys.exit()
    try:
        Gamma1 = kwargs["Gamma1"]
    except:
        print("Gamma1 not specified")
        sys.exit()
    try:
        Gamma2 = kwargs["Gamma2"]
    except:
        print("Gamma2 not specified")
        sys.exit()
    if E<=Eref:
        return A*np.power((1/Eref)*E,-Gamma1)
    else:
        return A*np.power((1/Eref)*E,-Gamma2)

def LogUniform(E,**kwargs):
## Suppose that you treat this function as a probability distribution.
## If you draw enough points and set energy to a log scale, then you get a uniform distribution
## This is desirable for sensitivity studies since energies span a wide range of orders of magnitude
## This is the Reference energy-dependent flux used in GenSkyMap, hence the units don't really matter for this one
    try:
        a = kwargs["a"]
    except:
        print("a not specified")
        sys.exit()
    try:
        b = kwargs["b"]
    except:
        print("b not specified")
        sys.exit()
    scaling = np.log10(b/a)
    return np.power(E,-1)/scaling

def GenFluxes(EffectiveAreaFile  ,PhysicalFluxName , ReferenceFluxName ):
    args = parser.parse_args()
    EffAreaFile = ROOT.TFile(EffectiveAreaFilePath, "READ")
    EffAreaHist = EffAreaFile.EffArea
    BackgroundFile = ROOT.TFile(PhysicalFluxName ,"RECREATE")
## NOTE: When using Clone(), the title doesn't get changed. Make sure that you are aware of this
## You can change this if you want to with SetNameTitle(), but I didnt' care enough to do so
    BackgroundEnergyHist = EffAreaHist.Clone("BackgroundFlux")
    BackgroundEnergyHist.Reset()
## Get centers of bins. Use these bin centers as the x axis values
    EnergyCenters = [BackgroundEnergyHist.GetBinCenter(i) for i in range(BackgroundEnergyHist.GetNbinsX())]
## Need to modify the following line if you have a different background flux
# Background MeV Spectra found here: https://iopscience.iop.org/article/10.3847/1538-4357/acab69
    BackOutput = [BrokenPowerLaw(energy,A=2.2E-4,Eref=3,Gamma1=3.3,Gamma2=2) for energy in EnergyCenters]
## Fills output histogram with flux function by filling each bin weighted by the function
    for bin in range(0,BackgroundEnergyHist.GetNbinsX()):
        BackgroundEnergyHist.SetBinContent(bin,BackOutput[bin])
## Save to disk
    BackgroundFile.WriteObject(BackgroundEnergyHist, "PhysicalFlux")
## Create reference file
    ReferenceFile = ROOT.TFile(ReferenceFileName ,"RECREATE")
    ReferenceEnergyHist = EffAreaHist.Clone("ReferenceFlux")
    ReferenceEnergyHist.Reset()
    EnergyCenters = [ReferenceEnergyHist.GetBinCenter(i) for i in range(ReferenceEnergyHist.GetNbinsX())]
# Standard LogUniform distribution (ie. equal probabilities amoungst log bins)
# Bottom and top edge are the min and max energies of EffArea (normally 0.1 to 10 MeV)
    bottom_edge = EffAreaHist.GetBinLowEdge(0)
    center_of_top =  ReferenceEnergyHist.GetBinCenter(ReferenceEnergyHist.GetNbinsX())
# Have to do some weird stuff to get the top edge since ROOT excludes the upper limit from the histogram
    top_edge = (center_of_top-ReferenceEnergyHist.GetBinLowEdge(ReferenceEnergyHist.GetNbinsX()))+center_of_top
    RefOutput = [LogUniform(energy,a=bottom_edge,b=top_edge) for energy in EnergyCenters]
## Fills output histogram with flux function by filling each bin weighted by the function
    for bin in range(0,ReferenceEnergyHist.GetNbinsX()):
        ReferenceEnergyHist.SetBinContent(bin,RefOutput[bin])
## Save to disk
    ReferenceFile.WriteObject(ReferenceEnergyHist,"ReferenceFlux")

def SphereToCart(Azi,Alt):
## Takes in Azi and Alt as radians and calculates x,y,z direction
    x_truth = np.cos(Azi)*np.cos(Alt)
    y_truth = np.sin(Azi)*np.cos(Alt)
    z_truth = np.sin(Alt)
    return np.array([x_truth,y_truth,z_truth])

def shell_script_preamble(shell_name, tar_file_name):
    with open(shell_name, 'w') as f:
        f.write("#!/bin/bash -l\n")
#        f.write("source /usr/nevis/adm/nevis-init.sh\n")
        f.write("conda activate /nevis/amsterdam/share/seligman/conda/grams\n")
        f.write("process=$1\n")
## Extract folder and cd to directory to run GramsSim code
        f.write("tar -xzf "+tar_file_name+"\n")
        f.write("cd GramsWork\n")

def cmd_script_generation_cones(output_directory_base_path, Job, cmd_name, shell_name,tar_name, nbatches):
# Create absolute paths to location of shell script, tar file and name temporary files
    home = os.getcwd()
    shell_path = os.path.join(home,shell_name)
    tar_path = os.path.join(home,tar_name)
    condor_output = "temp-$(Process).out"
    condor_log = "temp-$(Process).log"
    condor_err = "temp-$(Process).err"
## Make sure that output folder exists.
    if not (os.path.exists(output_directory_base_path)):
        print(output_directory_base_path + "doesn't exist")
        sys.exit()
    output_dir = os.path.join(output_directory_base_path,Job)
    try:
        os.makedirs(output_dir)
## If the directory already exists, then keep going
    except FileExistsError as err:
        pass
    except  Exception as err:
        print("couldn't create "+output_dir)
        sys.exit()
## Write cmd file for condor
    with open(cmd_name, 'w') as f:
        f.write("executable = "+shell_path +"\n")
        f.write("transfer_input_files = "+tar_path+"\n")
        f.write("arguments = $(Process)\n")
        f.write("initialdir = "+output_dir+"\n")
        f.write("universe       = vanilla\n")
        f.write("should_transfer_files = YES\n")
        f.write("when_to_transfer_output = ON_EXIT\n")
        f.write("""requirements = ( Arch == "X86_64" )\n""")
        f.write("output = "+condor_output+"\n")
        f.write("error = "+condor_err+"\n")
        f.write("log = "+condor_log+"\n")
        f.write("notification   = Never\n")
        f.write("queue "+str(nbatches)+"\n")

def cmd_script_generation_skymap(output_directory_base_path, base_input_name, Job, cmd_name, shell_name,tar_name, nbatches):
# Create absolute paths to location of shell script, tar file and name temporary files
    home = os.getcwd()
    shell_path = os.path.join(home,shell_name)
    tar_path = os.path.join(home,tar_name)
    condor_output = "temp-$(Process).out"
    condor_log = "temp-$(Process).log"
    condor_err = "temp-$(Process).err"
## Make sure that output folder exists.
    if not (os.path.exists(output_directory_base_path)):
        print(output_directory_base_path + "doesn't exist")
        sys.exit()
    output_dir = os.path.join(output_directory_base_path,Job,"SkyMap")
    try:
        os.makedirs(output_dir)
## If the directory already exists, then keep going
    except FileExistsError as err:
        pass
    except  Exception as err:
        print("couldn't create "+output_dir)
        sys.exit()
## Check that the input cone files are present in the correct directory
    input_file_pattern = base_input_name+"*.root"
    data_dir = os.path.join(output_directory_base_path,Job,"Cones")
    if not (os.path.exists(data_dir)):
        print(data_dir + "doesn't exist")
        sys.exit()
## glob magic checks if any files have the correct naming scheme
    if len(glob.glob(os.path.join(data_dir,input_file_pattern))) == 0:
        print(data_dir + "doesn't contain cone files")
        sys.exit()
    input_data_file = os.path.join(data_dir, input_file_pattern+"$(Process).root")
## Write cmd file for sky map generation
    with open(cmd_name, 'w') as f:
        f.write("executable = "+shell_path +"\n")
        f.write("arguments = $(Process)\n")
        f.write("transfer_input_files = "+tar_path+", " + input_data_file+ "\n")
        f.write("initialdir = "+output_dir+"\n")
        f.write("universe       = vanilla\n")
        f.write("should_transfer_files = YES\n")
        f.write("when_to_transfer_output = ON_EXIT\n")
        f.write("""requirements = ( Arch == "X86_64" )\n""")
        f.write("output = "+condor_output+"\n")
        f.write("error = "+condor_err+"\n")
        f.write("log = "+condor_log+"\n")
        f.write("notification   = Never\n")
        f.write("queue "+str(nbatches)+"\n")

def EffectiveAreaGeneration(configuration):
    current_time = datetime.datetime.now().isoformat()
    base_name = "EffArea_"+str(current_time)
    base_name = base_name.replace(":",".")
    output_shell_file = base_name+".sh"
    output_cmd_file = base_name+".cmd"
    tar_file = base_name+".tar.gz"
    generic_gramssky_hepmc3_macro = "GenericHepmc3.mac"
    Geo = configuration["General"]["Geometry"].lower()
    output_directory_base_path = config["General"]["output_directory"]
## Constants
    PositionGeneration  = "Iso"
    PhiMinMax = "\"(-3.14159,3.14159)\""
    ThetaMinMax = "\"(-1.57079,1.57079)\""
    if(Geo=='cube'):
        OriginSphere = "\"(0,0,-40.0 )\""
    if(Geo=='flat'):
        OriginSphere = "\"(0,0,-10.0)\""
    minE = float(configuration["EffectiveArea"]["gramssky"]["minE"])
    maxE = float(configuration["EffectiveArea"]["gramssky"]["maxE"])
    energy_bins = int(configuration["EffectiveArea"]["gramssky"]["energy_bins"])
## cmd generation
    cmd_script_generation_cones(output_directory_base_path, "EffectiveArea", output_cmd_file,
    output_shell_file,tar_file, energy_bins)
## Generate energies for each job
    energies = np.logspace(math.log10(minE),math.log10(maxE),num=energy_bins)
    energy_str = "energies=( "
    for e in energies:
        energy_str = energy_str + ' \"'+str(e)+'\" '
    energy_str += ")\n"
# Start shell script construction
    shell_script_preamble(output_shell_file,tar_file)
## Start shell script
    with open(output_shell_file,"a") as f:
        f.write(energy_str)
### Create .mac file to process gramssky output
    nparticles = configuration["EffectiveArea"]["gramssky"]["nparticles"]
    with open("./GramsWork/mac/"+generic_gramssky_hepmc3_macro,'w') as f:
        f.write("/run/initialize\n")
        f.write("/run/beamOn "+str(nparticles)+"\n")
## Finish GramsSky command
    RadiusSphere = configuration["EffectiveArea"]["gramssky"]["RadiusSphere"]
    RadiusDisc = configuration["EffectiveArea"]["gramssky"]["RadiusDisc"]
    values = ["./gramssky","--options", "SensitivityOptions.xml"] 
    values += ["-o", "Events.hepmc3", "--RadiusSphere", RadiusSphere]
    values += ["--RadiusDisc", RadiusDisc, "--PositionGeneration", PositionGeneration]
    values += ["--PhiMinMax",PhiMinMax, "--ThetaMinMax", ThetaMinMax, "-n", nparticles]
    values += [" -s",  "${process}",  "-r" , "${process}"]
    values += [" --verbose"]
    values += ["--OriginSphere", OriginSphere, "--EnergyGeneration", "Fixed", "--FixedEnergy", "${energies[$process]}"]
    values += ["\n"]
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
# ## GramsG4
    values = []
    values += ["./gramsg4","--options", "SensitivityOptions.xml", '-g']
    if (Geo=="cube"):
        values += ["ThinGrams.gdml"]
    elif (Geo=="flat"):
        values += ["ThinFlatGrams.gdml"]
    values += ["-i","Events.hepmc3", "-s", "${process}", "-r", "${process}"]
    values += ["-o", "EffArea_${process}", "-m", "mac/"+ generic_gramssky_hepmc3_macro]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
# ## GramsDetSim
    values = []
    values = ["./gramsdetsim","--options", "SensitivityOptions.xml", "-i", "EffArea_${process}.root", "-s", "${process}"]
    values += ["-o" , "EffAreaDet_${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
# ## Extract
    values = ["./Extract", "--options", "SensitivityOptions.xml", "--GramsG4Name", "EffArea_${process}.root"]
    values += ["--GramsDetSimName" ,"EffAreaDet_${process}.root", "-o", "EffAreaExtract_${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
# ## Reconstruct 
    values = []
    values += ["./Reconstruct", "--options", "SensitivityOptions.xml", "-i", "EffAreaExtract_${process}.root"]
    values += ["-o","EffArea_Reco${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
# ## Clean up
    with open(output_shell_file,'a') as f:
        f.write("cp EffArea_Reco${process}.root ..\n")
        f.write("cd ..")
    subprocess.run(["tar", "-czf", tar_file, "GramsWork/"]) 

def SourceGeneration(configuration, JobType):
    current_time = datetime.datetime.now().isoformat()
    if(JobType=="Cones"):
        base_name = "Source_Cones_"+str(current_time)
    elif(JobType=="SkyMap"):
        base_name = "Source_SkyMap_"+str(current_time)
    else:
        print("Invalid Job type")
        sys.exit()
    base_name = base_name.replace(":",".")

    if(JobType=="Cones"):
        output_extract_name = base_name+"_Extract_${process}.root"
        output_cone_name = base_name+"_Reco_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
        generic_gramssky_hepmc3_macro = "GenericHepmc3.mac"
        shell_script_preamble(output_shell_file,tar_file)
        Geo = configuration["General"]["Geometry"].lower()
        SourceEnergy = configuration["Source"]["SourceEnergy"]
        SourceEventsPerJob = configuration["Source"]["SourceEventsPerJob"]
        SourceBatchs = configuration["Source"]["SourceBatches"]
        RASourceLoc = configuration["Source"]["RASourceLoc"]*np.pi/180
        ALTSourceLoc = configuration["Source"]["ALTSourceLoc"]*np.pi/180
        output_directory_base_path = config["General"]["output_directory"]
        SourceLoc = SphereToCart(RASourceLoc,ALTSourceLoc)
        PointLoc = "\"("+SourceLoc[0]+","+SourceLoc[1]+","+SourceLoc[2]+","+")\""
        PointLocSpherical = "\"("+RASourceLoc+","+ALTSourceLoc+")\""
        RadiusSphere = 300
        RadiusDisc = 200
        cmd_script_generation_cones(output_directory_base_path, "Source", JobType, output_cmd_file,
            output_shell_file,tar_file, SourceBatchs)
    # Create .mac file to process gramssky
        nparticles = configuration["Source"]["SourceEventsPerJob"]
        with open("./GramsWork/mac/"+generic_gramssky_hepmc3_macro,'w') as f:
            f.write("/run/initialize\n")
            f.write("/run/beamOn "+str(nparticles)+"\n")
## Gramssky part
        values = ["./gramssky", "-o","Events.hepmc3", "--RadiusSphere", RadiusSphere]
        values += ["--RadiusDisc", RadiusDisc]
        values += ["--PositionGeneration", "Point", "-n", SourceEventsPerJob]
        values += ["-s", "${process}","-r", "${process}"]
        if (Geo=="cube"):
            OriginSphere = "\"(0,0,-40.0)\""
        elif(Geo=="flat"):
            OriginSphere = "\"(0,0,-10.0)\""
        else:
            print("Unknown geometry")
            sys.exit()
        values += ["--OriginSphere", OriginSphere, "--EnergyGeneration", "Fixed"]
        values += ["--FixedEnergy", SourceEnergy]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
# GramsG4
        values = []
        values += ["./gramsg4"]
        if(Geo=="cube"):
            values += ["-g","ThinGrams.gdml"]
        elif(Geo=="flat"):
            values += ["-g","ThinFlatGrams.gdml"]
        else:
            print("Unknown Geometry")
            sys.exit()
        values += ["-i", "Events.hepmc3","-s", "${process}", "-r" ,"${process}"]
        values += ["-o","Source_${process}","-m","./mac/"+generic_gramssky_hepmc3_macro]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
## GramsDetSim
        values = []
        values += ["./gramsdetsim", "-i", "Source_${process}.root"]
        values += ["-s", "${process}", "-o" , "Source_Det_${process}.root"]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
## Extract
        values = []
        values += ["./Extract", "--GramsG4Name","Source_${process}.root","--GramsDetSimName","Source_Det_${process}.root"]
        values += ["-o",output_extract_name]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
# Generate cones
        values = []
        values += ["./Reconstruct", "-i",output_extract_name, "-o", output_cone_name]
        values += ["--SourceType", "Point", "--SourceLoc", PointLocSpherical]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
# clean up 
        with open(output_shell_file,'a') as f:
            f.write("cp "+output_extract_name+" ..\n")
            f.write("cp "+output_cone_name+" ..\n")
            f.write("cd ..\n")

    elif (JobType =="SkyMap"):
        output_directory_base_path = configuration["General"]["output_directory"]
        base_input_name = configuration["Source"]["SkyMap"]["SkyMapInputBaseName"]
        nbatches = configuration["Source"]["SourceBatches"]
        mask_output_name  = configuration["Mask"]["MaskOutput"]
        output_skymap_name = base_name+"_SkyMap_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
## Cmd generation
        cmd_script_generation_skymap(output_directory_base_path, base_input_name, Job, output_skymap_name, output_shell_file,tar_file, nbatches)
        input_file_name = base_input_name+"${process}.root"
        EffectiveAreaFile = configuration["CalcEffArea"]["BaseRecoName"]
        with open(output_shell_file,'w') as f:
            # generic setup for nevis cluster
            f.write("#!/bin/bash -l\n")
            f.write("process=$1\n")
            f.write("source /usr/nevis/adm/nevis-init.sh\n")
            f.write("conda activate /nevis/amsterdam/share/seligman/conda/grams\n")
            # unzip folder
            f.write("tar -xzf "+tar_file+"\n")
            # move input file to newly unzipped folder and cd to folder
            f.write("mv "+ input_file_name +"  GramsWork\n")
            f.write("cd  GramsWork\n")
            # Run unweighted calculation
            f.write("./GenSkyMap -i "+input_file_name+" -o "+output_skymap_name+" --MaskFile "+ mask_output_name+" --EffAreaFile "+EffectiveAreaFile+"\n")
            # move output back up 1 directory and cd up. Condor automatically transfers output
            f.write("mv "+output_skymap_name+" ..\n")
            f.write("cd ..\n")
    else:
        print("Unknown Geometry")
        sys.exit()
    subprocess.run(["tar", "-czf", tar_file, "GramsWork/"]) 

def BackgroundGeneration(configuration, JobType):
    current_time = datetime.datetime.now().isoformat()
    if(JobType=="Cones"):
        base_name = "Background_Cones_"+str(current_time)
    elif(JobType=="SkyMap"):
        base_name = "Background_SkyMap_"+str(current_time)
    else:
        print("Invalid Job type")
        sys.exit()
## Gramssky part
    if(JobType=="Cones"):
        base_name = base_name.replace(":",".")
        output_extract_name = base_name+"_Extract_${process}.root"
        output_cone_name = base_name+"_Reco_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
        generic_gramssky_hepmc3_macro = "GenericHepmc3.mac"
        shell_script_preamble(output_shell_file,tar_file)
        Geo = configuration["General"]["Geometry"].lower()
        output_directory_base_path = config["General"]["output_directory"]
        BackgroundEventsPerJob = configuration["Background"]["BackgroundEventsPerJob"]
        BackgroundBatches = configuration["Background"]["SourceBatches"]
        minE = configuration["Background"]["gramsky"]["minE"]
        maxE = configuration["Background"]["gramsky"]["maxE"]
        RadiusSphere = 300
        RadiusDisc = 200
        cmd_script_generation(output_directory_base_path, "Background", JobType, output_cmd_file,
            output_shell_file,tar_file, SourceBatchs)
# Create .mac file to process gramssky
        with open("./GramsWork/mac/"+generic_gramssky_hepmc3_macro,'w') as f:
            f.write("/run/initialize\n")
            f.write("/run/beamOn "+str(BackgroundEventsPerJob)+"\n")
        values = ["./gramssky", "-o","Events.hepmc3", "--RadiusSphere", RadiusSphere]
        values += ["--RadiusDisc", RadiusDisc]
        values += ["--PositionGeneration", "Iso", "-n", SourceEventsPerJob]
        values += ["--PhiMinMax" ,"\"(-3.14159,3.14159)\"", "--ThetaMinMax" ,"\"(0,3.14159265)\""]
        values += ["-s", "${process}","-r", "${process}"]
        if (Geo=="cube"):
            OriginSphere = "\"(0,0,-40.0)\""
        elif(Geo=="flat"):
            OriginSphere = "\"(0,0,-10.0)\""
        else:
            print("Unknown geometry")
            sys.exit()
        values += ["--OriginSphere", OriginSphere, "--EnergyGeneration", "Fixed"]
        values += ["--EnergyGeneration" "PowerLaw", "--PhotonIndex" "1"]
        values += ["--EnergyMin", minE, "--EnergyMax", maxE]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
# GramsG4
        values = []
        values += ["./gramsg4"]
        if(Geo=="cube"):
            values += ["-g","ThinGrams.gdml"]
        elif(Geo=="flat"):
            values += ["-g","ThinFlatGrams.gdml"]
        else:
            print("Unknown Geometry")
            sys.exit()
        values += ["-i", "Events.hepmc3","-s", "${process}", "-r" ,"${process}"]
        values += ["-o","Background_${process}","-m","./mac/"+generic_gramssky_hepmc3_macro]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
## GramsDetSim
        values = []
        values += ["./gramsdetsim", "-i", "Background_${process}.root"]
        values += ["-s", "${process}", "-o" , "Background_Det_${process}.root"]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
## Extract
        values = []
        values += ["./Extract", "--GramsG4Name","Background_${process}.root","--GramsDetSimName","Background_Det_${process}.root"]
        values += ["-o",output_extract_name]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
# Generate cones
        values = []
        values += ["./Reconstruct", "-i",output_extract_name, "-o", output_cone_name]
        values += ["--SourceType", "Iso"]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
# clean up 
        with open(output_shell_file,'a') as f:
            f.write("cp "+output_extract_name+" ..\n")
            f.write("cp "+output_cone_name+" ..\n")
            f.write("cd ..\n")
    elif(JobType=="SkyMap"):
        output_directory_base_path = configuration["General"]["output_directory"]
        base_input_name = configuration["Background"]["SkyMap"]["SkyMapInputBaseName"]
        nbatches = configuration["Background"]["BackgroundBatches"]
        mask_output_name  = configuration["Mask"]["MaskOutput"]
        output_skymap_name = base_name+"_SkyMap_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
## Cmd generation
        cmd_script_generation_skymap(output_directory_base_path, base_input_name, Job, output_skymap_name, output_shell_file,tar_file, nbatches)
        input_file_name = base_input_name+"${process}.root"
        EffectiveAreaFile = configuration["CalcEffArea"]["BaseRecoName"]
        TotalEvents = int(configuration["Background"]["BackgroundEventsPerJob"])*int(configuration["Background"]["BackgroundBatches"])
        PhysicalFluxFileName = "BackgroundFlux.root"
        ReferenceFluxFileName = "ReferenceFlux.root"
        GenFluxes(EffectiveAreaFile ,PhysicalFluxFileName,  ReferenceFluxFileName)
        subprocess(["mv", "BackgroundFlux.root" "GramsWork/."])
        subprocess(["mv", "ReferenceFlux.root" "GramsWork/."])
        ExposureTime = configuration["Background"]["SkyMap"]["ExposureTime"]
        with open(output_shell_file,'w') as f:
            # generic setup for nevis cluster
            f.write("#!/bin/bash -l\n")
            f.write("process=$1\n")
            f.write("source /usr/nevis/adm/nevis-init.sh\n")
            f.write("conda activate /nevis/amsterdam/share/seligman/conda/grams\n")
            # unzip folder
            f.write("tar -xzf "+tar_file+"\n")
            # move input file to newly unzipped folder and cd to folder
            f.write("mv "+ input_file_name +" GramsWork\n")
            f.write("cd  GramsWork\n")
            # Run weighted calculation
            f.write("./GenCountsHists -i "+ShellReconstructedDataFileName+" -o "+OutputFileName+" -w")
            f.write(" -t "+str(ExposureTime)+" --TotalEvents " +str(TotalEvents))
            f.write(" --EffAreaFile "+EffectiveAreaFile+" --PhysicalFluxFile "+PhysicalFluxFileName+
            " --ReferenceFluxFile "+ReferenceFluxFileName+" --MaskFile "+ mask_output_name+"\n")
            # move output back up 1 directory and cd up. Condor automatically transfers output
            f.write("mv "+output_skymap_name+" ..\n")
            f.write("cd ..\n")
    else:
        print("Unknown Geometry")
        sys.exit()
    subprocess.run(["tar", "-czf", tar_file, "GramsWork/"]) 

def SensitivityGeneration(configuration, verbose  = False):
    RootFolder = configuration["General"]["output_directory"]
    BackgroundCountFolder = os.path.join(RootFolder,"Background","SkyMap")
    BackgroundBaseName = configuration["Sensitivity"]["BackgroundBaseName"]
    SourceCountFolder = os.path.join(RootFolder,"Source","SkyMap")
    SourceBaseName = configuration["Sensitivity"]["SourceBaseName"]
    ExposureTime = configuration["Background"]["SkyMap"]["ExposureTime"]
    SourceEnergy = configuration["Source"]["SourceEnergy"]
    SourceEventsPerJob = configuration["Source"]["SourceEventsPerJob"]
## Currently source and background batches are coupled here. Need to fix this
    batches = configuration["Source"]["SourceBatches"]
    EffectiveAreaFile  = configuration["CalcEffArea"]["OutputFileName"]

    args = ["./CalcSensi","--options","SensitivityOptions.xml"]
    args += ["--BackgroundCountFolder", BackgroundCountFolder]
    args += ["--BackgroundBaseName", BackgroundBaseName]
    args += ["--ExposureTime", ExposureTime]
    args += ["--SourceCountsFolder",SourceCountFolder]
    args += ["--SourceCountsBaseName", SourceBaseName]
    args += ["--SourceEnergy", SourceEnergy]
    args += ["--Batches", batches]
    args += ["--EffectiveAreaRoot", EffectiveAreaFile]
    command  = ' '.join(args)
    args = shlex.split(command)
    subprocess.run(args)
    if(verbose):
        print(command)

if __name__ =="__main__":
    parser = argparse.ArgumentParser(
                        prog='GenCondorJobs',
                        description='Generates .sh and .cmd files for condor jobs to send to Nevis cluster')
    parser.add_argument("config",help="Path to .ini config file")
    parser.add_argument("--Job", default = "EffectiveArea", choices=["EffectiveArea","Source","Background","Sensitivity" ])
    parser.add_argument("--JobType", default="Cones", choices=["Cones","SkyMap"])
    args = parser.parse_args()

    try:
        with open(args.config,'rb') as f:
            config = tomllib.load(f)
    except:
        print("Couldn't read"+ args.config)
        sys.exit()

    if(args.Job=="EffectiveArea"):
        EffectiveAreaGeneration(config)
    elif (args.Job=="Source"):
        SourceGeneration(config,args.JobType)
    elif (args.Job=="Background"):
        BackgroundGeneration(config,args.JobType)
    elif (args.Job=="Sensitivity"):
        SensitivityGeneration(config)
    else:
        print("Invalid Job")