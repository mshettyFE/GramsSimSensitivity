# Program to spin up condor files for calculating counts and sky maps within a neighborhood of a location

import argparse
import os
import sys
import subprocess

if __name__=="__main__":
    # Parse command line
    parser = argparse.ArgumentParser(
                    prog='GenCondorScriptsHist.py',
                    description='Generates basic .cmd and .sh files to send to condor on the nevis cluster when reconstructing cones')
    # Same format as everything else: folder containing root files...
    parser.add_argument("Input_Path", help = "Absolute path to location of Reconstruction data.")
    # format of root files (ie. include everything except id and .root)
    parser.add_argument("Input_Path_Rootname", help = "Base name of each root file in Input_Path. For instance: filename= Area_Reco_0.root => basename= Area_Reco_")
    parser.add_argument("output_path", help="Absolute path to output directory. If folder is not present, will try to create it.")
    parser.add_argument("Process_name", help="Name assigned to .cmd and .sh files, the default output of condor, and output file.")
    parser.add_argument("NBatches", type=int, help="Number of reconstruction files under consideration")
    parser.add_argument("Folder", help="Folder to  be zipped up and send to cluster")
    # If you aren't reweighting, you must include an effective area file and a mask file
    parser.add_argument("EffectiveArea", default="", help="Name of Effective Area root file (not the path)")
    parser.add_argument("MaskFile", default="", help="Name of Mask root file (not the path)")
    # If you are reweighting, you also need a a reference flux, a physical flux, exposure time and total event number
    parser.add_argument("-w","--Weighted",action='store_true', help=" Dictates weather we are reweighting or not")
    parser.add_argument("--RefFlux", default="", help=" Name of Root file (not the path) containing 1/E reference flux (Log uniform)")
    parser.add_argument("--PhysicalFlux", default="", help="Name of Root file (not the path) containing physical flux of gamma ray source")
    parser.add_argument("--TotalEvents", default=-1, type=int, help="Total number of events generated across the entire condor job (NOTE: Not number of events for each job)")
    parser.add_argument("--ExposureTime", default=-1.0,type=float, help="Total exposure time in seconds")
    # These hopefully prevent condor from prematurely kicking out jobs, although I wouldn't bet on it
    parser.add_argument("-mem","--MemoryRequested", default=300, type=int, help="Amount of memory in MB requested per job")
    parser.add_argument("-dsk","--DiskUsageRequested", default=3000000, type=int, help="Amount of disk usage requested in kB per job")
    args = parser.parse_args()
    # Creates the files names and paths necessary to run batch job
    shell_script_name = args.Process_name+".sh"
    cmd_script_name = args.Process_name+".cmd"
    tar_name = args.Process_name+".tar.gz"
    # default output of condor files
    condor_output_name = args.Process_name+"_$(Process).out"
    condor_error_name = args.Process_name+"_$(Process).err"
    condor_log_name = args.Process_name+"_$(Process).log"
    # Create absolute path to GenBackCounts for histograms, shell script path, Reconstructed data location (ie. input), and output file name
    home = os.getcwd()
    shell_path = os.path.join(home,shell_script_name)
    tar_path = os.path.join(home,tar_name)
    # This file name goes in .cmd, hence $(Process). This allows condor to pull the correct input data file
    ReconstructedDataFileName = args.Input_Path_Rootname+"$(Process).root"
    # This is also the relavent data file, but this goes in the shell script, hence ${process}
    ShellReconstructedDataFileName = args.Input_Path_Rootname+"${process}.root"
    # The absolute path to input data file. condor screams at you if you don't give absolute paths when transferring files
    # See https://www.nevis.columbia.edu/~seligman/root-class/html/appendix/batch/SharedFilesystem.html for more details
    ReconstructionData_path = os.path.join(args.Input_Path,ReconstructedDataFileName)
    OutputFileName = args.Process_name+"_BackCount_${process}.root"
    ## Check if output directory exists. If it does not, try to create the folder
    if not (os.path.exists(args.output_path)):
        try:
            os.mkdir(args.output_path)
        except:
            print("Couldn't create output path")
            sys.exit()
  ## Check that required root files are present in tar folder 
  ## Also check if Total Events and Exposure time are valid
    if (args.MaskFile!=""):
        MaskPath = os.path.join(home,args.Folder,args.MaskFile)
        if (not os.path.exists(MaskPath)):
            print("Effective Area file not present in " + args.Folder)
            sys.exit()
    else:
        print("Need a mask file name")
        sys.exit()
    if (args.EffectiveArea!=""):
        EffAreaPath = os.path.join(home,args.Folder,args.EffectiveArea)
        if (not os.path.exists(EffAreaPath)):
            print("Effective Area file not present in "+args.Folder)
            sys.exit()
    else:
        print("Need an effective area file name")
        sys.exit()
    if args.Weighted:
        if((args.RefFlux=="")  or (args.PhysicalFlux=="")):
            print("Need to input Reference Flux and PhysicalFlux for reweighting")
            sys.exit()
        RefFluxPath = os.path.join(home,args.Folder,args.RefFlux)
        if (not os.path.exists(RefFluxPath)):
            print("Reference Flux file not present in "+args.Folder)
            sys.exit()
        PhysFluxPath = os.path.join(home,args.Folder,args.PhysicalFlux)
        if (not os.path.exists(PhysFluxPath)):
            print("Physical Flux file not present in "+ args.Folder)
            sys.exit()
        if (args.TotalEvents <=0):
            print("Number of events needs to be larger than zero")
            sys.exit()
        if (args.ExposureTime <=0):
            print("Exposure time needs to be larger than zero")
    ## Write the shell script
    with open(shell_script_name,'w') as f:
        # generic setup for nevis cluster
        f.write("#!/bin/bash\n")
        f.write("process=$1\n")
        f.write("export PATH=/sbin:/usr/sbin:/bin:/usr/bin\n")
        f.write("source /usr/nevis/adm/nevis-init.sh\n")
        f.write("module load root\n")
        # unzip folder
        f.write("tar -xzf "+tar_name+"\n")
        # move input file to newly unzipped folder and cd to folder
        f.write("mv "+ShellReconstructedDataFileName+" "+args.Folder+"\n")
        f.write("cd "+args.Folder+"\n")
        if(args.Weighted):
            # Run weighted calculation
            f.write("./GenCountsHists -i "+ShellReconstructedDataFileName+" -o "+OutputFileName+" -w")
            f.write(" -t "+str(args.ExposureTime)+" --TotalEvents " +str(args.TotalEvents))
            f.write(" --EffAreaFile "+args.EffectiveArea+" --PhysicalFluxFile "+args.PhysicalFlux+" --ReferenceFluxFile "+args.RefFlux+" --MaskFile "+ args.MaskFile+"\n")
        else:
            # Run weighted calculation
            f.write("./GenCountsHists -i "+ShellReconstructedDataFileName+" -o "+OutputFileName+" --MaskFile "+ args.MaskFile+" --EffAreaFile "+args.EffectiveArea+"\n")
        # move output back up 1 directory and cd up. Condor automatically transfers output
        f.write("mv "+OutputFileName+" ..\n")
        f.write("cd ..\n")
    ## Write the cmd file
    with open(cmd_script_name,'w') as f:
        # shell script name
        f.write("executable = "+shell_path +"\n")
        # Assign process number now so that we can transfer input file
        f.write("arguments = $(Process)\n")
        # transfer folder and input
        f.write("transfer_input_files = "+tar_path+" , "+ReconstructionData_path+"\n")
        # directory to write out to
        f.write("initialdir = "+args.output_path+"\n")
        # generic condor stuff
        f.write("universe       = vanilla\n")
        f.write("should_transfer_files = YES\n")
        f.write("when_to_transfer_output = ON_EXIT\n")
        f.write("""requirements = ( Arch == "X86_64" )\n""")
        f.write("request_memory = "+str(args.MemoryRequested) +"\n")
        f.write("request_disk = "+ str(args.DiskUsageRequested) +"\n")
        f.write("output = "+condor_output_name+"\n")
        f.write("error = "+condor_error_name+"\n")
        f.write("log = "+condor_log_name+"\n")
        # So that if your jobs fails because of bad inputs, your email doesn't get filled with 10,000 emails
        f.write("notification   = Never\n")
        # from NBatches number of jobs
        f.write("queue "+str(args.NBatches)+"\n")
    # zip up folder
    subprocess.run(["tar", "-czf", tar_name, args.Folder+"/"]) 
