#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "TChain.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"

#include "Options.h"
#include "CalcSenseUtils.h"

/*
## Plan: Read in all background histograms as TChain into a single summed background
## Apply Mask to background.
## Convert 2d skymap to 1D std::vector consisting of only non-zero values
## Extract mean and std. dev from std::vector. Get 3 sigma 5 sigma threshold
## Read in all the reconstructed cones of the Source into another TChain
## Iteratively reconstruct Source cones to histograms.
## Iteratively apply mask onto Source histograms. If you are non-zero, count the cone
## Continue until you get enough cones to hit 3 and 5 sigma thresholds
## Once you hit 5 sigma threshold, break out
## Convert number of cones needed to number of photons needed
## Convert number of photons needed to sensitivity by dividing by photon energy, effective area at that energy, and exposure time
*/

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
        exit(EXIT_FAILURE);
        return -1;
    }

    // Check for help and verbose flag
  bool help;
  options->GetOption("help",help);
  if (help) {
    options->PrintHelp();
    exit(EXIT_SUCCESS);
      return 0;
  }

  bool verbose;
  options->GetOption("verbose",verbose);

  // Create basename of background histograms
  std::string BackgroundPath;
  std::string BackgroundBase;
  std::string AbsBackRootName;
  bool BackPathCheck = options->GetOption("BackgroundHistFolder", BackgroundPath);
  bool BackBaseCheck  = options->GetOption("BackgroundHistBaseName", BackgroundBase);
  if(BackBaseCheck && BackBaseCheck){
    if((BackgroundPath.length()>0) || (BackgroundBase.length()>0)){
      AbsBackRootName = BackgroundPath+"/"+BackgroundBase;
    }
    else{
    std::cerr << "Empty background path or base name" << std::endl;
    return -1;
    }
  }
  else{
    std::cerr << "Invalid background path or background basename" << std::endl;
    return -1;
  }

  // Create basename of source cones
  std::string SourcePath;
  std::string SourceBase;
  std::string AbsSourceRootName;
  bool SourcePathCheck = options->GetOption("SourceConesFolder", SourcePath);
  bool SourceBaseCheck  = options->GetOption("SourceConesBaseName", SourceBase);
  if(SourceBaseCheck && SourceBaseCheck){
    if((SourcePath.length()>0) || (SourceBase.length()>0)){
      AbsSourceRootName = SourcePath+"/"+SourceBase;
    }
    else{
    std::cerr << "Empty Source path or base name" << std::endl;
    return -1;
    }
  }
  else{
    std::cerr << "Invalid Source path or Source basename" << std::endl;
    return -1;
  }

  // Read in distribution type. This determines 3 and 5 sigma thresholds
  std::string Distribution;
  std::string Gauss = "Gauss";
  std::string Poisson = "Poisson";
  bool DistCheck = options->GetOption("Distribution",Distribution);
  if(DistCheck){
    if((Distribution!=Gauss) && (Distribution!=Poisson)){
        std::cerr << "Invalid Distribution Name" << std::endl;
        return -1;
    }
  }
  else{
    std::cerr << "Invalid Distribution Name" << std::endl;
    return -1;
  }

    // Determine number of events. This is a workaround of parsing a string and fact checking that it can be a positive long
    // I really didn't want to try and template Options.h
    std::string TempNEvents;
    long NEvents;
    bool successNEvents = options->GetOption("TotalSourceEvents",TempNEvents);
    if (!(successNEvents)){
      std::cerr << "Invalid Number of total events" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }
    else{
        // try catch to see if we can convert to long
      try{
        NEvents = std::stol(TempNEvents);
      }
      catch(...){
        std::cerr << "Can't convert NEvents to long "  << std::endl;
        return -1;
      }
    // We need NEvents to be strictly positive
      if(NEvents<=0){
        std::cerr << "Number of total events needs be positive and non-zero" << std::endl;
        exit(EXIT_FAILURE);
        return -1;
      }
    }

    // Exposure time is strictly positive
    double ExposureTime;
    bool CheckExposure = options->GetOption("ExposureTime",ExposureTime);
    if(CheckExposure){
        if(ExposureTime<=0){
            std::cerr << "Invalid exposure time. needs to be strictly positive" << std::endl;
            return -1;
        }
    }
    else{
        std::cerr << "Invalid exposure time. needs to be strictly positive" << std::endl;
        return -1;
    }

  // Open up Mask histogram
  std::string MaskPath;
  TH2D* Mask;
  bool MaskPathCheck = options->GetOption("MaskLocation", MaskPath);
  if(MaskPathCheck){
    if(std::filesystem::exists(MaskPath)){
      try{
        std::unique_ptr<TFile> MaskFile(TFile::Open(MaskPath.c_str(), "READ"));
        Mask= (TH2D*)MaskFile->Get("Mask");
      }
      catch(...){
        std::cerr << "Couldn't open Mask root file "  << std::endl;
        return -1;
      }
    }
    else{
    std::cerr << "Invalid Mask Path" << std::endl;
    return -1;
    }
  }
  else{
    std::cerr << "Invalid Mask Path" << std::endl;
    return -1;
  }

  // Open up Effective Area histogram
  std::string EffectiveAreaPath;
  TH1D* EffArea;
  bool EffAPathCheck = options->GetOption("EffectiveAreaRoot", EffectiveAreaPath);
  if(EffAPathCheck){
    if(std::filesystem::exists(EffectiveAreaPath)){
      try{
        std::unique_ptr<TFile> EffAFile(TFile::Open(EffectiveAreaPath.c_str(), "READ"));
        Mask= (TH2D*)EffAFile->Get("Mask");
      }
      catch(...){
        std::cerr << "Couldn't open Effective Area root file "  << std::endl;
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

    // Load in skeleton of AggSkyMap from the first generated background skymap
    TH2D* AggSkyMap;
    std::string seed = AbsBackRootName+"0.root";
    if(!std::filesystem::exists(seed)){
      std::cerr << seed << " could not be found " << std::endl;
      return -1;
    }
    std::unique_ptr<TFile> Seed(TFile::Open(seed.c_str(), "READ"));
    AggSkyMap = (TH2D*)Seed->Get("SkyMap");
    AggSkyMap->Reset();

  // Aggregate all the sky maps that exist
  bool CheckAgg = ReadBackgroundSkyMaps(AbsBackRootName,batches,AggSkyMap);
  std::unique_ptr<TFile> TestFile(TFile::Open("TestFile.root", "RECREATE"));
  TestFile->WriteObject(AggSkyMap, "MyObject");
/*
  TChain SourceCones("Cones");
  ReadConeData(AbsSourceRootName,  batches, SourceCones);
  std::cout << SourceCones.GetEntries() << std::endl;
*/
  return 0;
}