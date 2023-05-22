#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

std::string gen_path(std::string starting_path, std::string base, int index){
    // convert index to string for concacentation
    std::string place = std::to_string(index);
    // Caculate index of last character in startin_path
    int str_end_index = starting_path.size()-1;
    // If the use has a stray '/', remove it
    if(starting_path[str_end_index]=='/'){
        starting_path = starting_path.substr(0, starting_path.size()-1);
    }
    // Concactenate the final path
    std::string Output = starting_path+'/'+base+place+".root";
    return Output;
}

std::map<double,double> ExtractEffArea(std::string starting_path, std::string base, std::string tree_name, int TotalBatches, int NEvents,bool verbose, double sphere_rad){
    std::map<double,double> output;
    double pi = acos(-1);
    for(int i=0; i<TotalBatches; ++i){
        // Generate the path to a file and check if it exists
        std::string current_path = gen_path(starting_path,base,i);
        if(std::filesystem::exists(current_path)){
            // Open up the reconstruction file and check if it is full
            TFile file(current_path.c_str());
            TTree* tree = (TTree*) file.Get(tree_name.c_str());
            int counts = tree->GetEntries();
            if(counts==0){
                continue;
            }
            // Get the first entry. All entries have the same Truth energy when we perform EffArea calculations
            double TruthEnergy;
            tree->SetBranchAddress("TruthEnergy", &TruthEnergy);
            tree->GetEntry(0);
            // multiply generating surface area of of 4*pi*sphere_rad^2 by the probability that an event was reconstructed
            double EffA = 4*pi*sphere_rad*sphere_rad*(counts/((double) NEvents));
            output.insert({TruthEnergy,EffA});
            if(verbose){
                std::cout << i << '\t' << TruthEnergy << '\t' << EffA << std::endl;
            }
        }
        else{
            std::cerr << current_path << " not found" << std::endl;
            continue;
        }
    }
    return output;
}

void WriteToText(std::string outputName,std::map<double,double> mapping, char seperator){
    // Very basic function to write to a text file. tab seperated by default
    std::ofstream file;
    file.open(outputName);
    file << "Energy\tEffArea\n";
    for(auto i =mapping.begin(); i!=mapping.end(); ++i){
        file << i->first << seperator << i->second << '\n';
    }
    file.close();
}

void WriteToROOT(std::string outputName,std::map<double,double> mapping){
    // Copy over the energies from the map. Energies already sorted
    std::vector<double> binnings;
    for(auto i =mapping.begin(); i!=mapping.end(); ++i){
        binnings.push_back(i->first);
    }

    // Need to add this so that ROOT doesn't scream at me.
    binnings.push_back(binnings[binnings.size()-1]);

    // Open file and create histogram with binning specified by map
    std::unique_ptr<TFile> file( TFile::Open(outputName.c_str(), "RECREATE") );
    TH1D *EffAreaHist = new TH1D("EffArea", "Effective Area", mapping.size(), &binnings[0] );

    // Write out to file and clean up
    file->Write();
    delete EffAreaHist;
}