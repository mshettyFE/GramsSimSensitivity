// Define the ROOT includes.
#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"

#include "GramsG4Entry.h"
#include "DetSimEntry.h"
#include "UsefulTypeDefs.h"

// The C++ includes.
#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <stdexcept>

bool WriteEntry(std::unique_ptr<GramsG4Entry>& G4, std::unique_ptr<GramsDetSimEntry>& DetSim, TTree* ntuple, bool verbose){
  // variables to be written
  EntryKey G4_key = G4->extract_key();
  EntryKey Det_key = DetSim->extract_key();
  if(G4_key[0] != Det_key[0]){
    throw std::invalid_argument("Keys don't match");
  }
  if(G4_key[1] != Det_key[1]){
    throw std::invalid_argument("Keys don't match");
  }
  int Run = G4->get_run();
  int Event = G4->get_event();
  int TrackID = G4->get_TrackID(); 
  int ParentID = G4->get_ParentID();
  int PDGCode = G4->get_PDGCode();
  double energy = G4->get_Energy();
  double t = G4->get_t();
  double x = G4->get_x();
  double y = G4->get_y();
  double z = G4->get_z();
  double tDet = DetSim->get_t();
  double xDet = DetSim->get_x();
  double yDet = DetSim->get_y();
  double zDet = DetSim->get_z();
  double DetEnergy = DetSim->get_Energy();
  int identifier = G4->get_ID();
  std::string series_type = G4->get_ProcessName();

  // Assign each variable to ntuple
  ntuple->Branch("Run", &Run);
  ntuple->Branch("Event", &Event);
  ntuple->Branch("TrackID", &TrackID);
  ntuple->Branch("ParentID", &ParentID);
  ntuple->Branch("PDGCode", &PDGCode);
  ntuple->Branch("energy", &energy);
  ntuple->Branch("t", &t);
  ntuple->Branch("x", &x);
  ntuple->Branch("y", &y);
  ntuple->Branch("z", &z);
  ntuple->Branch("tDet", &tDet);
  ntuple->Branch("xDet", &xDet);
  ntuple->Branch("yDet", &yDet);
  ntuple->Branch("zDet", &zDet);
  ntuple->Branch("DetEnergy", &DetEnergy);

  ntuple->Branch("identifier", &identifier);
  ntuple->Branch("SeriesType",&series_type);

  ntuple->Fill();

  if(verbose){
    G4->print();
    DetSim->print();    
  }
  return true;
}

std::vector<double> L2NormSquaredAdjacentScatters(std::vector<std::unique_ptr<GramsG4Entry>>& G4, std::vector<std::unique_ptr<GramsDetSimEntry>>& Det, bool MCTruth){
  // Function that calculates the L2NormSquared (ie. squared Euclidean distance) between adjacent scatters
  //Cone Variables: int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier
  std::vector<double> L2NormSquared;
  // We are assuming that the first "scatter" is a gamma ray, hence we skip that interaction
  if(MCTruth){
    for(long unsigned int i=1; i<(G4.size()-1);++i){
      double dx = G4[i]->get_x() - G4[i+1]->get_x();
      double dy = G4[i]->get_y() - G4[i+1]->get_y();
      double dz = G4[i]->get_z() - G4[i+1]->get_z();
      L2NormSquared.push_back(dx*dx+dy*dy+dz*dz);  
    }
  }
  else{
    for(long unsigned int i=1; i<(Det.size()-1);++i){
      double dx = Det[i]->get_x() - Det[i+1]->get_x();
      double dy = Det[i]->get_y() - Det[i+1]->get_y();
      double dz = Det[i]->get_z() - Det[i+1]->get_z();
      L2NormSquared.push_back(dx*dx+dy*dy+dz*dz);
    }
  }
  return L2NormSquared;
}

bool UniqueCellsOnePairMC(std::unique_ptr<GramsG4Entry>& first, std::unique_ptr<GramsG4Entry>& second,
std::vector<double> Dimensions, std::vector<int> Binnings){
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
  int first_bin = classifier->Fill(first->get_x(),first->get_y(),first->get_z());
  int second_bin = classifier->Fill(second->get_x(),second->get_y(),second->get_z());
  // clean up pointer
  delete classifier;
  return first_bin==second_bin;
}

bool UniqueCellsOnePairDet(std::unique_ptr<GramsDetSimEntry>& first, std::unique_ptr<GramsDetSimEntry>& second,
std::vector<double> Dimensions, std::vector<int> Binnings){
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
  int first_bin = classifier->Fill(first->get_x(),first->get_y(),first->get_z());
  int second_bin = classifier->Fill(second->get_x(),second->get_y(),second->get_z());
  // clean up pointer
  delete classifier;
  return first_bin==second_bin;
}


bool filter(std::vector<std::unique_ptr<GramsG4Entry>>& G4Series, std::vector<std::unique_ptr<GramsDetSimEntry>>& DetSimSeries, std::string SeriesType,
 std::string FilterType,  std::vector<double> Dimensions ,  std::vector<int> Binnings,  double seperation , bool MCTruth, bool verbose){
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
    std::unique_ptr<GramsG4Entry> event;
    if(verbose){
      for(long unsigned  int i =0; i<G4Series.size(); ++i){
        event = std::move(G4Series[i]);
        std::string PName = event->get_ProcessName();
        std::cout << PName << std::endl;
      }
    }
    for(long unsigned int i =0; i<G4Series.size(); ++i){
        event = std::move(G4Series[i]);
        std::string ProcessName = event->get_ProcessName();
  // If the event is not a primary gamma, a compton scatter, or a photoabsorbtion, reject series
        bool ProcessCheck = (ProcessName=="Primary") || (ProcessName == "compt") || (ProcessName == "phot");
        if (!ProcessCheck){
          return false;
        }
  // If the event is not a primary photon, check if it is inside the LAr. If it is not, the reject event
        if (ProcessName != "Primary"){
          // we divide by 1000000 since interactions in LAr have an identifier starting with 1
          if ( static_cast<int>( static_cast<float>( event->get_ID() ) /1000000.0)  !=1){
              return false;
          }
        }
  // Check if all events originated from primary gamma
      int ParentIdentity = event->get_ParentID();
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
        std::unique_ptr<GramsG4Entry> first = std::move(G4Series[index]);
        std::unique_ptr<GramsG4Entry> second = std::move(G4Series[index+1]);
        // If two adjacent scatters are not in unique cells, return false
        if(!UniqueCellsOnePairMC(first,second,Dimensions,Binnings)){
          return false;
        }
      }
    }
    else{
      for(long unsigned int index=1; index <G4Series.size()-1; ++index){
        std::unique_ptr<GramsDetSimEntry> first = std::move(DetSimSeries[index]);
        std::unique_ptr<GramsDetSimEntry> second = std::move(DetSimSeries[index+1]);
        // If two adjacent scatters are not in unique cells, return false
        if(!UniqueCellsOnePairDet(first,second,Dimensions,Binnings)){
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

void escape(std::vector<std::unique_ptr<GramsG4Entry>>& series, std::string &outParam){
  // int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // Assuming the events are time sorted, we see if last event was a photoabsorbtion
  // Each series is gaurenteed to be at least 1 (since the gamma ray is part of the series)
    if (series[series.size()-1]->get_ProcessName()  == "phot"){
        outParam =  std::string("AllIn");
    }
    else{
        outParam = std::string("Escape");
    }
}

void FilterWrite(std::map<EntryKey, std::vector<std::unique_ptr<GramsG4Entry>>>& GramsG4Map,
  std::map<EntryKey, std::vector<std::unique_ptr<GramsDetSimEntry>>>& GramsDetSimMap,
  std::string outputRootFile, std::string FilterType,
  std::vector<double> Dimensions,  std::vector<int> Binnings, double seperation,  bool MCTruth, bool verbose){
  // Create output file
  TFile* output = new TFile(outputRootFile.c_str(), "RECREATE");
  TTree* ntuple = new TTree("FilteredSeries","Compton scatter series");

  std::string series_type;

  // Check if a series is easy to analyze
  for (auto i= GramsG4Map.begin(); i != GramsG4Map.end(); ++i){
    EntryKey key = i->first;
    std::vector<std::unique_ptr<GramsG4Entry>> value = std::move(i->second);
  // Sort events by time
    std::sort(value.begin(),value.end(),[](
    std::unique_ptr<GramsG4Entry>& a, 
    std::unique_ptr<GramsG4Entry>& b){
    return (a->get_t() < b->get_t());
    });

  // Figures out wheather the series is escape or all in
    escape(value,series_type);
    std::vector<std::unique_ptr<GramsDetSimEntry>> AssociatedEntry = std::move(GramsDetSimMap[key]);
    bool keep = filter(value, AssociatedEntry, series_type, FilterType, Dimensions, Binnings,seperation, MCTruth, verbose);
  // Keep easy series
    if(keep){
  // Extract all the  variables and write out to ntuple
        std::sort(AssociatedEntry.begin(),AssociatedEntry.end(),[](
          std::unique_ptr<GramsDetSimEntry>& a,
          std::unique_ptr<GramsDetSimEntry>& b){
          return (a->get_t() < b->get_t());
        });
      for(long unsigned int j=0; j<value.size(); j++){
        WriteEntry(value[j], AssociatedEntry[j], ntuple, verbose);
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