#ifndef ExtractUtils
#define ExtractUtils 1

// Define the ROOT includes.
#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"

// The C++ includes.
#include <vector>
#include <map>
#include <cstring>
#include <memory>

#include "GramsG4Entry.h"
#include "DetSimEntry.h"

// calculate L2 Norm squared between adjacent scatter points
std::vector<double> L2NormSquaredAdjacentScatters(const std::vector<GramsG4Entry>& G4);

// check if two adjacent series are in a unique cell
bool UniqueCellsOnePairMC(const GramsG4Entry& first, const GramsG4Entry& second, const std::vector<double> Dimensions, const  std::vector<int> Binnings);

// Filter GramsG4 Series to see if it passes criterion. Return true if it does pass
// Filter types are Standard, Sphere and Unique
bool filter(const std::vector<GramsG4Entry>& G4Series, const std::string SeriesType,const std::string FilterType, 
const std::vector<double> Dimensions = {70.0,70.0,80.0} ,  const std::vector<int> Binnings = {7,7,8}, 
const double seperation = 10 , const bool MCTruth=true, const bool verbose = false);

// Figure out if Series is an escape event or not
void escape(const std::vector<GramsG4Entry>& series, std::string &outParam);

// Wrapper function to Filter and Write output of Extract to  ROOT file
void FilterWrite(const std::map<std::vector<int>, std::vector<GramsG4Entry>>& GramsG4Map,
  const std::map<std::vector<int>, std::vector<GramsDetSimEntry>> & GramsDetSimMap,
  const std::string outputRootFile, const std::string FilterType,
  const std::vector<double> Dimensions = {70.0,70.0,80.0},
  const std::vector<int> Binnings = {7,7,8},
  const double seperation = 10,
  const bool MCTruth = true,
  const bool verbose=false);
#endif
