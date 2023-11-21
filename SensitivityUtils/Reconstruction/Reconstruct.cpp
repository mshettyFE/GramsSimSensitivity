#include "TTree.h"
#include "TFile.h"

#include<vector>
#include<map>
#include<tuple>
#include<string>
#include<iostream>
#include<filesystem>
#include<memory>

#include "ExtractionEntry.h"
#include "ReadRootFiles.h"
#include "Options.h"
#include "ReconstructUtils.h"
#include "UsefulTypeDefs.h"

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
        return -1;
    }

    // Check for help and verbose flag
  bool help;
  options->GetOption("help",help);
  if (help) {
    options->PrintHelp();
      return 0;
  }

  bool verbose;
  options->GetOption("verbose",verbose);

  bool MCTruth;
  options->GetOption("MCTruth",MCTruth);

  std::string inputFileName;
  std::string outputFileName;
  bool checkInput = options->GetOption("Input",inputFileName);
  bool checkOutput = options->GetOption("Output",outputFileName);
  if(!checkInput){
    std::cerr << "Couldn't parse input file name" << std::endl;
    return -1;
  }
  else{
    if(!std::filesystem::exists(inputFileName)){
      std::cerr << inputFileName << " doesn't exist" << std::endl;
      return -1;
    }
  }
  if(!checkOutput){
    std::cerr << "Couldn't parse output file name" << std::endl;
    return -1;
  }

  // Read in Source Types
  std::string SourceType;
  bool CheckSourceType = options->GetOption("SourceType",SourceType);
  if(!CheckSourceType){
    std::cerr << "Couldn't parse Source Type" << std::endl;
    return -1;
  }
  else{
    if(!((SourceType=="Iso") || (SourceType=="Point"))){
      std::cerr << "Invalid Source Type. Needs to be either 'Iso' or 'Point'" << std::endl;
      return -1;
    }
  }

  // Read In Source Location in spherical coordinates and convert to cartesian
  std::vector<double> TempVector;
  bool CheckSourceLoc = options->GetOption("SourceLoc",TempVector);
  if(!CheckSourceLoc){
    std::cerr << "Couldn't parse source location" << std::endl;
    return -1;
  }

  SkyMapLoc Spherical = std::make_tuple(TempVector[0],TempVector[1]);
  R3 truthLoc = SphereToCart(Spherical);

  std::map<EntryKey, std::vector<GramsExtractEntry> > Series;
  Series = ReadExtract(inputFileName,verbose);

  TFile* OFile = new TFile(outputFileName.c_str(), "RECREATE");
  TTree* tree;
  tree = new TTree("Cones","Compton Cones");
  // Fill tree with Reconstructed Cone Data
  Reconstruction(Series,tree,truthLoc, MCTruth, verbose, SourceType);
  // Write TTree to File
  tree->Write();
  // Clean Up
  delete tree;
  OFile->Close();
  return 0;
}