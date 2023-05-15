#include "Options.h"
#include "ExtractUtils.h"


int main(int argc, char** argv){
// Taken from https://github.com/wgseligman/GramsSim/tree/master/util

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

    // Check for help flag
  bool help;
  options->GetOption("help",help);
  if (help) {
    options->PrintHelp();
    exit(EXIT_SUCCESS);
      return 0;
  }

    std::string G4Name;
    std::string DetSimName;
    bool successG4 = options->GetOption("GramsG4Name",G4Name);
    bool successGDetSim = options->GetOption("GramsDetSimName",DetSimName);

    if (!(successG4 && successGDetSim)){
      std::cout << "Invalid Arguments" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }

  std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping;
  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > DetMapping;
  mapping = ReadGramsG4(G4Name);
  DetMapping = ReadGramsDetSim(DetSimName);
  return 0;
}