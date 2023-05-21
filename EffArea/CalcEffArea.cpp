#include "Options.h"
#include "CalcEffAreaFunctions.h"

#include <string>
#include <iostream>

int main(int argc, char** argv){
// Parser logic adapted from https://github.com/wgseligman/GramsSim/tree/master/util

    auto options = util::Options::GetInstance();
    auto result = options->ParseOptions(argc, argv, "CalcEffArea");

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

  std::string StartPath;
  bool successPath = options->GetOption("start_folder",StartPath);
  std::string base_name;
  bool successBaseName = options->GetOption("base_name",base_name);
  if(!(successBaseName && successPath)){
    std::cerr << "Invalid path or base name " << std::endl;
    return -1;
  }

  int NEvents;
  bool successN = options->GetOption("TotalEvents",NEvents);
  if(!successN){
    std::cerr << "Invalid number of events" std::endl;
  }

  for

  return 0;
}