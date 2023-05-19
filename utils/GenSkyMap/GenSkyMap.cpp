#include "Options.h"
#include "ReadRootFiles.h"
#include "GenSkyMapUtils.h"

#include "TH2D.h"

#include <iostream>
#include <cmath>

int main(int argc, char** argv){
// Parser logic adapted from https://github.com/wgseligman/GramsSim/tree/master/util

    auto options = util::Options::GetInstance();
    auto result = options->ParseOptions(argc, argv, "GenSkyMap");

    // Abort if we couldn't parse the job options.
    if (!(result)){
        std::cerr << "ABORT: File " << __FILE__ << " Line " << __LINE__ << " " 
                << std::endl 
                << "Aborting job due to failure to parse options"
                << std::endl;
        exit(EXIT_FAILURE);
        return -1;
    }

    // Check for help, verbose and plot flag
  bool help;
  options->GetOption("help",help);
  if (help) {
    options->PrintHelp();
    exit(EXIT_SUCCESS);
      return 0;
  }

  bool verbose;
  options->GetOption("verbose",verbose);

  bool plot;
  options->GetOption("plot",plot);

// Reads in input and output file names
    std::string RecoName;
    std::string OutputFileName;
    bool successInput = options->GetOption("Input",RecoName);
    bool successOutput = options->GetOption("Output",OutputFileName);
 
    if (!(successInput && successOutput)){
      std::cerr << "Invalid Arguments" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }

  // Read in RABins, ALTBins, and NPts
    double TempRA, TempAlt, TempN;
    int RABins, ALTBins, NPts;
    bool successRA = options->GetOption("RABins",TempRA);
    bool successALT = options->GetOption("ALTBins",TempAlt);
    bool successN = options->GetOption("NPts",TempN);

    if (!(successALT && successN && successRA)){
      std::cerr << "Invalid Arguments" << std::endl;
      exit(EXIT_FAILURE);
      return -1;
    }
    else{
      RABins = static_cast<int>(TempRA);
      ALTBins = static_cast<int>(TempAlt);
      NPts = static_cast<int>(TempN);
      if((RABins <=0) || (ALTBins <= 0) || (NPts <= 0)){
        std::cerr << "Invalid Arguments" << std::endl;
        exit(EXIT_FAILURE);
        return -1;
      }
    }

    std::map<std::tuple<int,int>, std::tuple<double,double,double,double,double,double,double>> ConeData;
    ConeData = ReadReconstructFromSkyMap(RecoName,verbose);
    std::cout << ConeData.size() << std::endl;
    double pi = acos(-1);
    TH2D Seed = MultipleConesToSkyMap(ConeData,RABins,ALTBins,NPts);
    if(plot){
      SaveImage(Seed);
    }
    return 0;
}