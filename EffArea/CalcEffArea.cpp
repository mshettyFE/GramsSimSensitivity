#include "Options.h"
#include "CalcEffAreaFunctions.h"

#include <string>
#include <iostream>
#include <filesystem>

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
  std::string base_name;
  bool successPath = options->GetOption("start_folder",StartPath);
  bool successBaseName = options->GetOption("base_name",base_name);
  if(!(successBaseName && successPath)){
    std::cerr << "Invalid path or base name " << std::endl;
    return -1;
  }
  else{
    if(StartPath==""){
      std::cerr << "You need to provide a path to the directory containing the root files" << std::endl;
      return -1;
    }
    if(base_name==""){
      std::cerr << "You need to provide a non-empty base_name" << std::endl;
      return -1;
    }
  }

  std::string OutputName;
  bool successOutputName = options->GetOption("Output",OutputName);
  if(!successOutputName){
    std::cerr << "Invalid output file name" << std::endl;
    return -1;
  }

  int NEvents;
  bool successN = options->GetOption("TotalEvents",NEvents);
  if(!successN){
    std::cerr << "Invalid number of events" << std::endl;
    return -1;
  }

  int NBatches;
  bool successNBatch = options->GetOption("TotalBatches",NBatches);
  if(!successNBatch){
    std::cerr << "Invalid number of batches" << std::endl;
    return -1;
  }

  std::string FileTypeName;
  bool successFileTypeName = options->GetOption("FileType",FileTypeName);
  if(!successFileTypeName){
    std::cerr << "Invalid Output File Type" << std::endl;
    return -1;
  }
  else{
    if(!((FileTypeName== ".txt") || (FileTypeName == ".root"))){
      std::cerr << "File Type not supported for write out" << std::endl;
      return -1;
    }
    else{
      OutputName = OutputName+FileTypeName;
    }
  }

  std::string tree_name;
  bool successTreeName = options->GetOption("tree_name",tree_name);
  if(!successTreeName){
    std::cerr << "Invalid tree name" << std::endl;
    return -1;
  }

  std::string current_path = gen_path(StartPath,base_name,10);
  auto mapping = ExtractEffArea(StartPath, base_name, tree_name, NBatches, NEvents,verbose);

/*
  for(auto i=mapping.begin(); i!=mapping.end(); ++i){
    std::cout << (*i).first << '\t' << (*i).second << std::endl;
  }
*/

  return 0;
}