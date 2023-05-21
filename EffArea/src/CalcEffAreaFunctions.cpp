#include <string>
#include <filesystem>
#include <iostream>

#include "TFile.h"
#include "TTree.h"

#include "ReadRootFiles.h"

std::string gen_path(std::string starting_path, std::string base, int index){
    std::string place = std::to_string(index);
    int str_end_index = starting_path.size()-1;
    if(starting_path[str_end_index]=='/'){
        starting_path = starting_path.substr(0, starting_path.size()-1);
    }
    std::string Output = starting_path+'/'+base+place+".root";
    return Output;
}

std::map<double,double> ExtractEffArea(std::string starting_path, std::string base, std::string tree_name, int TotalBatches, int NEvents,bool verbose, double sphere_rad){
    std::map<double,double> output;
    double pi = acos(-1);
    for(int i=0; i<TotalBatches; ++i){
        std::string current_path = gen_path(starting_path,base,i);
        if(std::filesystem::exists(current_path)){
            auto Data = ReadExtract(current_path);
            auto first_series_data = Data.begin()->second;
            double TruthEnergy = std::get<0>(first_series_data[0]);
            double EffA = 4*pi*sphere_rad*sphere_rad*Data.size()/((double) NEvents);
            output.insert({TruthEnergy,EffA});
            if(verbose){
                std::cout << i << '\t' << TruthEnergy << '\t' << EffA << std::endl;
            }
        }
        else{
            continue;
        }
    }
    return output;
}

