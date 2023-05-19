#include "TTree.h"
#include "TFile.h"

#include "ReadRootFiles.h"
#include "Options.h"
#include "ReconstructUtils.h"

#include<vector>
#include<map>
#include<tuple>
#include<string>
#include<iostream>

int main(int argc, char** argv){
  // Parser logic adapted from https://github.com/wgseligman/GramsSim/tree/master/util

    auto options = util::Options::GetInstance();
    auto result = options->ParseOptions(argc, argv, "Reconstruct");

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

  std::string inputFileName;
  std::string outputFileName;
  options->GetOption("Input",inputFileName);
  options->GetOption("Output",outputFileName);

//  std::string fname = "/nevis/milne/files/ms6556/Spring2023/Sensitivity/SensitivityJob/SourceExtracted_0.root";

  // Read In Source Location in spherical coordinates and convert to cartesian
  std::vector<double> TempVector;
  options->GetOption("SourceLoc",TempVector);
  std::tuple<double,double> Spherical = std::make_tuple(TempVector[0],TempVector[1]);
  std::tuple<double,double,double> truthLoc = SphereToCart(Spherical);

  std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > Series;
  Series = ReadExtract(inputFileName,verbose);

  TFile* OFile = new TFile(outputFileName.c_str(), "RECREATE");
  TTree* tree;
  tree = new TTree("Cones","Compton Cones");
  // Fill tree with Reconstructed Cone Data
  Reconstruction(Series,tree,truthLoc);
  // Write TTree to File
  tree->Write();
  // Clean Up
  delete tree;
  OFile->Close();
  return 0;
}