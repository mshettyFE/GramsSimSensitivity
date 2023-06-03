// standard libraries
#include <iostream>
#include <filesystem>
#include <chrono>

// ROOT libraries
#include "TChain.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"

// util libraries
#include "Options.h"
#include "CalcSenseUtils.h"
#include "GenBackCountsUtils.h"

int main(int argc, char** argv){
// Parser logic adapted from https://github.com/wgseligman/GramsSim/tree/master/util

    auto options = util::Options::GetInstance();
    auto result = options->ParseOptions(argc, argv, "Sensitivity");

    // Abort if we couldn't parse the job options.
    if (!(result)){
        std::cerr << "ABORT: File " << __FILE__ << " Line " << __LINE__ << " " 
                << std::endl 
                << "Aborting job due to failure to parse options"
                << std::endl;
        return -1;
    }

    // Check for help and verbose flag
  bool help;
  options->GetOption("help",help);
  if (help) {
    options->PrintHelp();
      return 0;
  }

  bool verbose;
  options->GetOption("verbose",verbose);

  // Create basename of background histograms
  std::string BackgroundPath;
  std::string BackgroundBase;
  std::string AbsBackRootName;
  bool BackPathCheck = options->GetOption("BackgroundCountFolder", BackgroundPath);
  bool BackBaseCheck  = options->GetOption("BackgroundBaseName", BackgroundBase);
  if(BackBaseCheck && BackBaseCheck){
    // Sanity check that this file format works (doesn't mean it's the correct file but that is a pebkac)
    AbsBackRootName = BackgroundPath+"/"+BackgroundBase;
    // Check if the 0.root file exists
    if(!std::filesystem::exists(AbsBackRootName+"0.root")){
      std::cerr << AbsBackRootName+"0.root" << " doesn't exist" << std::endl;
      return -1;
    }
  }
  else{
    std::cerr << "Invalid background folder or background basename" << std::endl;
    return -1;
  }

  // Create basename of source cones
  std::string SourcePath;
  std::string SourceBase;
  std::string AbsSourceRootName;
  bool SourcePathCheck = options->GetOption("SourceConesFolder", SourcePath);
  bool SourceBaseCheck  = options->GetOption("SourceConesBaseName", SourceBase);
  if(SourceBaseCheck && SourceBaseCheck){
    // Sanity check that file exists (doesn't mean it's the correct file but that is a pebkac)
    AbsSourceRootName = SourcePath+"/"+SourceBase;
    if(!std::filesystem::exists(AbsSourceRootName+"0.root")){
      std::cerr << AbsSourceRootName+"0.root" << " doesn't exist" << std::endl;
      return -1;
    }
  }
  else{
    std::cerr << "Invalid Source folder or Source basename" << std::endl;
    return -1;
  }

  // Read in NPts to generate per Compton cone
    int NPts;
    bool successN = options->GetOption("NPts",NPts);
    if (!(successN)){
      std::cerr << "Invalid Number of Points per cone" << std::endl;
      return -1;
    }
    else{
      if( (NPts <= 0)){
        std::cerr << "Invalid Number of points per cone" << std::endl;
        return -1;
      }
    }

    // Determine number of events per batch job of source.
    // This is a workaround consisting of parsing a string and fact checking that it can be a positive long
    // I really didn't want to try and template Options.h
    std::string TempNEvents;
    long SourceEventsPerJob;
    bool successNEvents = options->GetOption("SourceEventsPerJob",TempNEvents);
    if (!(successNEvents)){
      std::cerr << "Invalid Number of total events" << std::endl;
      return -1;
    }
    else{
        // try catch to see if we can convert to long
      try{
        SourceEventsPerJob = std::stol(TempNEvents);
      }
      catch(...){
        std::cerr << "Can't convert SourceEventsPerJob  to long "  << std::endl;
        return -1;
      }
    // We need SourceEventsPerJob to be strictly positive
      if(SourceEventsPerJob<=0){
        std::cerr << "Number of total events needs be positive and non-zero" << std::endl;
        return -1;
      }
    }

    // Exposure time is strictly positive
    double ExposureTime;
    bool CheckExposure = options->GetOption("ExposureTime",ExposureTime);
    if(CheckExposure){
        if(ExposureTime<=0){
            std::cerr << "Invalid exposure time. needs to be strictly positive. Units of seconds" << std::endl;
            return -1;
        }
    }
    else{
        std::cerr << "Invalid exposure time. needs to be strictly positive" << std::endl;
        return -1;
    }

    // Energy of source is strictly positive
    double SourceEnergy;
    bool CheckSourceEnergy = options->GetOption("SourceEnergy",SourceEnergy);
    if(CheckSourceEnergy){
        if(SourceEnergy<=0){
            std::cerr << "Invalid Source Energy. needs to be strictly positive. Units MeV" << std::endl;
            return -1;
        }
    }
    else{
        std::cerr << "Invalid Source Energy. needs to be strictly positive" << std::endl;
        return -1;
    }

  // Open up Effective Area histogram
  std::string EffectiveAreaPath;
  bool EffAPathCheck = options->GetOption("EffectiveAreaRoot", EffectiveAreaPath);
  if(EffAPathCheck){
    if(std::filesystem::exists(EffectiveAreaPath)){
      try{
        // Make sure that we can open up file and read contents
        std::unique_ptr<TFile> EffAFile(TFile::Open(EffectiveAreaPath.c_str(), "READ"));
        TH2D* EffA= (TH2D*)EffAFile->Get("EffArea");
        if(EffA==NULL){
          throw 1;
        }
      }
      catch(...){
        std::cerr << "Couldn't open Effective Area histogram "  << std::endl;
        return -1;
      }
    }
    else{
      std::cerr << "Invalid Effective Area Path" << std::endl;
      return -1;
    }
  }
  else{
    std::cerr << "Invalid Effective Area Path" << std::endl;
    return -1;
  }

  // Read in total number of batches
  int batches;
  bool CheckBatch = options->GetOption("Batches",batches);
  if(CheckBatch){
    if(batches < 0){
      std::cerr << "Batches need to be a strictly positive number" << std::endl;
      return -1;
    }
  }
  else{
    std::cerr << "Invalid number of batches" << std::endl;
    return -1;
  }

    std::cout << "Reading in Background..." << std::endl;

  // Load in skeleton of BackgroundCounts from the first background count histogram
  TH1D* AggCounts;
  std::string seed = AbsBackRootName+"0.root";

  std::unique_ptr<TFile> Seed(TFile::Open(seed.c_str(), "READ"));
  AggCounts = (TH1D*)Seed->Get("Counts");
  if(AggCounts==NULL){
    std::cerr << "Couldn't read Counts histogram in " << seed << std::endl;
    return -1;
  }
  // Create histogram. We now have the correct binnings.
  AggCounts->Reset();

  // Get bin number of region of background we are comparing against
  int TargetBin = ExtractBinNum(SourceEnergy, AggCounts);

  // Aggregate all the sky maps that exist
  bool CheckAgg = ReadBackgroundCounts(AbsBackRootName,batches,AggCounts,verbose);

  // Calculate mean and std. dev. assuming Poisson stats
  double mu = AggCounts->GetBinContent(TargetBin);
  double sigma = AggCounts->GetBinError(TargetBin);
  // Calculates 3 and 5 sigma thresholds
  double ThreeSigmaThreshold = mu+3*sigma;
  double FiveSigmaThreshold = mu+5*sigma;

  std::cout << "Mean: " << mu << '\t' << "Sigma: " << sigma << std::endl;

  // Place holder variables
  Long64_t TotalRecordedPhotons = 0.0;
  Long64_t ThreeSigmaRecordedPhotons = 0.0;
  Long64_t FiveSigmaRecordedPhotons = 0.0;
  double TotalCounts = 0.0;
  // Flags to make sure that I only write the number of photons at each threshold only once
  bool ThreeSigmaFlag = false;
  bool FiveSigmaFlag = false;

  std::cout << "Calculating Sensitivity" << std::endl;
  for(int i=0; i< batches; ++i){
    // Try and read in source file
    std::string id = std::to_string(i);
    std::string path = AbsSourceRootName+id+".root";
    if(!std::filesystem::exists(path)){
      if(verbose){
          std::cerr << path << " could not be found " << std::endl;
      }
      continue;
    }
    // Get the number of cones that lie within the mask
    std::unique_ptr<TFile> Current(TFile::Open(path.c_str(), "READ"));
    TH1D* CurrentCounts = (TH1D*)Current->Get("Counts");
    if(CurrentCounts==NULL){
      std::cerr << "Couldn't read Counts histogram in" << path << std::endl;
      continue;
    }
    TotalCounts += CurrentCounts->GetBinContent(CurrentCounts->FindBin(SourceEnergy));
    // Keep a running tally of how many photons you need to observe TotalCounts number of cones
    TotalRecordedPhotons += SourceEventsPerJob;
    if(verbose){
      std::cout << "FileNum: "<< i  << " Counts: " << TotalCounts << " Thresholds: " << ThreeSigmaThreshold << '\t' << FiveSigmaThreshold << std::endl;
    }
    // If you hit the 3 sigma threshold, you save how many photons where needed to generate the current TotalCounts number of cones
    if((TotalCounts >= ThreeSigmaThreshold) && !(ThreeSigmaFlag)){
      ThreeSigmaFlag = true;
      ThreeSigmaRecordedPhotons = TotalRecordedPhotons;
    }
    // Ditto with 5 sigma, but now you break out of loop since 5 sigma is good enough
    if((TotalCounts >= FiveSigmaThreshold) && !(FiveSigmaFlag)){
      FiveSigmaFlag = true;
      FiveSigmaRecordedPhotons = TotalRecordedPhotons;
      break;
    }
  }

  // We assume that the number of photons is proportional to the number of cones generated
  // The proportionality constant might be different for each energy, but we only consider 1 energy at a time, so it's OK
  // as an example: NPhotonsThreeSigma/FiveSigmaRecordedPhotons = ThreeSigmaThresholdCones/TotalCones. Rearrange for NPhotonsThreeSigma
  double NPhotonsThreeSigma = (ThreeSigmaThreshold)/(TotalCounts)*((double) FiveSigmaRecordedPhotons);
  double NPhotonsFiveSigma = (FiveSigmaThreshold)/(TotalCounts)*((double) FiveSigmaRecordedPhotons);

  // Get the effective area at this energy
  std::unique_ptr<TFile> EffAFile(TFile::Open(EffectiveAreaPath.c_str(), "READ"));
  TH1D* EffA= (TH1D*) EffAFile->Get("EffArea");

  double SourceEffA = ExtractEffArea(SourceEnergy, EffA);

  // Conversion rate of MeV to ergs because astronomers still user ergs
  const double MeVtoErgs = 1.60218E-6;

  // Convert number of photons to sensitivity (units of ergs/(cm^2*s))
  // Multiply number of photons by source energy, convert to ergs, and divide by effective area nd exposure time
  double ThreeSigmaDiffSensitivity = NPhotonsThreeSigma*SourceEnergy*MeVtoErgs/(SourceEffA*ExposureTime);
  double FiveSigmaDiffSensitivity = NPhotonsFiveSigma*SourceEnergy*MeVtoErgs/(SourceEffA*ExposureTime);
  // Another way of representing sensitivity is photons/cm^2/s.
  double ThreeSigmaPhotonSensitivity = NPhotonsThreeSigma/(SourceEffA*ExposureTime);
  double FiveSigmaPhotonSensitivity = NPhotonsFiveSigma/(SourceEffA*ExposureTime);

  // Printing to console
  std::cout << "At a source energy of "<< SourceEnergy << " MeV and exposure time of " << ExposureTime << " seconds , the sensitivity is (in ergs*cm^-2*s^-1):" << std::endl;
  std::cout << "\tThree Sigma: " << ThreeSigmaDiffSensitivity << std::endl;
  std::cout << "\tFive Sigma: " << FiveSigmaDiffSensitivity << std::endl;
  std::cout << "In photons/cm^2/s:" <<std::endl;
  std::cout << "\tThree Sigma: " << ThreeSigmaPhotonSensitivity << std::endl;
  std::cout << "\tFive Sigma: " << FiveSigmaPhotonSensitivity << std::endl;

  return 0;
}
