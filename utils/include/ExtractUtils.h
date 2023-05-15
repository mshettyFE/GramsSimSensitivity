#ifndef ExtractUtils
#define ExtractUtils 1
// Define the ROOT includes.
#include <TFile.h>
#include <TTree.h>
#include <ROOT/RDataFrame.hxx>


// The C++ includes.
#include <vector>
#include <map>
#include <cstring>

// Reads in GramsG4 Output
// Format of map
// <Run, Event> : [<TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier>,...]
  std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > ReadGramsG4(std::string GramsG4FileName);

// Reads in GramsDetSim Output
std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > ReadGramsDetSim(std::string GramsDetSimFileName);

// Print output of GramsG4
void print_G4map(std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping);

// Print output of GramsDetSim
void print_DetSimMap(  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > DetMapping);
#endif