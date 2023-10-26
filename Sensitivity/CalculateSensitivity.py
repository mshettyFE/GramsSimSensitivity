import tomllib
import argparse
import sys
import os

import ROOT
ROOT.gROOT.SetBatch(True)

def ExtractBinNum(SourceEnergy, Histogram):
## Function that extracts bin number of a particular energy, with error checks at boundaries
    nbins = Histogram.GetNbinsX()
    XAxis = Histogram.GetXaxis()
## See if SourceEnergy is higher than upper bound
    upper_bound = XAxis.GetBinCenter(nbins)
    if(SourceEnergy > upper_bound):
        print("Source energy to high, defaulting to highest bin")
        return nbins-1
## Same for lower bound
    lower_bound = XAxis.GetBinLowEdge(1)
    if(SourceEnergy<= lower_bound):
        print("Source energy too low. Defaulting to lowest bin...")
    return Histogram.FindBin(SourceEnergy)

def ReadBackgroundCounts(BackgroundFileLocation ,  BackgroundSkyMapBase, nbatches):
## Aggregate all background files counts into one histogram
## Skips files if they are not present
    first_file = os.path.join(BackgroundFileLocation,BackgroundSkyMapBase+"_0.root")
    first_file_temp = ROOT.TFile.Open(first_file,"READ")
    AggCounts = first_file_temp.Get("Counts")
    AggCounts.Reset()
    for i in range(nbatches):
        file_path = os.path.join(BackgroundFileLocation,BackgroundSkyMapBase+"_"+str(i)+".root")
        if not os.path.exists(file_path):
            print(file_path+" doesn't exists. Skipping...")
            continue
        Current = ROOT.TFile.Open(file_path,"READ")
        AddonHist = Current.Get("Counts")
        if not AddonHist:
            print("Couldn't read Counts histogram in "+ file_path+" . Skipping...")
            continue
        AggCounts.Add(AddonHist)
    Output = AggCounts.Clone()
    Output.SetDirectory(0)
    return Output

if __name__=="__main__":
## MevtoERgs conversion
    MeVtoErgs = 1.60218E-6
    parser = argparse.ArgumentParser(
                        prog='GenCondorJobs',
                        description='Generates .sh and .cmd files for condor jobs to send to Nevis cluster',
                        epilog='NOTE: Effective Area root file must be in same directory as '+sys.argv[0])
    parser.add_argument("config",help="Path to .ini config file")
## Parse toml file
    args = parser.parse_args()
    try:
        with open(args.config,'rb') as f:
            config = tomllib.load(f)
    except:
        print("Couldn't read"+ args.config)
        sys.exit()

## Read in and validate input parameters
    SourceEnergy = float(config["Source"]["SourceEnergy"])
    if(SourceEnergy <=0):
        print("Source energy must be positive")
        sys.exit()

    SourceEventsPerJob = int(config["Source"]["SourceEventsPerJob"])
    if(SourceEventsPerJob <=0):
        print("Number of jobs per batch must be is positive")
        sys.exit()

    SourceBatches = int(config["Source"]["SourceBatches"])
    if(SourceBatches <=0):
        print("Number of source batches must be is positive")
        sys.exit()

    BackBatches = int(config["Background"]["BackgroundBatches"])
    if(BackBatches <=0):
        print("Number of background batches must be is positive")
        sys.exit()

    ExposureTime = float(config["Background"]["SkyMap"]["ExposureTime"])
    if(ExposureTime <=0):
        print("Background exposure time must be is positive")
        sys.exit()

    Scaling = float(config["Sensitivity"]["TimeScaling"])
    if(Scaling <=0):
        print("Time scaling must be is positive")
        sys.exit()
## EffArea file reading
    print("Reading in Effective Area...")
    EffAreaRoot = config["CalcEffArea"]["OutputFileName"]+".root"
    try:
        EffAreaF = ROOT.TFile.Open(EffAreaRoot,"READ")
        EffArea = EffAreaF.Get("EffArea")
    except:
        print("Couldn't read Effective Area file")
        sys.exit()
## Background file reading
    print("Reading in Background...")
    BackgroundFileLocation = os.path.join(config["General"]["output_directory"], "Background", "SkyMap")
    BackgroundSkyMapBase = config["Background"]["SkyMap"]["SkyMapOutput"]
    FirstBack = os.path.join(BackgroundFileLocation,BackgroundSkyMapBase+"_0.root")
#    try:
    FirstBackRootF = ROOT.TFile.Open(FirstBack,"READ")
    FirstBackRoot = FirstBackRootF.Get("Counts")
#    except:
#        print("Couldn't read "+ FirstBack)
#        sys.exit()
## Extract background counts and calculate 3 sigma thresholds
    bin_num = ExtractBinNum(SourceEnergy,FirstBackRoot)
    AggCounts = ReadBackgroundCounts(BackgroundFileLocation ,  BackgroundSkyMapBase, BackBatches)
    mu = AggCounts.GetBinContent(bin_num)
    sigma = AggCounts.GetBinError(bin_num)
    ThreeSigmaThreshold = mu+3*sigma
    FiveSigmaThreshold = mu+5*sigma
    print("Mean: "+ str(mu)+ "Sigma: "+ str(sigma)+"\n")
# Read in source files to determine sensitivity
    SourceFileLocation = os.path.join(config["General"]["output_directory"], "Source", "SkyMap")
    SourceSkyMapBase = config["Source"]["SkyMap"]["SkyMapOutput"]
    TotalRecordedPhotons = 0
    ThreeSigmaRecordedPhotons = 0
    FiveSigmaRecordedPhotons = 0
    TotalCounts = 0.0
    ThreeSigmaFlag = False
    FiveSigmaFlag = False
    print("Calculating Sensitivity...")
    for i in range(SourceBatches):
# Skip Source files if they don't exist
        source_file_path = os.path.join(SourceFileLocation, SourceSkyMapBase+"_"+str(i)+".root")
        if not os.path.exists(source_file_path):
            print(source_file_path+" doesn't exists. Skipping...")
            continue
# Add to number of observed counts in each file to counter, and keep track of when sigma tresholds are met
        Current = ROOT.TFile.Open(source_file_path,"READ")
        CurrentCounts = Current.Get("Counts")
        TotalCounts += CurrentCounts.GetBinContent(CurrentCounts.FindBin(SourceEnergy))
        TotalRecordedPhotons += SourceEventsPerJob
        if ((TotalCounts > ThreeSigmaThreshold) and  not (ThreeSigmaFlag)):
            ThreeSigmaFlag = True
            ThreeSigmaRecordedPhotons = TotalRecordedPhotons
        if ((TotalCounts > FiveSigmaThreshold) and  not (FiveSigmaFlag)):
            FiveSigmaFlag = True
            FiveSigmaRecordedPhotons = TotalRecordedPhotons
            break
# Scaling time and effective area at source energy
    EffectiveTime = ExposureTime*Scaling
    SourceEffArea = EffArea.GetBinContent(ExtractBinNum(SourceEnergy,EffArea))
    Denom = EffectiveTime*SourceEffArea
# Calculate Number of photons needed to hit a sensitivity threshold
    NPhotonsThreeSigma = ThreeSigmaThreshold/TotalCounts*ThreeSigmaRecordedPhotons
    NPhotonsFiveSigma = FiveSigmaThreshold/TotalCounts*FiveSigmaRecordedPhotons
# Differential sensitivity in units of (units of ergs/(cm^2*s))
    ThreeSigmaDiffSensitivity = NPhotonsThreeSigma*SourceEnergy*MeVtoErgs/(Denom)
    FiveSigmaDiffSensitivity = NPhotonsFiveSigma*SourceEnergy*MeVtoErgs/(Denom)
# Differential Sensitivity in units of photons/cm^2/s
    ThreeSigmaPhotonSensitivity = NPhotonsThreeSigma/(Denom)
    FiveSigmaPhotonSensitivity = NPhotonsFiveSigma/(Denom)
    print("At a source energy of " + str(SourceEnergy) + " MeV and exposure time of " + str(EffectiveTime) + " seconds , the sensitivity is (in ergs*cm^-2*s^-1):")
    print("\tThree Sigma: "+str(ThreeSigmaDiffSensitivity))
    print("\tFive Sigma: "+str(FiveSigmaDiffSensitivity))
    print("In photons/cm^2/s:")
    print("\tThree Sigma: "+ str(ThreeSigmaPhotonSensitivity))
    print("\tFive Sigma: "+ str(FiveSigmaPhotonSensitivity))
