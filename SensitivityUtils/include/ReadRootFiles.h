#ifndef ReadROOTFiles
#define ReadROOTFiles 1

#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "GramsG4Entry.h"
#include "DetSimEntry.h"
#include "ExtractionEntry.h"
#include "RecoEntry.h"

template <typename E>
int push_to_map(EntryKey &key, E values, std::map< EntryKey, std::vector<std::unique_ptr<E>> > &Series);

// Reads in GramsG4 Output
// Format of map
// <Run, Event> : [G4Entry,...]
  std::map<std::vector<int>, std::vector<std::unique<GramsG4Entry>>> ReadGramsG4(std::string GramsG4FileName,bool verbose=false);


// Reads in GramsDetSim Output
std::map<std::vector<int>, std::vector<std::unique_ptr<GramsDetSimEntry>> > ReadGramsDetSim(std::string GramsDetSimFileName,bool verbose=false);

// Read in output from Extract.cpp
std::map<std::vector<int>, std::vector<std::unique_ptr<ExtractionEntry>> > ReadExtract(
  std::string ExtractFileName, bool verbose=false);

// Read in output from Reconstruct.cpp for 2D sky map generation
std::map<std::vector<int>, std::unique_ptr<RecoEntry>> ReadReconstructFromSkyMap(std::string ReconstructName, bool verbose=false);

#endif