// Define the ROOT includes.
#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"

#include "GramsG4Entry.h"
#include "DetSimEntry.h"
#include "ExtractionEntry.h"
#include "UsefulTypeDefs.h"

// The C++ includes.
#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <stdexcept>

/*
void FillEntry(TTree* ntuple, GramsExtractEntry& entry, bool verbose){
  if(verbose){
    entry.print();
  }
  ntuple->Fill();
}
*/

std::vector<double> L2NormSquaredAdjacentScatters(const std::vector<GramsG4Entry>& G4, const std::vector<GramsDetSimEntry>& Det, bool MCTruth){
  // Function that calculates the L2NormSquared (ie. squared Euclidean distance) between adjacent scatters
  //Cone Variables: int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier
  std::vector<double> L2NormSquared;
  // We are assuming that the first "scatter" is a gamma ray, hence we skip that interaction
  if(MCTruth){
    for(long unsigned int i=1; i<(G4.size()-1);++i){
      double dx = G4[i].get_x() - G4[i+1].get_x();
      double dy = G4[i].get_y() - G4[i+1].get_y();
      double dz = G4[i].get_z() - G4[i+1].get_z();
      L2NormSquared.push_back(dx*dx+dy*dy+dz*dz);  
    }
  }
  else{
    for(long unsigned int i=0; i<(Det.size()-1);++i){
      double dx = Det[i].get_x() - Det[i+1].get_x();
      double dy = Det[i].get_y() - Det[i+1].get_y();
      double dz = Det[i].get_z() - Det[i+1].get_z();
      L2NormSquared.push_back(dx*dx+dy*dy+dz*dz);
    }
  }
  return L2NormSquared;
}

bool UniqueCellsOnePairMC(const GramsG4Entry& first, const GramsG4Entry& second, const std::vector<double> Dimensions, const  std::vector<int> Binnings){
  //Cone variables: int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // create a histogram that aligns with the coordinate system of TPc (ie. x and y origin at center of detector. z=0 at top of detector)
    // x dimension ranges from -x_dim/2 to x_dim/2
    // y dimension from -y_dim/2 to y_dim/2
    // z dimension from -z_dim to 0
  TH3D *classifier = new TH3D("class", "doesn't matter",
   Binnings[0], -Dimensions[0]/2.0, Dimensions[0]/2.0,
   Binnings[1], -Dimensions[1]/2.0, Dimensions[1]/2.0,
   Binnings[2], -Dimensions[2], Dimensions[2]);
  // Bin the first and second scatters and get the associated bin numbers
  // (there might be a bug if you exceed the histogram limits, but that shouldn't happen... I hope)
  int first_bin = classifier->Fill(first.get_x(),first.get_y(),first.get_z());
  int second_bin = classifier->Fill(second.get_x(),second.get_y(),second.get_z());
  // clean up pointer
  delete classifier;
  return first_bin==second_bin;
}

bool UniqueCellsOnePairDet(const GramsDetSimEntry& first, const GramsDetSimEntry& second, const std::vector<double> Dimensions, const std::vector<int> Binnings){
  //Cone variables: int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // create a histogram that aligns with the coordinate system of TPc (ie. x and y origin at center of detector. z=0 at top of detector)
    // x dimension ranges from -x_dim/2 to x_dim/2
    // y dimension from -y_dim/2 to y_dim/2
    // z dimension from -z_dim to 0
  TH3D *classifier = new TH3D("class", "doesn't matter",
   Binnings[0], -Dimensions[0]/2.0, Dimensions[0]/2.0,
   Binnings[1], -Dimensions[1]/2.0, Dimensions[1]/2.0,
   Binnings[2], -Dimensions[2], Dimensions[2]);
  // Bin the first and second scatters and get the associated bin numbers
  // (there might be a bug if you exceed the histogram limits, but that shouldn't happen... I hope)
  int first_bin = classifier->Fill(first.get_x(),first.get_y(),first.get_z());
  int second_bin = classifier->Fill(second.get_x(),second.get_y(),second.get_z());
  // clean up pointer
  delete classifier;
  return first_bin==second_bin;
}


bool filter(const std::vector<GramsG4Entry>& G4Series, const std::vector<GramsDetSimEntry>& DetSimSeries,
  const std::string SeriesType,const std::string FilterType, 
  const std::vector<double> Dimensions ,  const std::vector<int> Binnings, 
  const double seperation  , const bool MCTruth, const bool verbose){
  // variables in series corresponds to the following:
    //int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // Function that sorts through events and selects Compton series to be analyzed
  // This means at  all non-gamma ray events are inside detector, all events inside TPC are comptons and photoabsorbitons, and all events came from primary (ie. no showers)
  // Assume that events are sorted in order of increasing time
  // Checks for at least 3 events (Primary, first event, second event). Need at least 2 events to perform reconstruction
  // FilterType can add on additional filtering.
    // Standard adds no additional filtering criterion
    // UniqueCells stipulates that each scatter must occur in a unique cell (ie. each scatter happens in an optically isolated region)
    // Sphere denotes that the distance between each scatter must be at least seperation cm apart
    GramsG4Entry event;
    if(verbose){
      for(long unsigned  int i =0; i<G4Series.size(); ++i){
        std::cout << i << '\t' <<  G4Series[i].get_ProcessName() << std::endl;
      }
    }
    for(long unsigned int i =0; i<G4Series.size(); ++i){
        std::string ProcessName = G4Series[i].get_ProcessName();
  // If the event is not a primary gamma, a compton scatter, or a photoabsorbtion, reject series
        bool ProcessCheck = (ProcessName=="Primary") || (ProcessName == "compt") || (ProcessName == "phot");
        if (!ProcessCheck){
          return false;
        }
  // If the event is not a primary photon, check if it is inside the LAr. If it is not, the reject event
        if (ProcessName != "Primary"){
          // we divide by 1000000 since interactions in LAr have an identifier starting with 1
          if ( static_cast<int>( static_cast<float>( G4Series[i].get_ID() ) /1000000.0)  !=1){
              return false;
          }
        }
  // Check if all events originated from primary gamma
      int ParentIdentity = G4Series[i].get_ParentID();
      bool PaternityTest = (ParentIdentity ==1 || ParentIdentity ==0); 
      if(!PaternityTest){
        return false;
      }
    }
    
  if(SeriesType=="Escape"){
    // Need 3 non-gamma events
    if(G4Series.size()<4){
      return false;
    }
  }
  else if(SeriesType=="AllIn"){
    // Need 2 non gamma events
    if(G4Series.size()<3){
      return false;
    }
  }
  else{
    // SHouldn't happen, but just in case
    std::cerr << "Invalid Series Type" << std::endl;
    return false;
  }

  if(FilterType=="Standard"){
    // No additional filters
    return true;
  }

  else if(FilterType=="Sphere"){
    std::vector<double> L2NormSquaredDistances = L2NormSquaredAdjacentScatters(G4Series, DetSimSeries, MCTruth);
    for(long unsigned int index=0; index <L2NormSquaredDistances.size(); ++index){
    // check if distance b/w scatters is at least seperation
    // Actually calculate distance squared so that we don't need to take square roots
      if(L2NormSquaredDistances[index]<(seperation*seperation)){
        return false;
      }
    }
    return true;
  }

  else if(FilterType=="UniqueCells"){
    // check for unique, optically isolated cells
    // Ignore the first hit since it is a gamma ray.
    if(MCTruth){
      for(long unsigned int index=1; index <G4Series.size()-1; ++index){
        // If two adjacent scatters are not in unique cells, return false
        if(!UniqueCellsOnePairMC(G4Series[index],G4Series[index+1],Dimensions,Binnings)){
          return false;
        }
      }
    }
    else{
      // for detSim, photon is not included, so start from 0
      for(long unsigned int index=0; index <DetSimSeries.size()-1; ++index){
        // If two adjacent scatters are not in unique cells, return false
        if(!UniqueCellsOnePairDet(DetSimSeries[index],DetSimSeries[index+1],Dimensions,Binnings)){
          return false;
        }
      }
    }
    // All the scatters are in unique cells
    return true;
  }
  else{
  // Defaults to "Standard" if somehow, you get an invalid filter value
    return true;
  }
}

void escape(const std::vector<GramsG4Entry>& series, std::string &outParam){
  // int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // Assuming the events are time sorted, we see if last event was a photoabsorbtion
  // Each series is gaurenteed to be at least 1 (since the gamma ray is part of the series)
    if (series[series.size()-1].get_ProcessName()  == "phot"){
        outParam =  std::string("AllIn");
    }
    else{
        outParam = std::string("Escape");
    }
}

void FilterWrite(const std::map<EntryKey, std::vector<GramsG4Entry>>& GramsG4Map,
  const std::map<EntryKey, std::vector<GramsDetSimEntry>> & GramsDetSimMap,
  const std::string outputRootFile, const std::string FilterType,
  const std::vector<double> Dimensions,  const std::vector<int> Binnings,
  const double seperation,  const bool MCTruth,  const bool verbose){

  // Create output file
  TFile* output = new TFile(outputRootFile.c_str(), "RECREATE");
  TTree* ntuple = new TTree("FilteredSeries","Compton scatter series");
  int Run, Event;
  double t,x,y,z,energy;
  double tDet, xDet, yDet, zDet, DetEnergy;
  std::string SeriesType;
  GramsExtractEntry OutputEntry = GramsExtractEntry();

// TODO: somehow use std::vector<std::variant> and DumpData() method in Entry to pass values in class to local variables above

  ntuple->Branch("Run", &Run,"Run/I");
  ntuple->Branch("Event", &Event,"Event/I");
  ntuple->Branch("t", &t, "t/D");
  ntuple->Branch("x", &x, "x/D");
  ntuple->Branch("y", &y, "y/D");
  ntuple->Branch("z", &z, "z/D");
  ntuple->Branch("energy", &energy, "energy/D");
  ntuple->Branch("tDet", &tDet, "tDet/D");
  ntuple->Branch("xDet", &xDet, "xDet/D");
  ntuple->Branch("yDet", &yDet, "yDet/D");
  ntuple->Branch("zDet", &zDet,"zDet/D");
  ntuple->Branch("DetEnergy", &DetEnergy, "DetEnergy/D");
  ntuple->Branch("SeriesType",&SeriesType);

  // Check if a series is easy to analyze
  for (auto i= GramsG4Map.begin(); i != GramsG4Map.end(); ++i){
    std::vector<GramsG4Entry> value = i->second;
  // Sort events by time
    std::sort(value.begin(),value.end(),[](
    GramsG4Entry& a, 
    GramsG4Entry& b){
    return (a.get_t() < b.get_t());
    });

  // Figures out wheather the series is escape or all in
    EntryKey key = {value[0].get_run(), value[0].get_event(), value[0].get_TrackID()};
    escape(value,SeriesType);
    std::vector<GramsDetSimEntry> associated_comptons = GramsDetSimMap.at(key);
    bool keep = filter(value, associated_comptons, SeriesType, FilterType, Dimensions, Binnings,seperation, MCTruth, verbose);
  // Keep easy series
    if(keep){
      if(associated_comptons.size() == 0){
        throw std::invalid_argument("No Compton Scatters found in GramsDetSim for entry");
      }
  // Sort DetSim to match with G4 variables
      std::sort(associated_comptons.begin(),associated_comptons.end(),[](
        GramsDetSimEntry& a,
        GramsDetSimEntry& b){
        return (a.get_t() < b.get_t());
      });

  // treat first entry (photon) differently from other scatters since Photon doesn't interact with DetSim
      GramsG4Entry photon_G4 =  value[0];
      GramsDetSimEntry photon_DetSim = GramsDetSimEntry(photon_G4.get_run(), photon_G4.get_event(),photon_G4.get_TrackID(),
         photon_G4.get_Energy(), photon_G4.get_x(),photon_G4.get_y(),photon_G4.get_z(), photon_G4.get_t());
      OutputEntry = GramsExtractEntry(photon_G4, photon_DetSim, SeriesType);
//      FillEntry(ntuple, OutputEntry,verbose);
    // Write out rest of variables
      for(long unsigned int j=0; j<associated_comptons.size(); j++){
        OutputEntry = GramsExtractEntry(value[j+1],associated_comptons[j], SeriesType);
//        FillEntry(ntuple, OutputEntry,verbose);
      }
    }
  }

  // Each series is identified by the Run and Event number. You can sort by time to organize the series
  // The first hit is the Primary Photon, the intermediate steps are Compton scatters
  // The final event is either a photoabsorbtion (if the escape flag is ) or a compton scatter (if the flag is E) 
  ntuple->Write();
  delete ntuple;
  output->Close();
  }