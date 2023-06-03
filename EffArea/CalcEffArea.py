# Program to Aggregate results of condor job to calculate effective area at various energies

import argparse
import os,sys,math
import array
import ROOT

if __name__=="__main__":
# Read in command line arguments
    parser = argparse.ArgumentParser(prog=sys.argv[0], description='Concatenate effective area results')
# start_folder states where the Effective area root files from a condor batch job are stored at
    parser.add_argument("start_folder", help="Folder where effective area calculations are stored")
# base_name if the formatting of the file. Each root files has some preappended string, follwed by an id number and .root
# For this program, these root files are the output of ./Reconstruct, which produces the Compton Cones
    parser.add_argument("base_name",help="base_name of root files in folder. Ex. Ext_0.root as a base_name of Ext_")
# The name of the output file. I gave the option of writing to txt (if you want to plot with python easier) or root (if are going to use the files in other parts of the simulation)
# In hindsight, the .txt is probably unnecessary
    parser.add_argument("Output", help="Outputs EffArea data in specified format. Available formats are either .txt or .root.")
# TotalEvents is the number of events assigned to each condor job. So if you ran 1000 jobs with a total of 2000000 gamma rays, this number should be 2000 
# Should be consistent with you .sh script
    parser.add_argument("-n","--TotalEvents",type=int,default=10000,help="Number of events assigned to each batch file")
# Total batches dictates how many condor job instances you ran
    parser.add_argument("-b", "--TotalBatches", type=int,default=10000,help="Number of batches")
# Sphere radius is the radius of the sphere used for isotropic generation.
    parser.add_argument("--sphere_rad", default=300,type=float,help="Radius of generating sphere. Default 300")
# Currently, for .txt output, defaults to .tsv. If you want something else, change this
    parser.add_argument("--seperator", default='\t',help="Seperator of .txt file output")
# Outputs more information if needed. Useful for debugging
    parser.add_argument("-v","--verbose",action="store_true")
    args = parser.parse_args()
# Validate that the output file is either .txt or .root
    mapping = []
    if not (args.Output.endswith(".txt") or args.Output.endswith(".root")):
        print("Invalid output format")
        sys.exit()
    for batch in range(args.TotalBatches):
# Checks if the folder ends with a '/' and removes if needed
        if(args.start_folder.endswith('/')):
            args.start_folder = args.start_folder[:-1]
# Create path to root file and test if it exists. exit if it doesn't
        path = os.path.join(args.start_folder,args.base_name+str(batch)+".root")
        try:
            file = ROOT.TFile(path,"READ")
            tree = file.Get("Cones")
        except:
            print("Problem with opening ROOT file:", path)
            sys.exit()
# Get the current number of entries on 
        counts = tree.GetEntries()
# I can't divine the energy of the incident gamma rays if the root file is. So we skip these ones
# This does open up the possibility that some energy band has a very low cross section of Compton scattering
# This would mean that it might be likely that the effective area does in fact become zero
# This is somewhat of a problem at around 10 MeV, although if you generate around 20,000 or some 10 MeV gammas, you're good
# You can just generate more points if you get too many unfilled bins, so I wouldn't worry about this too much
        if(counts==0):
            print("Root file empty. Skipping energy")
            continue
        for entry in tree:
# Grab the energy from the first entry in the TChain. The assumption is that this energy is the same for all the data
# This assumes that you set up gramssky correctly
# In any case, once we have the energy, we break out and continue on
            TruthEnergy = entry.TruthEnergy
            break
# Effective area is (# of recorded photons)/(total number of photons)*(area of generating surface (ie. a sphere of radius sphere_rad))
# Each cone has a 1 to 1 correspondence with a incident gamma
        EffA = 4*math.pi*args.sphere_rad*args.sphere_rad*(counts/(args.TotalEvents))
# Append to list for writing later
        mapping.append( (TruthEnergy,EffA) )
    if(args.Output.endswith(".txt")):
# Save as raw tab seperated file (.tsv)
        with open(args.Output,'w') as f:
            f.write("Energy"+str(args.seperator)+"EffArea\n")
            for item in mapping:
                line = str(item[0])+str(args.seperator)+str(item[1])+'\n'
                if args.verbose:
                    print(line)
                f.write(line)
    if(args.Output.endswith(".root")):
        root_file= ROOT.TFile(args.Output, "RECREATE")
# Generate the binnings of the root file
        binnings=[]
        for item in mapping:
            binnings.append(item[0])
# Needed to add an additional bin at the end for ROOT to stop screaming at me
        binnings.append(binnings[-1])
# case binnings to array of double so that ROOT doesn't scream at me
        EffAreaHist = ROOT.TH1D("EffArea", "Effective Area", len(mapping),  array.array('d',binnings))
# for each energy, set the bin contents to be whatever is in the associated position in the python list
        for bin in range(0,EffAreaHist.GetNbinsX()):
            EffAreaHist.SetBinContent(bin,mapping[bin][1])
            if args.verbose:
                print(binnings[bin],EffAreaHist.GetBinContent(bin))
        root_file.WriteObject(EffAreaHist,"EffArea")
