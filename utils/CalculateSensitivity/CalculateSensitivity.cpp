#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "TChain.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"

#include "Options.h"
#include "CalcSenseUtils.h"
#include "GenBackCountsUtils.h"

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
  bool BackPathCheck = options->GetOption("BackgroundCountFolder", BackgroundPath);
  bool BackBaseCheck  = options->GetOption("BackgroundBaseName", BackgroundBase);
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

  // Read in NPts
    int NPts;
    bool successN = options->GetOption("NPts",NPts);
    if (!(successN)){
      std::cerr << "Invalid Number of Points per cone" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }
    else{
      if( (NPts <= 0)){
        std::cerr << "Invalid Number of points per cone" << std::endl;
        exit(EXIT_FAILURE);
        return -1;
      }
    }

    // Determine number of events per batch job of source.
    // This is a workaround of parsing a string and fact checking that it can be a positive long
    // I really didn't want to try and template Options.h
    std::string TempNEvents;
    long SourceEventsPerJob;
    bool successNEvents = options->GetOption("SourceEventsPerJob",TempNEvents);
    if (!(successNEvents)){
      std::cerr << "Invalid Number of total events" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }
    else{
        // try catch to see if we can convert to long
      try{
        SourceEventsPerJob = std::stol(TempNEvents);
      }
      catch(...){
        std::cerr << "Can't convert NEvents to long "  << std::endl;
        return -1;
      }
    // We need SourceEventsPerJob to be strictly positive
      if(SourceEventsPerJob<=0){
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

    // Energy of source is strictly positive
    double SourceEnergy;
    bool CheckSourceEnergy = options->GetOption("SourceEnergy",SourceEnergy);
    if(CheckSourceEnergy){
        if(SourceEnergy<=0){
            std::cerr << "Invalid Source Energy. needs to be strictly positive" << std::endl;
            return -1;
        }
    }
    else{
        std::cerr << "Invalid Source Energy. needs to be strictly positive" << std::endl;
        return -1;
    }

  // Open up Mask histogram
  std::string MaskPath;
  bool MaskPathCheck = options->GetOption("MaskLocation", MaskPath);
  if(MaskPathCheck){
    if(std::filesystem::exists(MaskPath)){
      try{
        // Make sure we can read file
        std::unique_ptr<TFile> MaskFile(TFile::Open(MaskPath.c_str(), "READ"));
        TH2D* Mask= (TH2D*)MaskFile->Get("Mask");
        if(Mask==NULL){
          throw 1;
        }
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

    std::cout << "Reading in Background..." << std::endl;

    // Load in skeleton of BackgroundCounts from the first background count histogram
    TH1D* AggBackCounts;
    std::string seed = AbsBackRootName+"0.root";
    if(!std::filesystem::exists(seed)){
      std::cerr << seed << " could not be found " << std::endl;
      return -1;
    }

  std::unique_ptr<TFile> Seed(TFile::Open(seed.c_str(), "READ"));
  AggBackCounts = (TH1D*)Seed->Get("BackgroundCounts");
  AggBackCounts->Reset();

  // Get bin number of region of background we are comparing against
  int TargetBin = ExtractBinNum(SourceEnergy, AggBackCounts);

  // Aggregate all the sky maps that exist
  bool CheckAgg = ReadBackgroundSkyMaps(AbsBackRootName,batches,AggBackCounts);

  std::unique_ptr<TFile> MaskFile(TFile::Open(MaskPath.c_str(), "READ"));
  TH2D* Mask = (TH2D*)MaskFile->Get("Mask");

  double mu = AggBackCounts->GetBinContent(TargetBin);
  double sigma = AggBackCounts->GetBinError(TargetBin);
  double ThreeSigmaThreshold = mu+3*sigma;
  double FiveSigmaThreshold = mu+5*sigma;

  std::cout << "Mean: " << mu << '\t' << "Sigma: " << sigma << std::endl;
 
  int RA_Bins = AggBackCounts->GetNbinsX(); 
  int ALT_Bins = AggBackCounts->GetNbinsY(); 

  std::cout << "Reading Source Cones..." << std::endl;
  TChain SourceCones("Cones");
  ReadConeData(AbsSourceRootName,  batches, SourceCones);
  Long64_t TotalCones = SourceCones.GetEntries();
  double xDir, yDir, zDir,xTip, yTip, zTip, RecoAngle;
  SourceCones.SetBranchAddress("xDir", &xDir);
  SourceCones.SetBranchAddress("yDir", &yDir);
  SourceCones.SetBranchAddress("zDir", &zDir);
  SourceCones.SetBranchAddress("xTip", &xDir);
  SourceCones.SetBranchAddress("yTip", &yDir);
  SourceCones.SetBranchAddress("zTip", &zDir);
  SourceCones.SetBranchAddress("RecoAngle",&RecoAngle);

  double Tally = 0.0;
  Long64_t ThreeSigmaCount;
  bool ThreeSigmaFlag = false;
  Long64_t FiveSigmaCount;
  bool FiveSigmaFlag = false;
  Long64_t count = 0;
  std::cout << "Calculating Sensitivity..." << std::endl;
  for(Long64_t i=0; i<TotalCones; ++i){
    if(verbose){
      std::cout << "Current Cone: " << i << " Total Cones: " << TotalCones <<  " Current Count: " << Tally <<
       " Thresholds: " << ThreeSigmaThreshold << "\t" << FiveSigmaThreshold << std::endl;
    }
    SourceCones.GetEntry(i);
    TH2D SourceHist = ConeToSkyMap( xDir,  yDir,  zDir,  xTip,  yTip,  zTip, RecoAngle, 
     RA_Bins,  ALT_Bins,NPts);
    double inside_count = SourceHist.Integral();
    if(inside_count > 0){
      Tally += inside_count;
      count += 1;
    }
    if((Tally>= ThreeSigmaThreshold) && !(ThreeSigmaFlag)){
      ThreeSigmaFlag = true;
      ThreeSigmaCount = count;
    }
    if((Tally>= FiveSigmaThreshold) && !(FiveSigmaFlag)){
      FiveSigmaFlag = true;
      FiveSigmaCount = count;
      break;
    }
  }


  std::tuple<long,long> Used3Sigma = CalculateUsedPhotons(AbsSourceRootName,batches,  SourceEventsPerJob, ThreeSigmaCount);
  std::tuple<long,long> Used5Sigma = CalculateUsedPhotons(AbsSourceRootName, batches, SourceEventsPerJob, FiveSigmaCount);

  // Fix this. Total cones to cones read in files
  double NPhotonsThreeSigma = ((ThreeSigmaCount)/((double)(std::get<1>(Used3Sigma)))) * ((double) std::get<0>(Used3Sigma));
  double NPhotonsFiveSigma = ((FiveSigmaCount)/((double)(std::get<1>(Used5Sigma)))) * ((double) std::get<0>(Used5Sigma));

  std::unique_ptr<TFile> EffAFile(TFile::Open(EffectiveAreaPath.c_str(), "READ"));
  TH1D* EffA= (TH1D*) EffAFile->Get("EffArea");

  double SourceEffA = ExtractEffArea(SourceEnergy, EffA);


  double MeVtoErgs = 1.60218E-6;

  double ThreeSigmaDiffSensitivity = NPhotonsThreeSigma*SourceEnergy*MeVtoErgs/(SourceEffA*ExposureTime);
  double FiveSigmaDiffSensitivity = NPhotonsFiveSigma*SourceEnergy*MeVtoErgs/(SourceEffA*ExposureTime);
  double ThreeSigmaPhotonSensitivity = NPhotonsThreeSigma/(SourceEffA*ExposureTime);
  double FiveSigmaPhotonSensitivity = NPhotonsFiveSigma/(SourceEffA*ExposureTime);


  std::cout << "At a source energy of "<< SourceEnergy << " MeV and exposure time of " << ExposureTime << " seconds , the sensitivity is (in ergs*cm^-2*s^-1):" << std::endl;
  std::cout << "\tThree Sigma: " << ThreeSigmaDiffSensitivity << std::endl;
  std::cout << "\tFive Sigma: " << FiveSigmaDiffSensitivity << std::endl;
  std::cout << "In photons/cm^2/s:" <<std::endl;
  std::cout << "\tThree Sigma: " << ThreeSigmaPhotonSensitivity << std::endl;
  std::cout << "\tFive Sigma: " << FiveSigmaPhotonSensitivity << std::endl;

  return 0;
}