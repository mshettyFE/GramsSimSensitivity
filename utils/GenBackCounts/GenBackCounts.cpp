#include "Options.h"
#include "ReadRootFiles.h"
#include "GenBackCountsUtils.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"

#include <iostream>
#include <cmath>

int main(int argc, char** argv){
// Parser logic adapted from https://github.com/wgseligman/GramsSim/tree/master/util

    auto options = util::Options::GetInstance();
    auto result = options->ParseOptions(argc, argv, "GenSkyMap");

    // Abort if we couldn't parse the job options.
    if (!(result)){
        std::cerr << "ABORT: File " << __FILE__ << " Line " << __LINE__ << " " 
                << std::endl 
                << "Aborting job due to failure to parse options"
                << std::endl;
        exit(EXIT_FAILURE);
        return -1;
    }

    // Check for help, verbose and plot flag
  bool help;
  options->GetOption("help",help);
  if (help) {
    options->PrintHelp();
    exit(EXIT_SUCCESS);
      return 0;
  }

  bool verbose;
  options->GetOption("verbose",verbose);

// Reads in input and output file names
  std::string RecoName;
  std::string OutputFileName;
  bool successInput = options->GetOption("Input",RecoName);
  bool successOutput = options->GetOption("Output",OutputFileName);

  if (!(successInput && successOutput)){
    std::cerr << "Invalid Input and output file names" << std::endl;
    exit(EXIT_FAILURE);
    return -1;
  }

// Reads in ReferenceFlux, PhysicalFlux, and EffectiveArea histograms
    std::string ReferenceFluxRoot,PhysicalFluxRoot,EffectiveAreaRoot, MaskRoot;
    bool successRef = options->GetOption("ReferenceFluxFile",ReferenceFluxRoot);
    bool successPhys = options->GetOption("PhysicalFluxFile",PhysicalFluxRoot);
    bool successEffArea = options->GetOption("EffAreaFile",EffectiveAreaRoot);
    bool successMask = options->GetOption("MaskFile",MaskRoot);
 
    if (!(successRef && successEffArea && successPhys && successMask)){
      std::cerr << "Invalid Root histograms" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }
    else{
      if(EffectiveAreaRoot.size()<=0){
        std::cerr << "Need to provide an effective area file. Generate from CalcEffArea.py" << std::endl;
        return -1;
      }
      if(ReferenceFluxRoot.size()<=0){
        std::cerr << "Need to provide a reference flux. Generate from GenEnergySpectrum.py" << std::endl;
        return -1;
      }
      if(PhysicalFluxRoot.size()<=0){
        std::cerr << "Need to provide a physical flux. Generate from GenEnergySpectrum.py" << std::endl;
        return -1;
      }
      if(MaskRoot.size()<=0){
        std::cerr << "Need to provide a binary mask. Generate from GenMask.py" << std::endl;
        return -1;
      }
    }

  // Read in NPts
    int NPts;
    bool successN = options->GetOption("NPts",NPts);

    if (!(successN)){
      std::cerr << "Invalid Number of Points per cone" << std::endl;
      return -1;
    }
    else{
      if((NPts <= 0)){
        std::cerr << "Invalid number of points per cone" << std::endl;
        return -1;
      }
    }

    double ExposureTime;
    bool successExposureTime = options->GetOption("ExposureTime",ExposureTime);
    if (!(successExposureTime)){
      std::cerr << "Invalid Exposure time" << std::endl;
      return -1;
    }
    else{
      if(ExposureTime<=0){
        std::cerr << "Exposure needs be positive and non-zero" << std::endl;
        exit(EXIT_FAILURE);
        return -1;
      }
    }

    std::string TempNEvents;
    long NEvents;
    bool successNEvents = options->GetOption("TotalEvents",TempNEvents);
    if (!(successNEvents)){
      std::cerr << "Invalid Number of total events" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }
    else{
      try{
        NEvents = std::stol(TempNEvents);
      }
      catch(...){
        std::cerr << "Can't convert NEvents to long "  << std::endl;
        return -1;
      }
      if(NEvents<=0){
        std::cerr << "Number of total events needs be positive and non-zero" << std::endl;
        exit(EXIT_FAILURE);
        return -1;
      }
    }

    std::map<std::tuple<int,int>, std::tuple<double,double,double,double,double,double,double,double>> ConeData;
    ConeData = ReadReconstructFromSkyMap(RecoName,verbose);

    std::unique_ptr<TFile> EffAreaFile(TFile::Open(EffectiveAreaRoot.c_str(), "READ"));
    std::unique_ptr<TFile> RefFile(TFile::Open(ReferenceFluxRoot.c_str(), "READ"));
    std::unique_ptr<TFile> PhysFile(TFile::Open(PhysicalFluxRoot.c_str(), "READ"));
    std::unique_ptr<TFile> MaskFile(TFile::Open(MaskRoot.c_str(), "READ"));

    TH1D* EffAreaFlux= (TH1D*)EffAreaFile->Get("EffArea");
    TH1D* RefFlux = (TH1D*)RefFile->Get("ReferenceFlux");
    TH1D* PhysFlux = (TH1D*)PhysFile->Get("PhysicalFlux");
    TH2D* Mask = (TH2D*)MaskFile->Get("Mask");

    TH1D* BackgroundCounts = (TH1D*) EffAreaFlux->Clone();
    BackgroundCounts->Reset();

    int NbinsPhys = PhysFlux->GetNbinsX(); 
    int NbinsEffArea = EffAreaFlux->GetNbinsX(); 
    if(NbinsPhys != NbinsEffArea){
      std::cout << "Binnings don't match. Physical flux has " << NbinsPhys << " while EffArea has " << NbinsEffArea << std::endl;
      return -1;
    }

    std::unique_ptr<TFile> OutputFile(TFile::Open(OutputFileName.c_str(), "RECREATE"));

    ConesToCountsWeighted(ConeData, NPts, EffAreaFlux, PhysFlux, RefFlux, Mask, BackgroundCounts, ExposureTime,NEvents);
   OutputFile->WriteObject(BackgroundCounts, "BackgroundCounts");
 
  // Old code for when I though I needed the reconstrution image
  // If you want to generate sky maps, uncomment code
  /*
    TH2D Seed;
    Seed = MultipleConesToSkyMapWeighted(ConeData,RABins,ALTBins,NPts,EffAreaFlux,PhysFlux,RefFlux,ExposureTime,NEvents);
    OutputFile->WriteObject(&Seed, "SkyMap");
  */
    return 0;
}