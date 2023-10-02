import tomllib
import os, sys, math
import argparse
import datetime
import numpy as np
import subprocess


def SphereToCart(Azi,Alt):
## Takes in Azi and Alt as radians and calculates x,y,z direction
    x_truth = np.cos(Azi)*np.cos(Alt)
    y_truth = np.sin(Azi)*np.cos(Alt)
    z_truth = np.sin(Alt)
    return np.array([x_truth,y_truth,z_truth])

def shell_script_preamble(shell_name, tar_file_name):
    with open(shell_name, 'w') as f:
        f.write("#!/bin/bash\n")
        f.write("process=$1\n")
        f.write("export PATH=/sbin:/usr/sbin:/bin:/usr/bin\n")
        f.write("export G4NEUTRONHPDATA=/usr/local/share/Geant4-10.5.1/data/G4NDL4.5\n")
        f.write("source /usr/nevis/adm/nevis-init.sh\n")
        f.write("conda activate /nevis/amsterdam/share/seligman/conda/grams\n")
## Extract folder and cd to directory to run GramsSim code
        f.write("tar -xzf "+tar_file_name+"\n")
        f.write("cd GramsWork\n")

def cmd_script_generation(output_directory_base_path, Job, JobType, cmd_name, shell_name,tar_name, nbatches):
    home = os.getcwd()
    shell_path = os.path.join(home,shell_name)
    tar_path = os.path.join(home,tar_name)
    condor_output = "temp-$(Process).out"
    condor_log = "temp-$(Process).log"
    condor_err = "temp-$(Process).err"
    if not (os.path.exists(output_directory_base_path)):
        print(output_directory_base_path + "doesn't exist")
        sys.exit()
    output_dir = os.path.join(output_directory_base_path,Job,JobType)
    try:
        os.makedirs(output_dir)
## If the directory already exists, then keep going
    except FileExistsError as err:
        pass
    except  Exception as err:
        print("couldn't create "+output_dir)
        sys.exit()
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

def EffectiveAreaGeneration(configuration, JobType):
    current_time = datetime.datetime.now().isoformat()
    base_name = "EffArea_"+str(current_time)
    base_name = base_name.replace(":",".")
    output_shell_file = base_name+".sh"
    output_cmd_file = base_name+".cmd"
    tar_file = base_name+".tar.gz"
    generic_gramssky_hepmc3_macro = "GenericHepmc3.mac"
    Geo = configuration["General"]["Geometry"].lower()
    output_directory_base_path = config["General"]["output_directory"]
# Constants
    PositionGeneration  = "Iso"
    PhiMinMax = "\"(-3.14159,3.14159)\""
    ThetaMinMax = "\"(0,3.14159265)\""
    if(Geo=='cube'):
        OriginSphere = "\"(0,0,-40.0 )\""
    if(Geo=='flat'):
        OriginSphere = "\"(0,0,-10.0)\""
# Gramssky
    minE = float(configuration["EffectiveArea"]["gramssky"]["minE"])
    maxE = float(configuration["EffectiveArea"]["gramssky"]["maxE"])
    energy_bins = int(configuration["EffectiveArea"]["gramssky"]["energy_bins"])
## Generate energies for each job
    energies = np.logspace(math.log10(minE),math.log10(maxE),num=energy_bins)
    energy_str = "energies=( "
    for e in energies:
        energy_str = energy_str + ' \"'+str(e)+'\" '
    energy_str += ")\n"
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
    RadiusSphere = 300
    RadiusDisc = 200
    values = ["./gramssky", "-o", "Events.hepmc3", "--RadiusSphere", RadiusSphere]
    values += ["--RadiusDisc", RadiusDisc, "--PositionGeneration", PositionGeneration]
    values += ["--PhiMinMax",PhiMinMax, "--ThetaMinMax", ThetaMinMax, "-n", nparticles]
    values += [" -s",  "${process}",  "-r" , "${process}"]
    values += ["--OriginSphere", OriginSphere, "--Fixed", "${energies[$process]}"]
    values += ["\n"]
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
## GramsG4
    values = []
    values += ["./gramsg4", '-g']
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
## GramsDetSim
    values = []
    values = ["./gramsdetsim", "-i", "EffArea_${process}.root", "-s", "${process}"]
    values += ["-o" , "EffAreaDet_${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
## Extract
    values = ["./Extract" ,"--GramsG4Name", "EffArea_${process}.root"]
    values += ["--GramsDetSimName" ,"EffAreaDet_${process}.root", "-o", "EffAreaExtract_${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
## Reconstruct 
    values = []
    values += ["./Reconstruct",  "-i", "EffAreaExtract_${process}.root"]
    values += ["-o","EffArea_Reco${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
## Clean up
    with open(output_shell_file,'a') as f:
        f.write("cp EffArea_Reco${process}.root ..\n")
        f.write("cd ..")
## cmd generation
    cmd_script_generation(output_directory_base_path, "EffectiveArea", JobType, output_cmd_file,
    output_shell_file,tar_file, energy_bins)
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
# Create .mac file to process gramssky
    nparticles = configuration["Source"]["SourceEventsPerJob"]
    with open("./GramsWork/mac/"+generic_gramssky_hepmc3_macro,'w') as f:
        f.write("/run/initialize\n")
        f.write("/run/beamOn "+str(nparticles)+"\n")
## Gramssky part
    if(JobType=="Cones"):
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
#############
#Fill out here
#############
        pass
    else:
        print("Unknown Geometry")
    cmd_script_generation(output_directory_base_path, "Source", JobType, output_cmd_file,
        output_shell_file,tar_file, SourceBatchs)
def BackgroundGeneration(configuration, JobType):
    current_time = datetime.datetime.now().isoformat()
    if(JobType=="Cones"):
        base_name = "Background_Cones_"+str(current_time)
    elif(JobType=="SkyMap"):
        base_name = "Background_SkyMap_"+str(current_time)
    else:
        print("Invalid Job type")
        sys.exit()
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
## Gramssky part
    if(JobType=="Cones"):
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
#############
#Fill out here
#############
        pass
    else:
        print("Unknown Geometry")
        sys.exit()
    cmd_script_generation(output_directory_base_path, "Background", JobType, output_cmd_file,
        output_shell_file,tar_file, SourceBatchs)
def SensitivityGeneration(configuration):
    pass

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
        EffectiveAreaGeneration(config,args.JobType)
    elif (args.Job=="Source"):
        SourceGeneration(config,args.JobType)
    elif (args.Job=="Background"):
        BackgroundGeneration(config,args.JobType)
    elif (args.Job=="Sensitivity"):
        SensitivityGeneration(config)
    else:
        print("Invalid Job")