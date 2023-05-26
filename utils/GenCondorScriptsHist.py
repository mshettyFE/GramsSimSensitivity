import argparse
import os
import sys

if __name__=="__main__":
    # Parse command line
    parser = argparse.ArgumentParser(
                    prog='GenCondorScriptsHist.py',
                    description='Generates basic .cmd and .sh files to send to condor on the nevis cluster when reconstructing cones')
    parser.add_argument("Input_Path", help = "Absolute path to location of Reconstruction data.")
    parser.add_argument("Input_Path_Rootname", help = "Base name of each root file in Input_Path. For instance: filename= Area_0.root => basename= Area_")
    parser.add_argument("output_path", help="Absolute path to output directory. Folder will be created if not present.")
    parser.add_argument("Process_name", help="Name assigned to .cmd and .sh files, the default output of condor, and output histogram file.")
    parser.add_argument("NBatches", type=int, help="Number of reconstruction files under consideration")
    parser.add_argument("-w","--weighted", action='store_true', help="Flag to turn on reweighing scheme. If on, need to provide the total number of events, total exposure time, as well asRefFlux,PhysicalFlux, and EffectiveArea root files")
    parser.add_argument("--RefFlux", default="", help="Root file containing 1/E reference flux (Log uniform)")
    parser.add_argument("--PhysicalFlux", default="", help="Root file containing physical flux of gamma ray source")
    parser.add_argument("--EffectiveArea", default="", help="Effective Area root file")
    parser.add_argument("--TotalEvents", default=-1, type=int, help="Total number of events generated across the entire condor job")
    parser.add_argument("--ExposureTime", default=-1.0,type=float, help="Total exposure time in seconds")
    parser.add_argument("-mem","--MemoryRequested", default=300, type=int, help="Amount of memory in MB requested per job")
    parser.add_argument("-dsk","--DiskUsageRequested", default=3000000, type=int, help="Amount of disk usage requested in kB per job")
    args = parser.parse_args()
    # Creates the files names and paths necessary to run batch job
    shell_script_name = args.Process_name+".sh"
    cmd_script_name = args.Process_name+".cmd"
    condor_output_name = args.Process_name+"_$(Process).out"
    condor_error_name = args.Process_name+"_$(Process).err"
    condor_log_name = args.Process_name+"_$(Process).log"
    # Create absolute path to GenSkyMap for histograms, shell script path, Reconstructed data location (ie. input), and output file name
    home = os.getcwd()
    shell_path = os.path.join(home,shell_script_name)            
    GenSkyMap_path = os.path.join(home,"GenSkyMap")
    ReconstructedDataFileName = args.Input_Path_Rootname+"$(Process).root"
    ReconstructionData_path = os.path.join(args.Input_Path,ReconstructedDataFileName)
    OutputFileName = args.Process_name+"_SkyMap_${process}.root"
    ## Check if directory exists. If it does not, create the folder
    if not (os.path.exists(args.output_path)):
        try:
            os.mkdir(args.output_path)
        except:
            print("Couldn't create output path")
            sys.exit()
  ## Check that required root files are present in current directory 
  ## Also check if Total Events and Exposure time are valid
    if(args.weighted):
        if (not os.path.exists(args.RefFlux)):
            print("Reference Flux file not present")
            sys.exit()
        else:
            RefFluxPath = os.path.join(home,args.RefFlux)
        if (not os.path.exists(args.PhysicalFlux)):
            print("Physical Flux file not present")
            sys.exit()
        else:
            PhysFluxPath = os.path.join(home,args.PhysicalFlux)
        if (not os.path.exists(args.EffectiveArea)):
            print("Effective Area file not present")
            sys.exit()
        else:
            EffAreaPath = os.path.join(home,args.EffectiveArea)
        if (args.TotalEvents <=0):
            print("Number of events needs to be larger than zero")
            sys.exit()
        if (args.ExposureTime <=0):
            print("Exposure time needs to be larger than zero")
    ## Write the shell script
    with open(shell_script_name,'w') as f:
#./GenSkyMap -i ~/RiversideData/Background/Background_Reco_0.root --TotalEvents 200000000 -o SkyMapWeighted.root -w
        f.write("#!/bin/bash\n")
        f.write("process=$1\n")
        f.write("export PATH=/sbin:/usr/sbin:/bin:/usr/bin\n")
        f.write("source /usr/nevis/adm/nevis-init.sh\n")
        f.write("module load root\n")
        f.write("mkdir temp\n")
        f.write("cd temp\n")
        if(args.weighted):
            f.write("cp ../"+ args.EffectiveArea+" .")
            f.write("cp ../"+ args.RefFlux+" .")
            f.write("cp ../"+ args.PhysFluxPath+" .")
            f.write("./GenSkyMap -i "+ReconstructedDataFileName+" -o "+OutputFileName)
            f.write("-t "+str(args.ExposureTime)+"--TotalEvents" +str(args.TotalEvents))
            f.write(" --EffAreaFile"+args.EffectiveArea+"--PhysicalFluxFile"+args.PhysicalFlux+"--ReferenceFluxFile"+args.ReferenceFluxFile+"\n")
        else:
            f.write("./GenSkyMap -i "+ReconstructedDataFileName+" -o "+OutputFileName+"\n")
        f.write("cp "+OutputFileName+" ..\n")
        f.write("cd ..\n")
    ## Write the cmd file
    with open(cmd_script_name,'w') as f:
        f.write("executable = "+shell_path +"\n")
        f.write("arguments = $(Process)\n")
        if(args.weighted):
            f.write("transfer_input_files = "+GenSkyMap_path+","+ReconstructionData_path+","+EffAreaPath+","+PhysFluxPath+","+RefFluxPath+"\n")
        else:
            f.write("transfer_input_files = "+GenSkyMap_path+","+ReconstructionData_path+"\n")
        f.write("initialdir = "+args.output_path+"\n")
        f.write("universe       = vanilla\n")
        f.write("should_transfer_files = YES\n")
        f.write("when_to_transfer_output = ON_EXIT\n")
        f.write("""requirements = ( Arch == "X86_64" )\n""")
        f.write("request_memory = "+str(args.MemoryRequested) +"\n")
        f.write("request_disk = "+ str(args.DiskUsageRequested) +"\n")
        f.write("output = "+condor_output_name+"\n")
        f.write("error = "+condor_error_name+"\n")
        f.write("log = "+condor_log_name+"\n")
        f.write("notification   = Never\n")
        f.write("queue "+str(args.NBatches)+"\n")