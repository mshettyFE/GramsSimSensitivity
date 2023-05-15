// Define the ROOT includes.
#include <ROOT/RDataFrame.hxx>
#include <TFile.h>
#include <TTree.h>

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


std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > ReadGramsG4(std::string GramsG4FileName, bool verbose){
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
  // Format of map
  // <Run, Event> : [<TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier>,...]
    std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping;

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
    if(verbose){
      print_G4map(mapping);
    }    
    return mapping;
}

void print_DetSimMap(  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > DetMapping){
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

std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > ReadGramsDetSim(std::string GramsDetSimFileName,bool verbose){
  // DetSim Leafs
  /*
  *Br    0 :Run       : Run/I                                                  *
  *Br    1 :Event     : Event/I                                                *
  *Br    2 :TrackID   : TrackID/I                                              *
  *Br    3 :PDGCode   : PDGCode/I                                              *
  *Br    4 :numPhotons : numPhotons/I                                 *
  *Br    5 :energy    : vector<double>                                         *
  *Br    6 :numElectrons : vector<double>                                      *
  *Br    7 :x         : vector<double>                                         *
  *Br    8 :y         : vector<double>                                         *
  *Br    9 :z         : vector<double>                                         *
  *Br   10 :timeAtAnode : vector<double>                                       *
  *Br   11 :identifier : identifier/I                                          *
  */

  // Open Relevant DetSim data
  // Format of DetSimMap
  ROOT::RDataFrame DetSim( "DetSim", GramsDetSimFileName, {"Run", "Event", "TrackID","energy",
    "x","y","z","timeAtAnode"} );

  // <Run, Event,TrackID> : [<energy,x,y,z,timeAtAnode>,...]
  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > DetMapping;

  // Lambda expression to extract variables from detSim
  DetSim.Foreach(
    [&DetMapping]( int run, int event, int trackID,
  ROOT::VecOps::RVec<double> energy, ROOT::VecOps::RVec<double> x,ROOT::VecOps::RVec<double> y,
  ROOT::VecOps::RVec<double> z,ROOT::VecOps::RVec<double> t)
    {
  // Create key and value pair
      std::tuple<int,int,int> key = std::make_tuple(run,event,trackID);
  // Only read the first value of the vectors, since that is where the interaction occurred at
      std::tuple<double,double, double,double,double> hit = std::make_tuple(energy[0],x[0],y[0],z[0],t[0]);
  // Check if key exists. If it doesn't, create a new series and add to map
      if(DetMapping.count(key) == 0){
      std::vector<std::tuple<double,double, double,double,double>> series;
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

void SortEvents(std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> &Events){
  // Sorts events by time for easier processing later (ie. 5th argument)
    sort(Events.begin(),Events.end(),[](
      std::tuple<int,int,int,std::string,double,float,float,float,double,int> a, 
      std::tuple<int,int,int,std::string,double,float,float,float,double,int> b){
      return (std::get<4>(a) < std::get<4>(b));
    });
}

void SortDetEvents(std::vector<std::tuple<double,double,double,double,double>> &Events){
  // Sorts events by time for easier processing later (ie. 5th argument)
    sort(Events.begin(),Events.end(),[](
      std::tuple<double,double,double,double,double> a,
       std::tuple<double,double,double,double,double> b){
      return (std::get<4>(a) < std::get<4>(b));
    });
}

bool filter(std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> series, bool verbose){
  // int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // Function that sorts through events and selects easy to analyze Compton series
  // This means at least 1 non gamma ray event, wheather all events are inside detector, wheather all events are comptons and photoabsorbitons, and wheather all events came from primary (ie. no showers)
  // Assume that events are sorted in order of increasing time
  // Checks for at least 3 events (Primary, first event, second event). Need at least 2 events to perform reconstruction
    if(verbose){
      for(int i =0; i<series.size(); ++i){
        auto &event = series[i];
        std::string PName = std::get<3>(event);
        std::cout << PName << '\t';
      }
      std::cout << std::endl;
    }
    if(series.size()<3){
      return false;
    }
    for(int i =0; i<series.size(); ++i){
        auto &event = series[i];
        std::string ProcessName = std::get<3>(event);
  // If the event is not a primary gamma, a compton scatter, or a photoabsorbtion, reject series
        bool ProcessCheck = (ProcessName=="Primary") || (ProcessName == "compt") || (ProcessName == "phot");
        if (!ProcessCheck){
          return false;
        }
  // If the event is not a primary photon, check if it is inside the LAr. If it is not, the reject event
        if (ProcessName != "Primary"){
          if ( static_cast<int>( static_cast<float>( std::get<9>(event) ) /1000000.0)  !=1){
              return false;
          }
        }
  // Check if all events originated from primary gamma
      int ParentIdentity = std::get<1>(event);
      bool PaternityTest = (ParentIdentity ==1 || ParentIdentity ==0); 
      if(!PaternityTest){
        return false;
      }
    }
  return true;
}

void escape(std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> series, std::string &outParam){
  // int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // Assuming the events are time sorted, we see if last event was a photoabsorbtion
    if (std::get<3>(series.at(series.size()-1)) == "phot"){
        outParam =  std::string("AllIn");
    }
    else{
        outParam = std::string("Escape");
    }
}

void FilterWrite(std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > GramsG4Map,
  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > GramsDetSimMap,
  std::string outputRootFile,bool verbose){
  // Create output file
  TFile* output = new TFile(outputRootFile.c_str(), "RECREATE");
  TTree* ntuple = new TTree("FilteredSeries","Compton scatter series");

  // variables to be written
  int Run;
  int Event;
  int TrackID;
  int ParentID;
  int PDGCode;
  double energy;
  double t;
  double x;
  double y;
  double z;
  double tDet;
  double xDet;
  double yDet;
  double zDet;
  double DetEnergy;
  int identifier;
  std::string series_type;

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

  // Check if a series is easy to analyze
  for (auto i= GramsG4Map.begin(); i != GramsG4Map.end(); ++i){
    auto key = (*i).first;
    auto value = (*i).second;
  // Sort events by time
    SortEvents(value);
    bool keep = filter(value,verbose);
  // Keep easy series
    if(keep){
  // Figures out wheather the series is escape or all in
      escape(value,series_type);
  // Extract all the  variables and write out to ntuple
      for (const auto& event : value){
        Run = std::get<0>(key);
        Event = std::get<1>(key);
        TrackID = std::get<0>(event);
        ParentID = std::get<1>(event);
        PDGCode = std::get<2>(event);
        t = std::get<4>(event);
        x=  std::get<5>(event);
        y=  std::get<6>(event);
        z=  std::get<7>(event);
        energy=  std::get<8>(event);
        identifier =  std::get<9>(event);
  // Find associated data in GramsDetSim file and extract x,y,z,t values 
        std::tuple<int,int,int> eventIdentifier = std::make_tuple(Run,Event,TrackID);
        if(GramsDetSimMap.count(eventIdentifier) != 0){
            // Sort events by time to match. Take the first one since gamma rays not included in DetSim file
            SortDetEvents(GramsDetSimMap[eventIdentifier]);
            std::tuple<double,double,double,double,double> DetEvent = GramsDetSimMap[eventIdentifier][0];
            DetEnergy = std::get<0>(DetEvent);
            xDet = std::get<1>(DetEvent);
            yDet = std::get<2>(DetEvent);
            zDet = std::get<3>(DetEvent);
            tDet = std::get<4>(DetEvent);
        }
        else{
  // This should never happen, but if it does, then you write nonsensical values to indicate something went wrong
            DetEnergy = -1000;
            xDet = -1000;
            yDet = -1000;
            zDet = -1000;
            tDet = -1000;
        }

        if(verbose){
          std::cout << "Run " << Run
          << " Event " << Event
          << " TrackID " << TrackID
          << " ParentID " << ParentID
          << " PDGCode " << PDGCode
          << " t= " << t
          << " x= " << x
          << " y= " << y
          << " z= " << z
          << " Etot= " << energy
          << " tDet= " << tDet
          << " xDet= " << xDet
          << " yDet= " << yDet
          << " zDet= " << zDet
          << " EDet= " << DetEnergy
          << " identifier= " << identifier
          << " event_type= " << series_type
          << std::endl;
        }
        ntuple->Fill();
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