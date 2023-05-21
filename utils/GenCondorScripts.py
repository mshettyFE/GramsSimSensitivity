import argparse
import subprocess
import math    
import os
import sys
import numpy as np

if __name__=="__main__":
    # Parse command line
    parser = argparse.ArgumentParser(
                    prog='GenCondorScripts.py',
                    description='Generates basic .cmd and .sh files to send to condor on the nevis cluster',
                    epilog='Text at the bottom of help')
    parser.add_argument("output_path", help="Absolute path to output directory. Folder will be created if not present.")
    parser.add_argument("Process_name", help="Name assigned to .cmd and .sh files, and the default output of condor.")
    parser.add_argument("Command_file", help="File containing the commands that come after the preamble in the .sh file")
    parser.add_argument("-nb","--n_batches", default=1000, type=int, help="Number of batches to be generated")
    parser.add_argument("-ne","--n_events", default=1000000, type=int, help="Total number of events across all batches to be generated")
    parser.add_argument('-m',"--macros", action="store_true", help="Denotes that SenseJob/mac/batch will be filled with a series of macro files. For when gramssky is not enough.")
    parser.add_argument("--BatchMacroName", default="REPLACE_ME_", help="Default batch macro file name. Please, give it a better name when you actually run this script")
    parser.add_argument("--minE",default = 0.1, type=float, help="Minimum energy assigned via macrofile")
    parser.add_argument("--maxE",default = 10, type=float, help="Maximum energy assigned via macrofile")
    parser.add_argument("-s","--scale", default="log", choices=['log','linear'], help="Scaling of x axis. Defaults to log")
    # A bit of a hack, since I probably shoud read this value from the .gdml file. This is easier. Just need to make sure this value is consistent with detector
    parser.add_argument("-Zc","--Zcenter", default=-40.0, type=float, help="Center of detector along z axis")
    args = parser.parse_args()
    # Calculate the number of events per batch, and modify total number of events if initially events_per_batch is not an even number
    events_per_batch = math.floor(args.n_events/args.n_batches)
    if(events_per_batch*args.n_batches != args.n_events):
        events_per_batch += 1
        args.n_events = events_per_batch*args.n_batches
        print("Increasing n_events to be evenly divisible by n_batches. n_events now is "+str(args.n_events))
    # Creates the files names and paths necessary to run batch job
    shell_script_name = args.Process_name+".sh"
    cmd_script_name = args.Process_name+".cmd"
    macrofile_name = args.Process_name+".mac"
    tar_name = "SenseJob"+args.Process_name+".tar.gz"
    condor_output_name = args.Process_name+"_$(Process).out"
    condor_error_name = args.Process_name+"_$(Process).err"
    condor_log_name = args.Process_name+"_$(Process).log"
    ## We need out current working directory for later...
    home = os.getcwd()
    shell_path = os.path.join(home,shell_script_name)
    tar_path = os.path.join(home,tar_name)
    macrofile_path = os.path.join(home,"SenseJob","mac",macrofile_name)
    # Validate batch macro parameters
    if(args.macros):
        if( (args.minE<=0) or (args.maxE<=0) or (args.maxE <= args.minE)):
            print("Energy ranges invalid")
            sys.exit() 
        if(args.BatchMacroName==""):
            print("Invalid Batch Macro Filename")
            sys.exit()
        if(args.scale=="linear"):
            energies = np.linspace(args.minE, args.maxE, num=args.n_batches)
        elif(args.scale=="log"):
            energies = np.logspace(math.log10(args.minE),math.log10(args.maxE),num=args.n_batches)
        else:
            print("Something went horribly wrong with determining the x scale of the batch energies")
            sys.exit()
        if(args.Zcenter>=0):
            print("Coordinate system of Grams assigns negative values to z axi. Hence, the center should be a negative value")
            sys.exit()
    # Gets additional commands from text file to be added to .sh file
    try:
        with open(args.Command_file,'r') as f:
            commands = f.read()
    except:
        print("Couldn't read command file")
        sys.exit()
            
    ## Check if directory exists. If it does not, create the folder
    if not (os.path.exists(args.output_path)):
        try:
            os.mkdir(args.output_path)
        except:
            print("Invalid output path")
            sys.exit()
    ## Write the shell script
    with open(shell_script_name,'w') as f:
        f.write("#!/bin/bash\n")
        f.write("process=$1\n")
        f.write("n_batches="+str(args.n_batches)+'\n')
        f.write("total_events="+str(args.n_events)+'\n')
        f.write("event_per_batch="+str(events_per_batch)+'\n')
        f.write("export PATH=/sbin:/usr/sbin:/bin:/usr/bin\n")
        f.write("export G4NEUTRONHPDATA=/usr/local/share/Geant4-10.5.1/data/G4NDL4.5\n")
        f.write("source /usr/nevis/adm/nevis-init.sh\n")
        f.write("module load cmake root geant4 hepmc3 healpix\n")
        f.write("tar -xzf "+tar_name+"\n")
        f.write("cd SenseJob\n")
        ## Write the additional commands
        f.write(commands)
    ## Write the cmd file
    with open(cmd_script_name,'w') as f:
        f.write("executable = "+shell_path +"\n")
        f.write("transfer_input_files = "+tar_path+"\n")
        f.write("arguments = $(Process)\n")
        f.write("initialdir = "+args.output_path+"\n")
        f.write("universe       = vanilla\n")
        f.write("should_transfer_files = YES\n")
        f.write("when_to_transfer_output = ON_EXIT\n")
        f.write("""requirements = ( Arch == "X86_64" )\n""")
        f.write("output = "+condor_output_name+"\n")
        f.write("error = "+condor_error_name+"\n")
        f.write("log = "+condor_log_name+"\n")
        f.write("notification   = Never\n")
        f.write("queue "+str(args.n_batches)+"\n")
  ## I'm assuming that SenseJob is in the same directory as GenCondorScripts.py, which it should be if you ran cmake
    with open(macrofile_path,'w') as f:
        f.write("/run/initialize\n")
        f.write("/run/beamOn "+str(events_per_batch)+'\n')
    if(args.macros):
        ## Clean batch directory of any prior mac files by erasing directory, and then recreating directory
        subprocess.run(["rm", "-rf", "SenseJob/mac/batch"]) 
        subprocess.run(["mkdir", "SenseJob/mac/batch"]) 
        ## CD to batch folder
        os.chdir("SenseJob/mac/batch")
        ## For each energy, we create a macro that produces isotropic, monoenergetic gamma rays aimed at the detector center.
        ## This is difficult to replicate with gramssky, hence why this is here
        for i in range(args.n_batches):
            fname = args.BatchMacroName+str(i)+".mac"
            with open(fname,'w') as f:
                f.write("/run/initialize\n")
                f.write("/gps/particle gamma\n")
                f.write("/gps/ene/type Mono\n")
                f.write("/gps/ene/mono "+str(energies[i])+" MeV\n")
                f.write("/gps/pos/type Surface\n")
                f.write("/gps/pos/shape Sphere\n")
                f.write("/gps/pos/radius 300 cm\n")
                f.write("/gps/pos/centre 0 0 "+str(args.Zcenter)+" cm\n")
                f.write("/gps/ang/type focused\n")
                f.write("/gps/ang/focuspoint 0 0 "+str(args.Zcenter)+" cm\n")
                f.write("/run/beamOn "+str(args.n_batches)+"\n")
    ## Return home so that we can create a tarball out of SenseJob
    os.chdir(home)
    subprocess.run(["tar", "-czf", tar_name, "SenseJob/"]) 