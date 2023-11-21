#include "Options.h"
#include "ExtractUtils.h"
#include "ReadRootFiles.h"

#include <iostream>
#include <filesystem>


int main(int argc, char** argv){
// Parser logic adapted from https://github.com/wgseligman/GramsSim/tree/master/util

    auto options = util::Options::GetInstance();
    auto result = options->ParseOptions(argc, argv, "Extract");

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

// Reads in GramsG4, GramsDetSim and output file names
    std::string G4Name;
    std::string DetSimName;
    std::string outputFile;
    bool successG4 = options->GetOption("GramsG4Name",G4Name);
    bool successGDetSim = options->GetOption("GramsDetSimName",DetSimName);
    bool successOutput = options->GetOption("ExtractOutput",outputFile);

    if (!(successG4 && successGDetSim && successOutput)){
      std::cerr << "Invalid Arguments" << std::endl;
      return -1;
    }
    else{
      if(!std::filesystem::exists(G4Name)){
        std::cerr << G4Name << " doesn't exists" << std::endl;
        return -1;
      }
      if(!std::filesystem::exists(DetSimName)){
        std::cerr << DetSimName << " doesn't exists" << std::endl;
        return -1;
      }
    }

  // Read in Length Unit and assign an appropriate scale factor to convert all values to units of cm.
  double scaling;
  std::string lengthUnit;
  bool scale = options->GetOption("LengthUnit",lengthUnit);

  if(!scale){
    std::cerr << "Invalid Length Unit" << std::endl;
    return -1;
  }
  else{
    if(lengthUnit=="cm"){
      scaling = 1.0;
    }
    else if (lengthUnit=="mm"){
      scaling = 0.1;
    }
    else{
      std::cerr << "Invalid Length Unit" << std::endl;
      return -1;
    }
  }

  // Get Dimensions for possibility that Filter==UniqueCells
  std::vector<double> Dimensions;
  options->GetOption("Dimensions",Dimensions);
  if ( Dimensions.size() != 3 ) {
     std::cerr << "Dimensions needs exactly three values!" << std::endl;
     return -1;
  }
  // Sanity check that Dimensions has a physical meaning (ie. strictly positive)
  if ((Dimensions[0]<=0.0) || (Dimensions[1] <= 0.0) || (Dimensions[2] <= 0.0) ){
    std::cerr << "Dimensions need to be strictly positive" << std::endl;
    return -1;
  }
  else{
  // Scale all the Dimensions by the scaling factor so that they are in cm
    for(int dim=0; dim<3; ++dim){
      Dimensions[dim] *= scaling;
    }
  }

  // Get Binnings for possibility that Filter==UniqueCells
  std::vector<double> TempBinnings;
  std::vector<int> Binnings;
  options->GetOption("Binnings",TempBinnings);
  if ( TempBinnings.size() != 3 ) {
     std::cerr << "Binnings needs exactly three values!" << std::endl;
     return -1;
  }

  // Make sure that Number of Binnings along each dimension are strictly greater than 1
  if ((TempBinnings[0]<1.0) || (TempBinnings[1]<1.0) || (TempBinnings[2]<1.0) ){
    std::cerr << "Dimensions need to be positive" << std::endl;
    return -1;
  }
  // cast the TempBinnings to a vector of ints to account for users inputting doubles. This effectively floors the input
  else{
    for(int i=0; i<3; ++i){
      Binnings.push_back(static_cast<int>(TempBinnings[i]));
    }
  }

  // Grab Seperation for possibility that Filter=Sphere
  double Seperation;
  bool sep = options->GetOption("Seperation",Seperation);
  if(!sep){
    std::cerr << "Couldn't parser seperation" << std::endl;
    return -1;
  }
  else{
    // Check for negative/zero seperation
    if(sep<=0){
      std::cerr << "Seperation needs to be strictly positive" << std::endl;
      return -1;
    }
  }

  // Grab filter type
  std::string filterValue;
  bool filterType = options->GetOption("Filter",filterValue);
  if(filterType){
  // Make sure that filterValue is either Standard, UniqueCells, or Sphere (ie. if it isn't one of these, exit program)
    if(!((filterValue=="Standard") || (filterValue=="UniqueCells") || (filterValue=="Sphere") )){
      std::cerr << "Invalid Filter Type" << std::endl;
      return -1;      
    }
  }
  else{
    std::cerr << "Couldn't parse Filter type" << std::endl;
    return -1;
  }

// Extracts data from GramsG4 into a map. See ExtractUtils.cpp for details
  std::map<std::vector<int>, std::vector<GramsG4Entry> > mapping;
  mapping = ReadGramsG4(G4Name,verbose);
// Extracts data from GramsDetSim into a map. See ExtractUtils.cpp for details
  std::map<std::vector<int>, std::vector<GramsDetSimEntry> > DetMapping;
  DetMapping = ReadGramsDetSim(DetSimName,verbose);
// Select scatter series that match the criterion and write these series to an output root file
// Note that the output always write the first interaction as a Primary gamma ray, and not a Compton scatter
  FilterWrite(mapping,DetMapping, outputFile, filterValue, Dimensions, Binnings, Seperation,MCTruth, verbose);
  return 0;
}