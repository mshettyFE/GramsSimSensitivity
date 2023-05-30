#include <string>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <numeric>

#include "TChain.h"
#include "TFile.h"
#include "TH2D.h"

bool ReadBackgroundSkyMaps(std::string base_name,int nbatches, TH2D* AggSkyMap){
        for(int i=0; i< nbatches; ++i){
          std::string id = std::to_string(i);
          std::string path = base_name+id+".root";
          if(!std::filesystem::exists(path)){
            std::cerr << path << " could not be found " << std::endl;
            continue;
          }
            std::unique_ptr<TFile> Current(TFile::Open(path.c_str(), "READ"));
            TH2D* AddonHist = (TH2D*)Current->Get("SkyMap");
            AggSkyMap->Add(AddonHist);
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

std::vector<double> ExtractNonZero(TH2D* histogram){
    std::vector<double> output;
    auto XAxis = histogram->GetXaxis();
    auto YAxis = histogram->GetYaxis();
    for(int i=0; i<histogram->GetNbinsX(); ++i){
        for(int j=0; j<histogram->GetNbinsY(); ++j){
            double RA = XAxis->GetBinCenter(i);
            double ALT = YAxis->GetBinCenter(j);
            long current_bin = histogram->FindBin(RA,ALT);
            double counts = histogram->GetBinContent(current_bin);
            if(counts >0){
                output.push_back(counts);
            }
        }
    }
    return output;
}

double Mean(std::vector<double> data){
    double sum = std::accumulate(std::begin(data), std::end(data), 0.0);
    return sum /((double) data.size());
}

double StdDev(std::vector<double> data){
    double mean = Mean(data);

    double accum = 0.0;
    std::for_each (std::begin(data), std::end(data), [&](const double d) {
        accum += (d - mean) * (d - mean);
    });

    return sqrt(accum /((double) (data.size()-1)));
}