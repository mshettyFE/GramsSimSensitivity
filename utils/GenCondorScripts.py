import argparse
import subprocess
import math    
import os
import sys

if __name__=="__main__":
    # Parse command line
    parser = argparse.ArgumentParser(
                    prog='GenCondorScripts.py',
                    description='Generates basic .cmd and .sh files to send to condor on the nevis cluster',
                    epilog='Text at the bottom of help')
    parser.add_argument("output_path", help="Absolute path to output directory. Folder will be created if not present.")
    parser.add_argument("-nb","--n_batches", default=1000, type=int, help="Number of batches to be generated")
    parser.add_argument("-ne","--n_events", default=1000000, type=int, help="Total number of events across all batches to be generated")
    parser.add_argument("-Pn","--Process_name",default="REPLACE_ME", help="Name assigned .cmd and .sh files, and the default output of condor. Defaults to REPLACE_ME. Please give your files a better name.")
    args = parser.parse_args()
    # Calculate the number of events per batch, and modify total number of events if initially events_per_batch is not an even number
    events_per_batch = math.floor(args.n_events/args.n_batches)
    if(events_per_batch*args.n_batches != args.n_events):
        events_per_batch += 1
        args.n_events = events_per_batch*args.n_batches
        print("Increasing n_events to be evenly divisible by n_batches. n_events now is "+str(args.n_events))
    shell_script_name = args.Process_name+".sh"
    cmd_script_name = args.Process_name+".cmd"
    macrofile_name = args.Process_name+".mac"
    base = os.getcwd()
    shell_path = os.path.join(base,shell_script_name)
    tar_path = os.path.join(base,"SenseJob.tar.gz")
    macrofile_path = os.path.join(base,"SenseJob","mac",macrofile_name)
    condor_output_name = args.Process_name+"_$(Process).out"
    condor_error_name = args.Process_name+"_$(Process).err"
    condor_log_name = args.Process_name+"_$(Process).log"
    ## Check if directory exists. If it does not, create the folder
    if not (os.path.exists(args.output_path)):
        try:
            os.mkdir(args.output_path)
        except:
            print("Invalid output path")
            sys.exit()
    ## Write the beginning of the shell script
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
        f.write("tar -xzf SenseJob.tar.gz\n")
        f.write("cd SenseJob\n")
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
#    subprocess.run(["tar", "-czf", "SenseJob.tar.gz", "SenseJob/"]) 
