// Define the ROOT includes.
#include <ROOT/RDataFrame.hxx>


// The C++ includes.
#include <vector>
#include <map>
#include <cstring>
#include <iostream>

void print_G4map(std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping){
      for ( auto i = mapping.begin(); i != mapping.end(); ++i ){
        auto key = (*i).first;
        auto value = (*i).second;
        std::cout << " run=" << std::get<0>(key) << " event= " << std::get<1>(key) << std::endl; 
        for (const auto& event : value){
          std::cout <<'\t'
          << " trackID= " << std::get<0>(event)
          << " ParentID= " << std::get<1>(event)
          << " PDGCode= " << std::get<2>(event)
          << " ProcessName= " << std::get<3>(event)
          << " t= " << std::get<4>(event)
          << " x= " << std::get<5>(event)
          << " y= " << std::get<6>(event)
          << " z= " << std::get<7>(event)
          << " Etot= " << std::get<8>(event)
          << " identifier= " << std::get<9>(event)
          << std::endl;
        }
      }
    }

std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > ReadGramsG4(std::string GramsG4FileName){
// Format of map
// <Run, Event> : [<TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier>,...]
  std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping;

// Leaves of TrackInfo NTuple
// Run             int
// Event           int
// TrackID         int
// ParentID        int
// PDGCode         int
// ProcessName     string
// t               = double
// x               = float
// y               = float
// z               = float
// Etot            = double
// px              = float
// py              = float
// pz              = float
// identifier      = int

// Open Input data
  ROOT::RDataFrame trackInfo( "TrackInfo", GramsG4FileName, {"Run", "Event", "TrackID","ParentID","PDGCode","ProcessName","t",
    "x","y","z","Etot","identifier"} );

// Lambda expression to extract variables from input
  trackInfo.Foreach(
    [&mapping]( int run, int event, int trackID, int ParentID, int PDGCode,
ROOT::VecOps::RVec<char> process,ROOT::VecOps::RVec<double> t, ROOT::VecOps::RVec<float> x,ROOT::VecOps::RVec<float> y,
ROOT::VecOps::RVec<float> z,ROOT::VecOps::RVec<double> Etot, ROOT::VecOps::RVec<int> identifier)
    {
// Read in processName
      std::string processName;
      for ( auto i = process.begin(); i != process.end() && (*i) != 0; ++i ){
        processName.push_back(*i);
      }
// Create key and value pair
      std::tuple<int,int> key = std::make_tuple(run,event);
// Only read the first value of the vectors, since that is where the interaction occurred at
      std::tuple<int,int,int,std::string,double,float,float,float,double,int> hit = std::make_tuple(trackID,ParentID,PDGCode,processName,
      t[0],x[0],y[0],z[0],Etot[0],identifier[0]);
// Check if key exists. If it doesn't, create a new series and add to map
      if(mapping.count(key) == 0){
        std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> series;
        series.push_back(hit);
        mapping[key] = series;
     }
      else{
// If the series exists, add hit to series
        mapping[key].push_back(hit);
      }
    }, 
{"Run", "Event", "TrackID","ParentID","PDGCode","ProcessName","t","x","y","z","Etot","identifier"}
            );// end of ForEach
    return mapping;
}