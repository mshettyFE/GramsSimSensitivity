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
#include <memory>

#include "Entry.h"
#include "GramsG4Entry.h"
#include "DetSimEntry.h"
#include "ExtractionEntry.h"
#include "RecoEntry.h"
#include "UsefulTypeDefs.h"

template <typename E>
int push_to_map(const EntryKey &key, const E values, std::map< EntryKey, std::vector<E> > &Series){
  // Push key,value pair to map
    if(Series.count(key)==0){
        std::vector<E> Events;
        Events.push_back(values);
        Series[key] = Events;
    }
    else{
        Series[key].push_back(values);
    }
  return 0;
}

std::map<EntryKey, std::vector<GramsG4Entry> > ReadGramsG4(const std::string GramsG4FileName, bool verbose){
  // See type defs for G4Entry
  // Uses <run,event> as key, and a list of interactions as the value
  std::map<EntryKey, std::vector<GramsG4Entry> > mapping;

  // Open Input data
  ROOT::RDataFrame trackInfo( "TrackInfo", GramsG4FileName, {"Run", "Event", "TrackID","ParentID","PDGCode","ProcessName","t",
    "x","y","z","Etot","identifier"} );

  // Lambda expression to extract variables from input
  trackInfo.Foreach(
    [&mapping,verbose]( int run, int event, int trackID, int ParentID, int PDGCode,
  ROOT::VecOps::RVec<char> process,ROOT::VecOps::RVec<double> t, ROOT::VecOps::RVec<float> x,ROOT::VecOps::RVec<float> y,
  ROOT::VecOps::RVec<float> z,ROOT::VecOps::RVec<double> Etot, ROOT::VecOps::RVec<int> identifier)
    {
      // Read in processName
      std::string processName;
      for ( auto i = process.begin(); i != process.end() && (*i) != 0; ++i ){
        processName.push_back(*i);
      }
      // Only read the first value of the vectors, since that is were the interaction occurred at
      GramsG4Entry hit(run, event, trackID,ParentID,PDGCode,processName, t[0],x[0],y[0],z[0],Etot[0],identifier[0]);
      EntryKey key = hit.extract_key();
      // push key to map. If key is the first one, add a new entry to the map
      push_to_map<GramsG4Entry>(key, hit, mapping);
      if(verbose){
        hit.print();
      }
    }, 
    {"Run", "Event", "TrackID","ParentID","PDGCode","ProcessName","t","x","y","z","Etot","identifier"}
  );// end of ForEach
    return mapping;
}

std::map<EntryKey, std::vector<GramsDetSimEntry> > ReadGramsDetSim(const std::string GramsDetSimFileName,bool verbose){
  // Open Relevant DetSim data
  // Format of DetSimMap
  ROOT::RDataFrame DetSim( "DetSim", GramsDetSimFileName, {"Run", "Event", "TrackID","energy",
    "x","y","z","timeAtAnode"} );

  std::map<EntryKey, std::vector<GramsDetSimEntry > >  DetMapping;

  // Lambda expression to extract variables from detSim
  DetSim.Foreach(
    [&DetMapping,verbose]( int run, int event, int trackID,
    ROOT::VecOps::RVec<double> energy, ROOT::VecOps::RVec<double> x,ROOT::VecOps::RVec<double> y,
    ROOT::VecOps::RVec<double> z,ROOT::VecOps::RVec<double> t)
    {
      GramsDetSimEntry hit = GramsDetSimEntry(run,event, trackID, energy[0], x[0], y[0], z[0], t[0]);

      // Create key and value pair
      EntryKey key = hit.extract_key();
      push_to_map(key, hit, DetMapping);
      if(verbose){
        hit.print();
      }
    }, 
    {"Run", "Event", "TrackID","energy","x","y","z","timeAtAnode"}
  );// end of ForEach
  return DetMapping;
}

std::map<EntryKey, std::vector<GramsExtractEntry> > ReadExtract(const std::string ExtractFileName, bool MCTruth, bool verbose){
  // Read in output of Extract executable
  ROOT::RDataFrame Series( "FilteredSeries", ExtractFileName, {"Run", "Event", "energy","t",
    "x","y","z","tDet","xDet","yDet","zDet", "DetEnergy","SeriesType"} );
  
  std::map<EntryKey, std::vector<GramsExtractEntry > >  Output;

  Series.Foreach(
  // Shove data into map depending on series type
  [&Output,MCTruth, verbose](int run, int event, double energy, double t, double x, double y, double z,
  double tDet, double xDet, double yDet, double zDet, double DetEnergy, std::string SeriesType)
  {
    std::string EventType;
    for ( auto i = SeriesType.begin(); i != SeriesType.end() && (*i) != 0; ++i ){
      EventType.push_back(*i);
    }
    GramsExtractEntry observables = GramsExtractEntry(MCTruth, run, event,  t,x,y,z,energy,  tDet, xDet, yDet, zDet, DetEnergy, EventType);
    EntryKey key = observables.extract_key();
    push_to_map(key,observables,Output);
      if(verbose){
        observables.print();
      }
  },
  {"Run", "Event", "energy","t","x","y","z","tDet","xDet","yDet","zDet", "DetEnergy","SeriesType"} );
  return Output;
}

std::map<EntryKey, GramsRecoEntry> ReadReconstructFromSkyMap(const std::string ReconstructName, bool verbose){
  // Read on output of Reconstruct exe
  ROOT::RDataFrame Series( "Cones", ReconstructName, 
  {"Run", "Event","EventType",
   "xDir","yDir","zDir",
   "xTip","yTip","zTip",
   "RecoAngle","ARM","RecoEnergy","TruthEnergy"
   } );

  std::map<EntryKey, GramsRecoEntry >  Output;

  Series.Foreach(
  // Shove data into map depending on series type
  [&Output,verbose](int run, int event, int eventType, double xDir, double yDir, double zDir,
  double xTip, double yTip, double zTip, double RecoAngle, double ARM, double RecoEnergy, double TruthEnergy)
  {
    const GramsRecoEntry observables  = GramsRecoEntry(run, event, eventType, xDir, yDir, zDir, xTip, yTip, zTip, RecoAngle,ARM, RecoEnergy, TruthEnergy);
    EntryKey key = observables.extract_key();
    Output[key] = observables;
    if(verbose){
      observables.print();
    }
 },
  {"Run", "Event","EventType",
   "xDir","yDir","zDir",
   "xTip","yTip","zTip",
   "RecoAngle","ARM","RecoEnergy","TruthEnergy"} );
  return Output;
}