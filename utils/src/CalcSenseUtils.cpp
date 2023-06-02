#include <string>
#include <tuple>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <numeric>

#include "TChain.h"
#include "TFile.h"
#include "TH2D.h"

bool ReadBackgroundCounts(std::string base_name,int nbatches, TH1D* AggBackCounts, bool verbose){
    for(int i=0; i< nbatches; ++i){
        // run through all the files
        std::string id = std::to_string(i);
        std::string path = base_name+id+".root";
        // If they don't exist, move onto the next one
        if(!std::filesystem::exists(path)){
          if(verbose){
              std::cerr << path << " could not be found " << std::endl;
          }
          continue;
        }
        //  read in counts and add to aggregate
        std::unique_ptr<TFile> Current(TFile::Open(path.c_str(), "READ"));
        TH1D* AddonHist = (TH1D*)Current->Get("Counts");
        // If histogram doesn't exist, move on
        if(AddonHist==NULL){
            std::cerr << "Couldn't read Counts histogram in " << AddonHist << std::endl;
            continue;
        }
        AggBackCounts->Add(AddonHist);
    }
  return true;
}

int ExtractBinNum(double SourceEnergy, TH1D* Histogram){
    // Get number of bins and XAxis object
    int nbins = Histogram->GetNbinsX();
    auto XAxis = Histogram->GetXaxis();
    // Get the center of the highest bin
    double upper_bound = XAxis->GetBinCenter(nbins);
    // If energy too high, default to the highest bin number
    if(SourceEnergy>= upper_bound){
        std::cerr << "Source Energy too high. Defaulting to highest bin..." << std::endl;
        return nbins-1;
    }
    // If energy too low, default to the lowest bin number
    double lower_bound = XAxis->GetBinLowEdge(1);
    if(SourceEnergy<= lower_bound){
        std::cerr << "Source Energy too low. Defaulting to lowest bin..." << std::endl;
        return 1;
    }
    // Otherwise, just use normal function
    return Histogram->FindBin(SourceEnergy);
}


double ExtractEffArea(double SourceEnergy, TH1D* EffAreaHist){
    return EffAreaHist->GetBinContent(ExtractBinNum(SourceEnergy,EffAreaHist));
}