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

bool WriteEntry(std::unique_ptr<GramsG4Entry> G4, std::unique_ptr<GramsDetSimEntry> DetSim, TTree* ntuple, bool verbose = false);
// Sort GramsG4 Events by time
void SortEvents(std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> &Events);

// Sort DetSim by time
void SortDetEvents(std::vector<std::tuple<double,double,double,double,double>> &Events);

// calculate L2 Norm squared between adjacent scatter points
std::vector<double> L2NormSquaredAdjacentScatters(std::vector<std::unique_ptr<GramsG4Entry>> G4, 
std::vector<std::unique_ptr<GramsDetSimEntry> Det, bool MCTruth);

// check if two adjacent series are in a unique cell
bool UniqueCellsOnePair(std::tuple<int,int,int,std::string,double,float,float,float,double,int> first, std::tuple<int,int,int,std::string,double,float,float,float,double,int> second,
std::vector<double> Dimensions, std::vector<int> Binnings);

// Filter GramsG4 Series to see if it passes criterion. Return true if it does pass
// Filter types are Standard, Sphere and Unique
bool filter(std::vector<std::unique_ptr<GramsG4Entry>> G4Series, std::vector<std::unique_ptr<GramsG4Entry>> DetSimSeries, std::string SeriesType,
 std::string FilterType,  std::vector<double> Dimensions = {70.0,70.0,80.0} ,  std::vector<int> Binnings = {7,7,8},  double seperation = 10 , bool MCTruth=true, bool verbose = false);

// Figure out if Series is an escape event or not
void escape(std::vector<std::unique_ptr<GramsG4Entry>> series, std::string &outParam);

// Wrapper function to Filter and Write output of Extract to  ROOT file
void FilterWrite(std::map<std::tuple<int,int>, std::vector<std::unique_ptr<GramsG4Entry>> GramsG4Map,
  std::map<std::tuple<int,int,int>, std::vector<std::unique_ptr<GramsDetSimEntry>> GramsDetSimMap,
  std::string outputRootFile, std::string FilterType,
  std::vector<double> Dimensions = {70.0,70.0,80.0},
  std::vector<int> Binnings = {7,7,8},
  double seperation = 10,
  bool MCTruth = true,
  bool verbose=false);
#endif