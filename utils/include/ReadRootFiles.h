#ifndef ReadROOTFiles
#define ReadROOTFiles 1

#include <vector>
#include <map>
#include <string>
#include <tuple>

// Reads in GramsG4 Output
// Format of map
// <Run, Event> : [<TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier>,...]
  std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > ReadGramsG4(std::string GramsG4FileName,bool verbose=false);

// Reads in GramsDetSim Output
std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > ReadGramsDetSim(std::string GramsDetSimFileName,bool verbose=false);

// Print output of GramsG4
void print_G4map(std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping);

// Print output of GramsDetSim
void print_DetSimMap(  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > DetMapping);

// Print output of Extraction
void print_ExtractMap(std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > Mapping);

// Print the REconstruction data used to generate a sky map
void print_ReconstructDataFromSkyMap(std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double>> > Mapping);

// Push key value pair to map
void push_to_map(std::tuple<int,int> &key, std::tuple<double,double,double,double,double,double,double,double,double,double,std::string> &values, 
  std::map< std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > &Series);

// Read in output from Extract.cpp
std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > ReadExtract(
  std::string ExtractFileName, bool verbose=false);

// Read in output from Reconstruct.cpp for 2D sky map generation
std::map<std::tuple<int,int>, std::tuple<double,double,double,double,double,double,double,double>> ReadReconstructFromSkyMap(std::string ReconstructName, bool verbose=false);
#endif