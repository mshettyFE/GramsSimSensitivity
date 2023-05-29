#include <string>
#include <filesystem>
#include <iostream>
#include <stdexcept>

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
