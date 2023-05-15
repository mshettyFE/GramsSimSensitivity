#include "Options.h"
#include "ExtractUtils.h"


int main(int argc, char** argv){
// Parser logic adapted from from https://github.com/wgseligman/GramsSim/tree/master/util

    auto options = util::Options::GetInstance();
    auto result = options->ParseOptions(argc, argv, "Extract");

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

// Reads in GramsG4, GramsDetSim and output file names
    std::string G4Name;
    std::string DetSimName;
    std::string outputFile;
    bool successG4 = options->GetOption("GramsG4Name",G4Name);
    bool successGDetSim = options->GetOption("GramsDetSimName",DetSimName);
    bool successOutput = options->GetOption("ExtractOutput",outputFile);

    if (!(successG4 && successGDetSim && successOutput)){
      std::cout << "Invalid Arguments" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }

// Extracts data from GramsG4 into a map. See ExtractUtils.cpp for details
  std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping;
  mapping = ReadGramsG4(G4Name,verbose);
// Extracts data from GramsDetSim into a map. See ExtractUtils.cpp for details
  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > DetMapping;
  DetMapping = ReadGramsDetSim(DetSimName,verbose);
// Select scatter series that match the criterion and write these series to an output root file
  FilterWrite(mapping,DetMapping,outputFile);
  return 0;
}