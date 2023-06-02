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
        std::string id = std::to_string(i);
        std::string path = base_name+id+".root";
        if(!std::filesystem::exists(path)){
          if(verbose){
              std::cerr << path << " could not be found " << std::endl;
          }
          continue;
        }
        std::unique_ptr<TFile> Current(TFile::Open(path.c_str(), "READ"));
        TH1D* AddonHist = (TH1D*)Current->Get("Counts");
        AggBackCounts->Add(AddonHist);
    }
return true;
}


void ReadConeData(std::string base_name, int nbatches, TChain &Output){
    for(int i=0; i<nbatches; ++i){
        std::string id = std::to_string(i);
        std::string path = base_name+id+".root";
        if(std::filesystem::exists(path)){
            Output.Add(path.c_str());
        }
        else{
            std::cerr << "Couldn't find " << path << std::endl;
        }
    }
}

int ExtractBinNum(double SourceEnergy, TH1D* Histogram){
    int nbins = Histogram->GetNbinsX();
    double upper_bound = Histogram->GetXaxis()->GetBinCenter(nbins);
    if(SourceEnergy>= upper_bound){
        std::cerr << "Source Energy too high. Defaulting to highest bin..." << std::endl;
        return nbins-1;
    }
    return Histogram->FindBin(SourceEnergy);
}


double ExtractEffArea(double SourceEnergy, TH1D* EffAreaHist){
    int nbins = EffAreaHist->GetNbinsX();
    double upper_bound = EffAreaHist->GetXaxis()->GetBinCenter(nbins);
    if(SourceEnergy>= upper_bound){
        std::cerr << "Source Energy too high. Defaulting to highest bin..." << std::endl;
        return     EffAreaHist->GetBinContent(nbins-1);
    }
    return EffAreaHist->GetBinContent(EffAreaHist->FindBin(SourceEnergy));
}

std::tuple<long,long> CalculateUsedPhotons(std::string base_name, int nbatches, int PhotonsPerBatch, double threshold){
    //0: TotalPhotons, 1: TotalCones associated with total photons
    std::tuple<long,long> output;

    int count = 0;
    for(int i=0; i<nbatches; ++i){
        std::string id = std::to_string(i);
        std::string path = base_name+id+".root";
        long sum = 0;
        if(std::filesystem::exists(path)){
            count++;
            std::unique_ptr<TFile> SourceCones(TFile::Open(path.c_str(), "READ"));
            TH1D* Cones = (TH1D*) SourceCones->Get("Cones");
            sum += Cones->GetEntries();
            if(sum>=threshold){
                long nphotons = count*PhotonsPerBatch;
                long ncones = sum;
                output = std::make_tuple(nphotons,ncones);
                return output;
            }
        }
        else{
            std::cerr << "Couldn't find " << path << std::endl;
            continue;
        }
    }

    std::cerr << "Threshold not met" << std::endl;
    output= std::make_tuple(0,0);
    return output;
}
