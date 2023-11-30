import toml
import os, sys, glob, math
import argparse
import datetime
import numpy as np
import subprocess
from  ROOT import TFile

def get_keys(config):
    # Recursively gather all the keys in a dictionary of nested dictionaries
    output = []
    for k,v in config.items():
    # Recurse: The items are dictionaries, so they can possible contain more keys themselves. Recurse on each dictionary v
        if isinstance(v,dict):
            output += [k]
            output += get_keys(v)
        else:
    # base case: There are no additional dictionaries in the items. Put key into output
            output += [k]
    return output

def remove_key(lok, key):
    # try to remove key from list of keys (lok)
    # If key is not found, then return False. Otherwise, return True
    if key in lok:
        lok.remove(key)
        return True
    return False

def ParameterValidityCheck(configuration):
    # Function that makes sure that all the parameters in configuration are valid values
    # This is needed becuase tomllib only checks if the .toml Config file is a valid .toml file

    # Returns Tuple where first element is boolean on wheather all parameters are valid in the context of the simulation
    # Second element is some string: either an error message or sucess message 

    # Get all keys in configurations. Remove from this list for each header/attribute we check
    # Duplicate keys are allowed (so if you have a subheader named "fizz",
    # a under two different main headers, you will have two "fizz" in the list. This is OK)
    possible_keys = get_keys(configuration)

    # General section
    if not remove_key(possible_keys,"General"):
        return (False, "'General' header not found")
    cur_attr = configuration["General"]

    ## Geometry
    if not remove_key(possible_keys,"Geometry"):
        return (False, "'Geometry' attribute not found")
    configuration["General"]["Geometry"] = configuration["General"]["Geometry"].lower()
    cur_attr = configuration["General"]["Geometry"]
    if (cur_attr != "cube") and (cur_attr != "flat"):
        return (False, "Invalid Geometry (must be 'cube' or 'flat')")

    ## RA and ALT binnings
    if not remove_key(possible_keys,"RABinnings"):
        return (False, "'RABinnings' attribute not found")
    cur_attr = configuration["General"]["RABinnings"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret RABinnings as an integer")
    if(temp <=0):
        return (False, "RABinnings must be strictly greater than 0")

    if not remove_key(possible_keys,"ALTBinnings"):
        return (False, "'ALTBinnings' attribute not found")
    cur_attr = configuration["General"]["ALTBinnings"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret ALTBinnings as an integer")
    if(temp <=0):
        return (False, "ALTBinnings must be strictly greater than 0")

    ## base output directory
    if not remove_key(possible_keys,"output_directory"):
        return (False, "'output_directory' attribute not found")
    cur_attr = configuration["General"]["output_directory"]
    if not (os.path.exists(cur_attr)):
        print("Base output dirctory currently does not exist. Attempting to create...")
        try:
            os.mkdir(cur_attr)
        except:
            return (False, "Could not create base directory"+cur_attr)
        print("directory created")

    ## MC_truth
    if not remove_key(possible_keys,"MC_truth"):
        return (False, "'MC_truth' attribute not found")
    cur_attr = configuration["General"]["MC_truth"]
    if not isinstance(cur_attr, bool):
        return (False, "MC_truth must be a boolean")

    ## scale
    if not remove_key(possible_keys,"scale"):
        return (False, "'scale' attribute not found")
    configuration["General"]["scale"] = configuration["General"]["scale"].lower()
    cur_attr = configuration["General"]["scale"]
    if (cur_attr != "linear") and (cur_attr != "log"):
        return (False, "Invalid scale (must be 'linear' or 'log')")
    
    # Effective Area
    if not remove_key(possible_keys,"EffectiveArea"):
        return (False, "'EffectiveArea' header not found")
    cur_attr = configuration["EffectiveArea"]

    ## gramssky
    if not remove_key(possible_keys,"gramssky"):
        return (False, "'gramssky' header not found")
    cur_attr = configuration["EffectiveArea"]["gramssky"]

    ### Radius Sphere
    if not remove_key(possible_keys,"RadiusSphere"):
        return (False, "'RadiusSphere' attribute not found")
    cur_attr = configuration["EffectiveArea"]["gramssky"]["RadiusSphere"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret RadiusSphere as an integer")
    if(temp <=0):
        return (False, "RadiusSphere must be strictly greater than 0")

    ### Radius Disk
    if not remove_key(possible_keys,"RadiusDisc"):
        return (False, "'RadiusDisc' attribute not found")
    cur_attr = configuration["EffectiveArea"]["gramssky"]["RadiusDisc"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret RadiusDisc as an integer")
    if(temp <=0):
        return (False, "RadiusDisc must be strictly greater than 0")

    ### minE
    if not remove_key(possible_keys,"minE"):
        return (False, "'minE' attribute not found")
    cur_attr = configuration["EffectiveArea"]["gramssky"]["minE"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret minE as a float")
    minE = temp
    if(temp <=0):
        return (False, "minE must be strictly greater than 0")

    ### maxE
    if not remove_key(possible_keys,"maxE"):
        return (False, "'maxE' attribute not found")
    cur_attr = configuration["EffectiveArea"]["gramssky"]["maxE"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret maxE as a float")
    if(temp <=0):
        return (False, "maxE must be strictly greater than 0")
    if (temp <= minE):
        return (False, "'maxE' must be strictly larger than 'minE'")
    ### energy_bins
    if not remove_key(possible_keys,"energy_bins"):
        return (False, "'energy_bins' attribute not found")
    cur_attr = configuration["EffectiveArea"]["gramssky"]["energy_bins"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret energy_bins as an integer")
    if(temp <=0):
        return (False, "energy_bins must be strictly greater than 0")

    ### nparticles
    if not remove_key(possible_keys,"nparticles"):
        return (False, "'nparticles' attribute not found")
    cur_attr = configuration["EffectiveArea"]["gramssky"]["nparticles"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret nparticles as an integer")
    if(temp <=0):
        return (False, "nparticles must be strictly greater than 0")

    ## gramsg4
    if not remove_key(possible_keys,"gramsg4"):
        return (False, "'gramsg4' header not found")
    cur_attr = configuration["EffectiveArea"]["gramsg4"]

    ## gramsdetsim
    if not remove_key(possible_keys,"gramsdetsim"):
        return (False, "'gramsdetsim' header not found")
    cur_attr = configuration["EffectiveArea"]["gramsdetsim"]

    ## Extract
    if not remove_key(possible_keys,"Extract"):
        return (False, "'Extract' header not found")
    cur_attr = configuration["EffectiveArea"]["Extract"]

    ### ExtractOutput
    if not remove_key(possible_keys,"ExtractOutput"):
        return (False, "'Extract' header not found")
    cur_attr = configuration["EffectiveArea"]["Extract"]["ExtractOutput"]
    if(len(cur_attr) ==0):
        return (False, "'ExtractOutput' must have a non-zero length")

    ## Reconstruct
    if not remove_key(possible_keys,"Reconstruct"):
        return (False, "'Reconstruct' header not found")
    cur_attr = configuration["EffectiveArea"]["Reconstruct"]

    ### ReconstructOutput
    if not remove_key(possible_keys,"ReconstructOutput"):
        return (False, "'ReconstructOutput' header not found")
    cur_attr = configuration["EffectiveArea"]["Reconstruct"]["ReconstructOutput"]
    if(len(cur_attr) ==0):
        return (False, "'ReconstructOutput' must have a non-zero length")

    # Source
    if not remove_key(possible_keys,"Source"):
        return (False, "'Source' header not found")
    cur_attr = configuration["Source"]

    # Source Energy
    if not remove_key(possible_keys,"SourceEnergy"):
        return (False, "'SourceEnergy' header not found")
    cur_attr = configuration["Source"]["SourceEnergy"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret SourceEnergy as a float")
    if(temp <=0):
        return (False, "SourceEnergy must be strictly greater than 0")

    # SourceEventsPerJob
    if not remove_key(possible_keys,"SourceEventsPerJob"):
        return (False, "'SourceEventsPerJob' header not found")
    cur_attr = configuration["Source"]["SourceEventsPerJob"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret SourceEventsPerJob as a float")
    if(temp <=0):
        return (False, "SourceEventsPerJob must be strictly greater than 0")

    # SourceEventsPerJob
    if not remove_key(possible_keys,"SourceBatches"):
        return (False, "'SourceBatches' header not found")
    cur_attr = configuration["Source"]["SourceBatches"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret SourceBatches as a float")
    if(temp <=0):
        return (False, "SourceBatches must be strictly greater than 0")

    # RASourceLoc
    if not remove_key(possible_keys,"RASourceLoc"):
        return (False, "'RASourceLoc' header not found")
    cur_attr = configuration["Source"]["RASourceLoc"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret RASourceLoc as a float")
    if(temp <0) or (temp > 180):
        return (False, "RASourceLoc must be between 0 and 180 degrees (inclusive)")

    # ALTSourceLoc
    if not remove_key(possible_keys,"ALTSourceLoc"):
        return (False, "'ALTSourceLoc' header not found")
    cur_attr = configuration["Source"]["ALTSourceLoc"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret ALTSourceLoc as a float")
    if(temp <0) or (temp > 90):
        return (False, "ALTSourceLoc must be between 0 and 90 degrees (inclusive)")

    ## gramssky
    if not remove_key(possible_keys,"gramssky"):
        return (False, "'gramssky' header not found")
    cur_attr = configuration["Source"]["gramssky"]

    ## gramsg4
    if not remove_key(possible_keys,"gramsg4"):
        return (False, "'gramsg4' header not found")
    cur_attr = configuration["Source"]["gramsg4"]

    ## gramsdetsim
    if not remove_key(possible_keys,"gramsdetsim"):
        return (False, "'gramsdetsim' header not found")
    cur_attr = configuration["Source"]["gramsdetsim"]

    ## Extract
    if not remove_key(possible_keys,"Extract"):
        return (False, "'Extract' header not found")
    cur_attr = configuration["Source"]["Extract"]

    ### ExtractOutput
    if not remove_key(possible_keys,"ExtractOutput"):
        return (False, "'ExtractOutput' header not found")
    cur_attr = configuration["Source"]["Extract"]["ExtractOutput"]
    if(len(cur_attr) ==0):
        return (False, "'ExtractOutput' must have a non-zero length")

    ## Reconstruct
    if not remove_key(possible_keys,"Reconstruct"):
        return (False, "'Reconstruct' header not found")
    cur_attr = configuration["Source"]["Reconstruct"]

    ### ReconstructOutput
    if not remove_key(possible_keys,"ReconstructOutput"):
        return (False, "'ReconstructOutput' header not found")
    cur_attr = configuration["Source"]["Reconstruct"]["ReconstructOutput"]
    if(len(cur_attr) ==0):
        return (False, "'ReconstructOutput' must have a non-zero length")

    ## SkyMap
    if not remove_key(possible_keys,"SkyMap"):
        return (False, "'SkyMap' header not found")
    cur_attr = configuration["Source"]["SkyMap"]

    ### SkyMapOutput
    if not remove_key(possible_keys,"SkyMapOutput"):
        return (False, "'SkyMapOutput' header not found")
    cur_attr = configuration["Source"]["SkyMap"]["SkyMapOutput"]
    if(len(cur_attr) ==0):
        return (False, "'SkyMapOutput' must have a non-zero length")
    
    # Background
    if not remove_key(possible_keys,"Background"):
        return (False, "'Background' header not found")
    cur_attr = configuration["Background"]

    # BackgroundEventsPerJob
    if not remove_key(possible_keys,"BackgroundEventsPerJob"):
        return (False, "'BackgroundEventsPerJob' header not found")
    cur_attr = configuration["Background"]["BackgroundEventsPerJob"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret BackgroundEventsPerJob as a float")
    if(temp <=0):
        return (False, "BackgroundEventsPerJob must be strictly greater than 0")

    # BackgroundBatches
    if not remove_key(possible_keys,"BackgroundBatches"):
        return (False, "'BackgroundBatches' header not found")
    cur_attr = configuration["Background"]["BackgroundBatches"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret BackgroundBatches as a float")
    if(temp <=0):
        return (False, "BackgroundBatches must be strictly greater than 0")

    # gramssky
    if not remove_key(possible_keys,"gramssky"):
        return (False, "'gramssky' header not found")
    cur_attr = configuration["Background"]["gramssky"]

    ### minE
    if not remove_key(possible_keys,"minE"):
        return (False, "'minE' attribute not found")
    cur_attr = configuration["Background"]["gramssky"]["minE"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret minE as a float")
    minE = temp
    if(temp <=0):
        return (False, "minE must be strictly greater than 0")

    ### maxE
    if not remove_key(possible_keys,"maxE"):
        return (False, "'maxE' attribute not found")
    cur_attr = configuration["Background"]["gramssky"]["maxE"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret maxE as a float")
    if(temp <=0):
        return (False, "maxE must be strictly greater than 0")
    if (temp <= minE):
        return (False, "'maxE' must be strictly larger than 'minE'")

    ## gramsg4
    if not remove_key(possible_keys,"gramsg4"):
        return (False, "'gramsg4' header not found")
    cur_attr = configuration["Background"]["gramsg4"]

    ## gramsdetsim
    if not remove_key(possible_keys,"gramsdetsim"):
        return (False, "'gramsdetsim' header not found")
    cur_attr = configuration["Background"]["gramsdetsim"]

    ## Extract
    if not remove_key(possible_keys,"Extract"):
        return (False, "'Extract' header not found")
    cur_attr = configuration["Background"]["Extract"]

    ### ExtractOutput
    if not remove_key(possible_keys,"ExtractOutput"):
        return (False, "'ExtractOutput' header not found")
    cur_attr = configuration["Background"]["Extract"]["ExtractOutput"]
    if(len(cur_attr) ==0):
        return (False, "'ExtractOutput' must have a non-zero length")

    ## Reconstruct
    if not remove_key(possible_keys,"Reconstruct"):
        return (False, "'Reconstruct' header not found")
    cur_attr = configuration["Background"]["Reconstruct"]

    ### ReconstructOutput
    if not remove_key(possible_keys,"ReconstructOutput"):
        return (False, "'ReconstructOutput' header not found")
    cur_attr = configuration["Background"]["Reconstruct"]["ReconstructOutput"]
    if(len(cur_attr) ==0):
        return (False, "'ReconstructOutput' must have a non-zero length")

    ## SkyMap
    if not remove_key(possible_keys,"SkyMap"):
        return (False, "'SkyMap' header not found")
    cur_attr = configuration["Background"]["SkyMap"]

    ### SkyMapOutput
    if not remove_key(possible_keys,"SkyMapOutput"):
        return (False, "'SkyMapOutput' header not found")
    cur_attr = configuration["Background"]["SkyMap"]["SkyMapOutput"]
    if(len(cur_attr) ==0):
        return (False, "'SkyMapOutput' must have a non-zero length")

    ## ExposureTime
    if not remove_key(possible_keys,"ExposureTime"):
        return (False, "'ExposureTime' attribute not found")
    cur_attr = configuration["Background"]["SkyMap"]["ExposureTime"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret ExposureTime as a float")
    if(temp <=0):
        return (False, "ExposureTime must be strictly greater than 0")

    ## Mask
    if not remove_key(possible_keys,"Mask"):
        return (False, "'Mask' header not found")
    cur_attr = configuration["Mask"]

    # nevents
    if not remove_key(possible_keys,"nevents"):
        return (False, "'nevents' header not found")
    cur_attr = configuration["Mask"]["nevents"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret nevents as a float")
    if(temp <=0):
        return (False, "nevents must be strictly greater than 0")

    # nbins
    if not remove_key(possible_keys,"nbins"):
        return (False, "'nbins' header not found")
    cur_attr = configuration["Mask"]["nbins"]
    try:
        temp = int(cur_attr)
    except:
        return (False,"Couldn't interpret nbins as a float")
    if(temp <=0):
        return (False, "nbins must be strictly greater than 0")

    ### MaskOutput
    if not remove_key(possible_keys,"MaskOutput"):
        return (False, "'MaskOutput' header not found")
    cur_attr = configuration["Mask"]["MaskOutput"]
    if(len(cur_attr) ==0):
        return (False, "'MaskOutput' must have a non-zero length")

    ## draw
    if not remove_key(possible_keys,"draw"):
        return (False, "'draw' attribute not found")
    cur_attr = configuration["Mask"]["draw"]
    if not isinstance(cur_attr, bool):
        return (False, "draw must be a boolean")

    ## CalcEffArea
    if not remove_key(possible_keys,"CalcEffArea"):
        return (False, "'CalcEffArea' header not found")
    cur_attr = configuration["CalcEffArea"]

    ### OutputFileName
    if not remove_key(possible_keys,"OutputFileName"):
        return (False, "'OutputFileName' header not found")
    cur_attr = configuration["CalcEffArea"]["OutputFileName"]
    if(len(cur_attr) ==0):
        return (False, "'OutputFileName' must have a non-zero length")

    ## Sensitivity
    if not remove_key(possible_keys,"Sensitivity"):
        return (False, "'Sensitivity' header not found")
    cur_attr = configuration["Sensitivity"]

    ### minE
    if not remove_key(possible_keys,"TimeScaling"):
        return (False, "'TimeScaling' attribute not found")
    cur_attr = configuration["Sensitivity"]["TimeScaling"]
    try:
        temp = float(cur_attr)
    except:
        return (False,"Couldn't interpret TimeScaling as a float")
    if(temp <=0):
        return (False, "TimeScaling must be strictly greater than 0")

    if(len(possible_keys) > 0):
        print("\n\n\n")
        for key in possible_keys:
            print("Parameter: " +str(key)+" not found")
        return (False, "Missing headers/attributes")

    return (True,"Parameters are sane...")



def SanityCheck(output_directory):
    # Function to make sure that you aren't overwriting old files by accident
    if not (os.path.isdir(output_directory)):
        print(output_directory+" doesn't exist")
        sys.exit()
    for file in os.listdir(output_directory):
        if file.endswith(".root"):
            print(output_directory+ " already has .root files in it.")
            user_input = input("Are you sure you have the correct output directory? (You can potentially overwrite these files with the resulting files if Y is selected) [Y/N]")
            if(user_input.lower().strip() == "y"):
                return True
            else:
                print("Ok. Terminating program...")
                sys.exit()
    return True


def BrokenPowerLaw(E,**kwargs):
    ## Flux function must have the form func(E,**kwargs)
    ## Please make sure that you sanitize your inputs like below when you write your own
    ## Also, physical fluxes should have units of 1/(MeV cm^2 s sr) to be consistent with the rest of the programs

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
    ## Generate reference and physical background flux for background cone generation
    home = os.getcwd()
    os.chdir(os.path.join(home,"GramsWork"))
    args = parser.parse_args()
    EffAreaFile = TFile(EffectiveAreaFile, "READ")
    EffAreaHist = EffAreaFile.EffArea
    BackgroundFile = TFile(PhysicalFluxName ,"RECREATE")
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
    ReferenceFile = TFile(ReferenceFluxName ,"RECREATE")
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
    os.chdir(home)

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

def cmd_script_generation_cones(output_directory_base_path, Job, cmd_name, shell_name,tar_name, nbatches, batch_mode):
    # Create absolute paths to location of shell script, tar file and name temporary files
    home = os.getcwd()
    shell_path = os.path.join(home,shell_name)
    tar_path = os.path.join(home,tar_name)
    condor_output = "temp-$(Process).out"
    condor_log = "temp-$(Process).log"
    condor_err = "temp-$(Process).err"
    ## Make sure that output folder exists.
    if not (os.path.exists(output_directory_base_path)):
        print(output_directory_base_path + " doesn't exist")
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
    output_dir = os.path.join(output_directory_base_path,Job,"Cones")
    try:
        os.makedirs(output_dir)
    ## If the directory already exists, then keep going
    except FileExistsError as err:
        pass
    except  Exception as err:
        print("couldn't create "+output_dir)
        sys.exit()
    if not batch_mode:
        SanityCheck(output_dir)
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

def cmd_script_generation_skymap(output_directory_base_path, base_input_name, Job, cmd_name, shell_name,tar_name, nbatches, batch_mode):
    # Create absolute paths to location of shell script, tar file and name temporary files
    home = os.getcwd()
    shell_path = os.path.join(home,shell_name)
    tar_path = os.path.join(home,tar_name)
    condor_output = "temp-$(Process).out"
    condor_log = "temp-$(Process).log"
    condor_err = "temp-$(Process).err"
    ## Make sure that output folder exists.
    if not (os.path.exists(output_directory_base_path)):
        print(output_directory_base_path + " doesn't exist")
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
    if not batch_mode:
        SanityCheck(output_dir)
    ## Check that the input cone files are present in the correct directory
    input_file_pattern = base_input_name+"*.root"
    data_dir = os.path.join(output_directory_base_path,Job,"Cones")
    if not (os.path.exists(data_dir)):
        print(data_dir + " doesn't exist")
        sys.exit()
    ## glob magic checks if any files have the correct naming scheme
    if len(glob.glob(os.path.join(data_dir,input_file_pattern))) == 0:
        print(data_dir + " doesn't contain cone files")
        sys.exit()
    input_data_file = os.path.join(data_dir, base_input_name+"_$(Process).root")
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

def EffectiveAreaGeneration(configuration, batch_mode):
    current_time = datetime.datetime.now().isoformat()
    base_name = "EffArea_"+str(current_time)
    base_name = base_name.replace(":",".")
    output_root_file_base = configuration["EffectiveArea"]["Reconstruct"]["ReconstructOutput"]
    output_shell_file = base_name+".sh"
    output_cmd_file = base_name+".cmd"
    tar_file = base_name+".tar.gz"
    generic_gramssky_hepmc3_macro = "GenericHepmc3.mac"
    Geo = configuration["General"]["Geometry"]
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
    output_shell_file,tar_file, energy_bins, batch_mode)
    ## Generate energies for each job. Uses "scale" to decide what scaliing the x axis should be (linear or log)
    if(configuration["General"]["scale"].lower() == "linear"):
        energies = np.linspace(minE,maxE,num=energy_bins)
    elif (configuration["General"]["scale"].lower() == "log"):
        energies = np.logspace(math.log10(minE),math.log10(maxE),num=energy_bins)
    ## This shouldn't happen, but you never know!
    else:
        print("Invalid scale for x axis of Effective Area")
        sys.exit()
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
    values = ["./Extract","--options", "SensitivityOptions.xml"]
    if(configuration["General"]["MC_truth"]):
        values += ["--MCTruth"]
    values += [ "--GramsG4Name", "EffArea_${process}.root"]
    values += ["--GramsDetSimName" ,"EffAreaDet_${process}.root"]
    values += ["-o", "EffAreaExtract_${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
    # ## Reconstruct 
    values = []
    values += ["./Reconstruct","--options", "SensitivityOptions.xml"]
    if(configuration["General"]["MC_truth"]):
        values += ["--MCTruth"]
    values += ["-i", "EffAreaExtract_${process}.root"]
    values += ["-o",output_root_file_base+"_${process}.root"]
    values += ['\n']
    command = " ".join([str(v) for v in values])
    with open(output_shell_file,'a') as f:
        f.write(command)
    # ## Clean up
    with open(output_shell_file,'a') as f:
        f.write("cp "+ output_root_file_base+"_${process}.root" +" ..\n")
        f.write("cd ..")
    subprocess.run(["tar", "-czf", tar_file, "GramsWork/"]) 

def SourceGeneration(configuration, JobType, batch_mode):
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
        output_extract_name = configuration["Source"]["Extract"]["ExtractOutput"]+"_${process}.root"
        output_cone_name = configuration["Source"]["Reconstruct"]["ReconstructOutput"]+"_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
        generic_gramssky_hepmc3_macro = "GenericHepmc3.mac"
        shell_script_preamble(output_shell_file,tar_file)
        Geo = configuration["General"]["Geometry"]
        SourceEnergy = configuration["Source"]["SourceEnergy"]
        SourceEventsPerJob = configuration["Source"]["SourceEventsPerJob"]
        SourceBatchs = configuration["Source"]["SourceBatches"]
        RASourceLoc = configuration["Source"]["RASourceLoc"]*np.pi/180
        ALTSourceLoc = configuration["Source"]["ALTSourceLoc"]*np.pi/180
        output_directory_base_path = config["General"]["output_directory"]
        SourceLoc = SphereToCart(RASourceLoc,ALTSourceLoc)
        PointLoc = "\"("+str(SourceLoc[0])+","+str(SourceLoc[1])+","+str(SourceLoc[2])+")\""
        PointLocSpherical = "\"("+str(RASourceLoc)+","+str(ALTSourceLoc)+")\""
        RadiusSphere = 300
        RadiusDisc = 200
        cmd_script_generation_cones(output_directory_base_path, "Source", output_cmd_file, output_shell_file,tar_file, SourceBatchs, batch_mode)
    # Create .mac file to process gramssky
        nparticles = configuration["Source"]["SourceEventsPerJob"]
        with open("./GramsWork/mac/"+generic_gramssky_hepmc3_macro,'w') as f:
            f.write("/run/initialize\n")
            f.write("/run/beamOn "+str(nparticles)+"\n")
    ## Gramssky part
        values = ["./gramssky", "--options", "SensitivityOptions.xml", "-o","Events.hepmc3", "--RadiusSphere", RadiusSphere]
        values += ["--RadiusDisc", RadiusDisc]
        values += ["--PositionGeneration", "Point", "-n", SourceEventsPerJob]
        values += ["--PointSource ", PointLoc]
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
        values += ["./gramsg4","--options", "SensitivityOptions.xml"]
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
        values += ["./gramsdetsim", "--options", "SensitivityOptions.xml", "-i", "Source_${process}.root"]
        values += ["-s", "${process}", "-o" , "Source_Det_${process}.root"]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
    ## Extract
        values = []
        values += ["./Extract",  "--options", "SensitivityOptions.xml"]
        if(configuration["General"]["MC_truth"]):
            values += ["--MCTruth"]
        values += [ "--GramsG4Name","Source_${process}.root","--GramsDetSimName","Source_Det_${process}.root"]
        values += ["-o",output_extract_name]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
    # Generate cones
        values = []
        values += ["./Reconstruct", "--options", "SensitivityOptions.xml"]
        if(configuration["General"]["MC_truth"]):
            values += ["--MCTruth"]
        values += [  "-i",output_extract_name]
        values += [ "-o", output_cone_name]
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
        base_input_name = configuration["Source"]["Reconstruct"]["ReconstructOutput"]
        nbatches = configuration["Source"]["SourceBatches"]
        mask_output_name  = configuration["Mask"]["MaskOutput"]
        output_skymap_name = configuration["Source"]["SkyMap"]["SkyMapOutput"]+"_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
    ## Cmd generation
        cmd_script_generation_skymap(output_directory_base_path, base_input_name, "Source", output_cmd_file, output_shell_file,tar_file, nbatches, batch_mode)
        input_file_name = base_input_name+"_${process}.root"
        EffectiveAreaFile = configuration["CalcEffArea"]["OutputFileName"]
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
            f.write("./GenSkyMap"+ " --options SensitivityOptions.xml "+ " -i "+input_file_name+" -o "+output_skymap_name+" --MaskFile "+ mask_output_name+" --EffAreaFile "+EffectiveAreaFile+"\n")
            # move output back up 1 directory and cd up. Condor automatically transfers output
            f.write("mv "+output_skymap_name+" ..\n")
            f.write("cd ..\n")
    else:
        print("Unknown Geometry")
        sys.exit()
    subprocess.run(["tar", "-czf", tar_file, "GramsWork/"]) 

def BackgroundGeneration(configuration, JobType, batch_mode):
    current_time = datetime.datetime.now().isoformat()
    if(JobType=="Cones"):
        base_name = "Background_Cones_"+str(current_time)
    elif(JobType=="SkyMap"):
        base_name = "Background_SkyMap_"+str(current_time)
    else:
        print("Invalid Job type")
        sys.exit()
    base_name = base_name.replace(":",".")
    ## Gramssky part
    if(JobType=="Cones"):
        output_extract_name = configuration["Background"]["Extract"]["ExtractOutput"]+"_${process}.root"
        output_cone_name = configuration["Background"]["Reconstruct"]["ReconstructOutput"]+"_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
        generic_gramssky_hepmc3_macro = "GenericHepmc3.mac"
        shell_script_preamble(output_shell_file,tar_file)
        Geo = configuration["General"]["Geometry"]
        output_directory_base_path = config["General"]["output_directory"]
        BackgroundEventsPerJob = configuration["Background"]["BackgroundEventsPerJob"]
        BackgroundBatches = configuration["Background"]["BackgroundBatches"]
        minE = configuration["Background"]["gramssky"]["minE"]
        maxE = configuration["Background"]["gramssky"]["maxE"]
        RadiusSphere = 300
        RadiusDisc = 200
        cmd_script_generation_cones(output_directory_base_path, "Background", output_cmd_file,  output_shell_file,tar_file, BackgroundBatches, batch_mode)
    # Create .mac file to process gramssky
        with open("./GramsWork/mac/"+generic_gramssky_hepmc3_macro,'w') as f:
            f.write("/run/initialize\n")
            f.write("/run/beamOn "+str(BackgroundEventsPerJob)+"\n")
        values = ["./gramssky", "--options", "SensitivityOptions.xml", "-o","Events.hepmc3", "--RadiusSphere", RadiusSphere]
        values += ["--RadiusDisc", RadiusDisc]
        values += ["--PositionGeneration", "Iso", "-n", BackgroundEventsPerJob]
        values += ["--PhiMinMax" ,"\"(-3.14159,3.14159)\"", "--ThetaMinMax" ,"\"(-1.57079,1.57079)\""]
        values += ["-s", "${process}","-r", "${process}"]
        if (Geo=="cube"):
            OriginSphere = "\"(0,0,-40.0)\""
        elif(Geo=="flat"):
            OriginSphere = "\"(0,0,-10.0)\""
        else:
            print("Unknown geometry")
            sys.exit()
        values += ["--OriginSphere", OriginSphere]
        values += ["--EnergyGeneration" ,"PowerLaw", "--PhotonIndex", "1"]
        values += ["--EnergyMin", minE, "--EnergyMax", maxE]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
    # GramsG4
        values = []
        values += ["./gramsg4","--options", "SensitivityOptions.xml"]
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
        values += ["./gramsdetsim", "--options", "SensitivityOptions.xml", "-i", "Background_${process}.root"]
        values += ["-s", "${process}", "-o" , "Background_Det_${process}.root"]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
    ## Extract
        values = []
        values += ["./Extract","--options", "SensitivityOptions.xml"]
        if(configuration["General"]["MC_truth"]):
            values += ["--MCTruth"]
        values += [ "--GramsG4Name","Background_${process}.root","--GramsDetSimName","Background_Det_${process}.root"]
        values += ["-o",output_extract_name]
        values += ["\n"]
        command = " ".join([str(v) for v in values])
        with open(output_shell_file,'a') as f:
            f.write(command)
    # Generate cones
        values = []
        values += ["./Reconstruct","--options", "SensitivityOptions.xml"]
        if(configuration["General"]["MC_truth"]):
            values += ["--MCTruth"]
        [  "-i",output_extract_name]
        values += [ "-o", output_cone_name]
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
        base_input_name = configuration["Background"]["Reconstruct"]["ReconstructOutput"]
        nbatches = configuration["Background"]["BackgroundBatches"]
        mask_output_name  = configuration["Mask"]["MaskOutput"]
        output_skymap_name = base_name+"_SkyMap_${process}.root"
        output_shell_file = base_name+".sh"
        output_cmd_file = base_name+".cmd"
        tar_file = base_name+".tar.gz"
    ## Cmd generation
        cmd_script_generation_skymap(output_directory_base_path, base_input_name, "Background", output_cmd_file, output_shell_file,tar_file, nbatches, batch_mode)
        input_file_name = base_input_name+"_${process}.root"
        output_skymap_name = configuration["Background"]["SkyMap"]["SkyMapOutput"]+"_${process}.root"
        EffectiveAreaFile = configuration["CalcEffArea"]["OutputFileName"]
        TotalEvents = int(configuration["Background"]["BackgroundEventsPerJob"])*int(configuration["Background"]["BackgroundBatches"])
        PhysicalFluxFileName = "BackgroundFlux.root"
        ReferenceFluxFileName = "ReferenceFlux.root"
        GenFluxes(EffectiveAreaFile ,PhysicalFluxFileName,  ReferenceFluxFileName)
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
            f.write("./GenSkyMap "+" --options SensitivityOptions.xml "+ " -i "+input_file_name+" -o "+output_skymap_name+" -w")
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

if __name__ =="__main__":
    parser = argparse.ArgumentParser(
                        prog='GenCondorJobs',
                        description='Generates .sh and .cmd files for condor jobs to send to Nevis cluster')
    parser.add_argument("config",help="Path to .toml config file")
    parser.add_argument("--Job", default = "EffectiveArea", choices=["EffectiveArea","Source","Background" ])
    parser.add_argument("--JobType", default="Cones", choices=["Cones","SkyMap"])
    parser.add_argument('-b', '--batch', action='store_true', help = "Run script without prompting. Turns off sanity check on wheather output directory is empty")
    args = parser.parse_args()

    try:
        config = toml.load(args.config)
    except:
        print("Couldn't read"+ args.config)
        sys.exit()
    opt = ParameterValidityCheck(config)
    print(opt[1])
    if not opt[0]:
        sys.exit()
## Using config dictionary to also pass down filename into config generation functions
    config["DummyPlaceholder"] = os.path.split(args.config)[1]
    if(args.Job=="EffectiveArea"):
        EffectiveAreaGeneration(config,args.batch)
    elif (args.Job=="Source"):
        SourceGeneration(config,args.JobType, args.batch)
    elif (args.Job=="Background"):
        BackgroundGeneration(config,args.JobType, args.batch)
    else:
        print("Invalid Job")
    print("Finished ;)")
