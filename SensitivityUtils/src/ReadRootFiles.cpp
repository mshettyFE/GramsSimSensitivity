// Define the ROOT includes.
#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

// The C++ includes.
#include <vector>
#include <map>
#include <cstring>
#include <iostream>

#include "GramsG4Entry.h"
#include "UsefulTypeDefs.h"

void print_G4map(std::map<std::tuple<int,int>, std::vector<G4Entry> > mapping){
  // Utility function to print contents of GramsG4
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


std::map<std::tuple<int,int>, std::vector<G4Entry> > ReadGramsG4(std::string GramsG4FileName, bool verbose){
  // See type defs for G4Entry
  // Uses <run,event> as key, and a list of interactions as the value
    std::map<std::tuple<int,int>, std::vector<G4Entry> > mapping;

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
      G4Entry hit = std::make_tuple(trackID,ParentID,PDGCode,processName,
      t[0],x[0],y[0],z[0],Etot[0],identifier[0]);
  // Check if key exists. If it doesn't, create a new series and add to map
      if(mapping.count(key) == 0){
        std::vector<G4Entry> series;
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
    if(verbose){
      print_G4map(mapping);
    }    
    return mapping;
}

void print_DetSimMap(  std::map<std::tuple<int,int,int>, std::vector<DetSimEntry> > DetMapping){
    // Utility function to print contents of GramsDetSim
  for ( auto i = DetMapping.begin(); i != DetMapping.end(); ++i ){
    auto key = (*i).first;
    auto value = (*i).second;
    std::cout << " run=" << std::get<0>(key) << " event= " << std::get<1>(key) << " trackID= " << std::get<2>(key) << std::endl; 
    for (const auto& event : value){
      std::cout <<'\t'
      << " energy = " << std::get<0>(event)
      << " x = " << std::get<1>(event)
      << " y = " << std::get<2>(event)
      << " z = " << std::get<3>(event)
      << " t = " << std::get<4>(event)
      << std::endl;
    }
  }
}

void print_ExtractMap(std::map<std::tuple<int,int>, std::vector<ExtractEntry> > Mapping){
  // Utility function to print contents of Extract output
  for ( auto i = Mapping.begin(); i != Mapping.end(); ++i ){
    auto key = (*i).first;
    auto value = (*i).second;
    std::cout << " run=" << std::get<0>(key) << " event= " << std::get<1>(key) << std::endl; 
    for (const auto& event : value){
      std::cout <<'\t'
      << " energy = " << std::get<0>(event)
      << " x = " << std::get<1>(event)
      << " y = " << std::get<2>(event)
      << " z = " << std::get<3>(event)
      << " t = " << std::get<4>(event)
      << " DetEnergy = " << std::get<5>(event)
      << " tDet = " << std::get<6>(event)
      << " xDet = " << std::get<7>(event)
      << " yDet = " << std::get<8>(event)
      << " zDet = " << std::get<9>(event)
      << " SeriesType = " << std::get<10>(event)
      << std::endl;
    }
  }

}

void print_ReconstructDataFromSkyMap(std::map<std::tuple<int,int>, ReconstructEntry > Mapping){
  // Utility function to print contents of Reconstruct output
  for ( auto i = Mapping.begin(); i != Mapping.end(); ++i ){
    auto key = (*i).first;
    auto event = (*i).second;
    std::cout << " run=" << std::get<0>(key) << " event= " << std::get<1>(key) << std::endl; 
      std::cout <<'\t'
      << " xDir = " << std::get<0>(event)
      << " yDir = " << std::get<1>(event)
      << " zDir = " << std::get<2>(event)
      << " xTip = " << std::get<3>(event)
      << " yTip = " << std::get<4>(event)
      << " zTip = " << std::get<5>(event)
      << " RecoAngle = " << std::get<6>(event)
      << " Truth Energy =" << std::get<7>(event)
      << std::endl;
  }
}

std::map<std::tuple<int,int,int>, std::vector<DetSimEntry> > ReadGramsDetSim(std::string GramsDetSimFileName,bool verbose){
  // Open Relevant DetSim data
  // Format of DetSimMap
  ROOT::RDataFrame DetSim( "DetSim", GramsDetSimFileName, {"Run", "Event", "TrackID","energy",
    "x","y","z","timeAtAnode"} );

  std::map<std::tuple<int,int,int>, std::vector<DetSimEntry> > DetMapping;

  // Lambda expression to extract variables from detSim
  DetSim.Foreach(
    [&DetMapping]( int run, int event, int trackID,
  ROOT::VecOps::RVec<double> energy, ROOT::VecOps::RVec<double> x,ROOT::VecOps::RVec<double> y,
  ROOT::VecOps::RVec<double> z,ROOT::VecOps::RVec<double> t)
    {
  // Create key and value pair
      std::tuple<int,int,int> key = std::make_tuple(run,event,trackID);
  // Only read the first value of the vectors, since that is where the interaction occurred at
      DetSimEntry hit = std::make_tuple(energy[0],x[0],y[0],z[0],t[0]);
  // Check if key exists. If it doesn't, create a new series and add to map
      if(DetMapping.count(key) == 0){
      std::vector<DetSimEntry> series;
        series.push_back(hit);
        DetMapping[key] = series;
     }
      else{
  // If the series exists, add hit to series
        DetMapping[key].push_back(hit);
      }
    }, 
  {"Run", "Event", "TrackID","energy","x","y","z","timeAtAnode"}
            );// end of ForEach
  if(verbose){
    print_DetSimMap(DetMapping);
  }
  return DetMapping;
}

void push_to_map(std::tuple<int,int> &key, ExtractEntry &values, 
  std::map< std::tuple<int,int>, std::vector<ExtractEntry> > &Series){
  // Push key,value pair to map
    if(Series.count(key)==0){
        std::vector<ExtractEntry> Events;
        Events.push_back(values);
        Series[key] = Events;
    }
    else{
        Series[key].push_back(values);
    }
}

std::map<std::tuple<int,int>, std::vector<ExtractEntry> > ReadExtract(std::string ExtractFileName,bool verbose){
  // Read in output of Extract executable
  ROOT::RDataFrame Series( "FilteredSeries", ExtractFileName, {"Run", "Event", "energy","t",
    "x","y","z","tDet","xDet","yDet","zDet", "DetEnergy","SeriesType"} );
  
  std::map<std::tuple<int,int>, std::vector<ExtractEntry> > Output;

  Series.Foreach(
  // Shove data into map depending on series type
  [&Output](int run, int event, double energy, double t, double x, double y, double z,
  double tDet, double xDet, double yDet, double zDet, double DetEnergy, std::string SeriesType)
  {
    std::tuple<int,int> key = std::make_tuple(run,event);
    ExtractEntry observables;
    std::string EventType;
    for ( auto i = SeriesType.begin(); i != SeriesType.end() && (*i) != 0; ++i ){
      EventType.push_back(*i);
    }
    observables = std::make_tuple(energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet,EventType);
    push_to_map(key,observables,Output);
  },
  {"Run", "Event", "energy","t","x","y","z","tDet","xDet","yDet","zDet", "DetEnergy","SeriesType"} );
  if(verbose){
    print_ExtractMap(Output);
  }
  return Output;
}

std::map<std::tuple<int,int>, ReconstructEntry> ReadReconstructFromSkyMap(std::string ReconstructName, bool verbose){
  // Read on output of Reconstruct exe
  ROOT::RDataFrame Series( "Cones", ReconstructName, {"Run", "Event","EventType",
   "xDir","yDir","zDir",
   "xTip","yTip","zTip",
   "RecoAngle","ARM","RecoEnergy","TruthEnergy"
   } );

  std::map<std::tuple<int,int>, ReconstructEntry> Output;

  Series.Foreach(
  // Shove data into map depending on series type
  [&Output](int run, int event, int eventType, double xDir, double yDir, double zDir,
  double xTip, double yTip, double zTip, double RecoAngle, double ARM, double RecoEnergy, double TruthEnergy)
  {
    std::tuple<int,int> key = std::make_tuple(run,event);
    ReconstructEntry observables;
    observables = std::make_tuple(xDir,yDir,zDir,xTip,yTip,zTip,RecoAngle,TruthEnergy);
    auto pair = std::make_pair(key,observables);
    Output.insert(pair);
  },
  {"Run", "Event","EventType",
   "xDir","yDir","zDir",
   "xTip","yTip","zTip",
   "RecoAngle","ARM","RecoEnergy","TruthEnergy"} );
  if(verbose){
    print_ReconstructDataFromSkyMap(Output);
  }
  return Output;
}