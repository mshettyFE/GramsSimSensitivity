# Program to Aggregate results of condor job to calculate effective area at various energies

import tomllib
import os,sys,math
import array
import ROOT
import argparse

if __name__=="__main__":
    parser = argparse.ArgumentParser(
                        prog='GenCondorJobs',
                        description='Generates .sh and .cmd files for condor jobs to send to Nevis cluster')
    parser.add_argument("config",help="Path to .tomi config file")
    args = parser.parse_args()

    try:
        with open(args.config,'rb') as f:
            config = tomllib.load(f)
    except:
        print("Couldn't read"+ args.config)
        sys.exit()
    start_folder = os.path.join(config["General"]["output_directory"],"EffectiveArea","Cones")
    if not (os.path.exists(start_folder)):
        print(start_folder+"Doesn't exist")
        sys.exit()
    base_name = config["EffectiveArea"]["Reconstruct"]["ReconstructOutput"]
    TotalEvents = config["EffectiveArea"]["gramssky"]["nparticles"]
    TotalBatches  = config["EffectiveArea"]["gramssky"]["energy_bins"]
    disk_rad = config["EffectiveArea"]["gramssky"]["RadiusDisc"]
    Output = config["CalcEffArea"]["OutputFileName"]+".root"

    mapping = []
    for batch in range(TotalBatches):
# Checks if the folder ends with a '/' and removes if needed
        if(start_folder.endswith('/')):
            start_folder = start_folder[:-1]
# Create path to root file and test if it exists. keep going if it does not
        path = os.path.join(start_folder,base_name+"_"+str(batch)+".root")
        try:
            file = ROOT.TFile(path,"READ")
            tree = file.Get("Cones")
        except:
            print("Problem with opening ROOT file:", path)
            continue
# Get the current number of entries on 
        counts = tree.GetEntries()
# I can't divine the energy of the incident gamma rays if the root file is non existant. So we skip these ones
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
# Effective area is (# of recorded photons)/(total number of photons)*(area of generating surface (ie. a tangent disk of radius disk_rad))
# Each cone has a 1 to 1 correspondence with a incident gamma
        EffA = math.pi*disk_rad*disk_rad*(counts/(TotalEvents))
# Append to list for writing later
        mapping.append( (TruthEnergy,EffA) )
    root_file= ROOT.TFile(Output, "RECREATE")

# Generate the binnings of the root file
    binnings=[]
    for item in mapping:
        binnings.append(item[0])
# Needed to add an additional bin at the end for ROOT to stop screaming at me
    binnings.append(binnings[-1])
# cast binnings to array of double so that ROOT doesn't scream at me
    EffAreaHist = ROOT.TH1D("EffArea", "Effective Area", len(mapping),  array.array('d',binnings))
# for each energy, set the bin contents to be whatever is in the associated position in the python list
    for bin in range(0,EffAreaHist.GetNbinsX()):
        EffAreaHist.SetBinContent(bin,mapping[bin][1])
    root_file.WriteObject(EffAreaHist,"EffArea")
