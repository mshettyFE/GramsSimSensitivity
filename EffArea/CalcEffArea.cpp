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

  // Get starting folder where EffArea root files are
  std::string StartPath;
  // Get the base name of the root files
  std::string base_name;
  bool successPath = options->GetOption("start_folder",StartPath);
  bool successBaseName = options->GetOption("base_name",base_name);
  if(!(successBaseName && successPath)){
    std::cerr << "Invalid path or base name " << std::endl;
    return -1;
  }
  else{
    // Sanity check that we aren't reading an empty path
    if(StartPath.size()==0){
      std::cerr << "You need to provide a path to the directory containing the root files" << std::endl;
      return -1;
    }
  }

  // Number of events initially generated per batch
  int NEvents;
  bool successN = options->GetOption("TotalEvents",NEvents);
  if(!successN){
    std::cerr << "Invalid number of events" << std::endl;
    return -1;
  }

  // The number of batches
  int NBatches;
  bool successNBatch = options->GetOption("TotalBatches",NBatches);
  if(!successNBatch){
    std::cerr << "Invalid number of batches" << std::endl;
    return -1;
  }

  // Name of output
  std::string OutputName;
  bool successOutputName = options->GetOption("Output",OutputName);
  if(!successOutputName){
    std::cerr << "Invalid output file name" << std::endl;
    return -1;
  }

  // The file type of the output
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
    // If everything is OK, we append the filetype to the output
      OutputName = OutputName+FileTypeName;
    }
  }

  // Name of TTree in root files
  std::string tree_name;
  bool successTreeName = options->GetOption("tree_name",tree_name);
  if(!successTreeName){
    std::cerr << "Invalid tree name" << std::endl;
    return -1;
  }

  // Aggregate all the data into a std::map<double,double>
  auto mapping = ExtractEffArea(StartPath, base_name, tree_name, NBatches, NEvents,verbose);

  // Write out
  if(FileTypeName== ".txt"){
    WriteToText(OutputName,mapping);
  }
  else if(FileTypeName==".root"){
    WriteToROOT(OutputName,mapping);
  }
  else{
    std::cerr << "Something went horribly wrong with writing out EffArea to file" << std::endl;
  }


  return 0;
}