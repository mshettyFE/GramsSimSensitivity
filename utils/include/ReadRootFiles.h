#ifndef ReadROOTFiles
#define ReadROOTFiles 1

#include "UsefulTypeDefs.h"

#include <vector>
#include <map>
#include <string>
#include <tuple>

// Reads in GramsG4 Output
// Format of map
// <Run, Event> : [G4Entry,...]
  std::map<std::tuple<int,int>, std::vector<G4Entry>> ReadGramsG4(std::string GramsG4FileName,bool verbose=false);

// Reads in GramsDetSim Output
std::map<std::tuple<int,int,int>, std::vector<DetSimEntry> > ReadGramsDetSim(std::string GramsDetSimFileName,bool verbose=false);

// Print output of GramsG4
void print_G4map(std::map<std::tuple<int,int>, std::vector<G4Entry> > mapping);

// Print output of GramsDetSim
void print_DetSimMap(  std::map<std::tuple<int,int,int>, std::vector<DetSimEntry> > DetMapping);

// Print output of Extraction
void print_ExtractMap(std::map<std::tuple<int,int>, std::vector<ExtractEntry> > Mapping);

// Print the REconstruction data used to generate a sky map
void print_ReconstructDataFromSkyMap(std::map<std::tuple<int,int>, std::vector<ReconstructEntry> > Mapping);

// Push key value pair to map
void push_to_map(std::tuple<int,int> &key, ExtractEntry &values, 
  std::map< std::tuple<int,int>, std::vector<ExtractEntry> > &Series);

// Read in output from Extract.cpp
std::map<std::tuple<int,int>, std::vector<ExtractEntry> > ReadExtract(
  std::string ExtractFileName, bool verbose=false);

// Read in output from Reconstruct.cpp for 2D sky map generation
std::map<std::tuple<int,int>, ReconstructEntry> ReadReconstructFromSkyMap(std::string ReconstructName, bool verbose=false);
#endif