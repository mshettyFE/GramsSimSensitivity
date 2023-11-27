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
int push_to_map(const EntryKey &key, const E values, std::map< EntryKey, std::vector<E> > &Series);
// Reads in GramsG4 Output
// Format of map
// <Run, Event> : [G4Entry,...]
std::map<EntryKey, std::vector<GramsG4Entry > > ReadGramsG4(const std::string GramsG4FileName,bool verbose=false);

// Reads in GramsDetSim Output
std::map<EntryKey, std::vector<GramsDetSimEntry> > ReadGramsDetSim(const std::string GramsDetSimFileName,bool verbose=false);

// Read in output from Extract.cpp
std::map<EntryKey, std::vector<GramsExtractEntry> > ReadExtract(const std::string ExtractFileName, bool MCTruth, bool verbose=false);

// Read in output from Reconstruct.cpp for 2D sky map generation
std::map<EntryKey, GramsRecoEntry> ReadReconstructFromSkyMap(const std::string ReconstructName, bool verbose=false);

#endif