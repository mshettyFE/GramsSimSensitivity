// Define the ROOT includes.
#include <ROOT/RDataFrame.hxx>
#include <TFile.h>
#include <TTree.h>
#include <TH3D.h>


// The C++ includes.
#include <vector>
#include <map>
#include <cstring>
#include <iostream>

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

std::vector<double> L2NormSquaredAdjacentScatters(std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> Series){
  // Function that calculates the L2NormSquared (ie. squared Euclidean distance) between adjacent scatters
  //int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier
  std::vector<double> L2NormSquared;
  // We are assuming that the first "scatter" is a gamma ray, hence we skip that interaction
  for(int i=1; i<(Series.size()-1);++i){
    double dx = std::get<5>(Series[i])-std::get<5>(Series[i+1]);
    double dy = std::get<6>(Series[i])-std::get<6>(Series[i+1]);
    double dz = std::get<7>(Series[i])-std::get<7>(Series[i+1]);
    L2NormSquared.push_back(dx*dx+dy*dy+dz*dz);
  }
  return L2NormSquared;
}

bool UniqueCellsOnePair(std::tuple<int,int,int,std::string,double,float,float,float,double,int> first, std::tuple<int,int,int,std::string,double,float,float,float,double,int> second,
std::vector<double> Dimensions, std::vector<int> Binnings){
  //int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
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
  int first_bin = classifier->Fill(std::get<5>(first),std::get<6>(first),std::get<7>(first));
  int second_bin = classifier->Fill(std::get<5>(second),std::get<6>(second),std::get<7>(second));
  // clean up pointer
  delete classifier;
  return first_bin==second_bin;
}

bool filter(std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> series, std::string SeriesType,
 std::string FilterType,  std::vector<double> Dimensions ,  std::vector<int> Binnings,  double seperation , bool verbose){
  // variables in series corresponds to the following:
    //int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // Function that sorts through events and selects easy to analyze Compton series
  // This means at least 1 non gamma ray event, wheather all events are inside detector, wheather all events are comptons and photoabsorbitons, and wheather all events came from primary (ie. no showers)
  // Assume that events are sorted in order of increasing time
  // Checks for at least 3 events (Primary, first event, second event). Need at least 2 events to perform reconstruction
  // FilterType can add on additional filtering.
    // Standard adds no additional filtering criterion
    // UniqueCells stipulates that each scatter must occur in a unique cell (ie. each scatter happens in an optically isolated region)
    // Sphere denotes that the distance between each scatter must be at least seperation cm apart
    if(verbose){
      for(int i =0; i<series.size(); ++i){
        auto &event = series[i];
        std::string PName = std::get<3>(event);
        std::cout << PName << '\t';
      }
      std::cout << std::endl;
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
    
  if(SeriesType=="Escape"){
    if(series.size()<4){
      return false;
    }
  }
  else if(SeriesType=="AllIn"){
    if(series.size()<3){
      return false;
    }
  }
  else{
    std::cerr << "Invalid Series Type" << std::endl;
    return false;
  }

  if(FilterType=="Standard"){
    // No additional filters
    return true;
  }

  else if(FilterType=="Sphere"){
    std::vector<double> L2NormSquaredDistances = L2NormSquaredAdjacentScatters(series);
    for(int index=0; index <L2NormSquaredDistances.size(); ++index){
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
    for(int index=1; index <series.size()-1; ++index){
      std::tuple<int,int,int,std::string,double,float,float,float,double,int> first = series[index];
      std::tuple<int,int,int,std::string,double,float,float,float,double,int> second = series[index+1];
      // If two adjacent scatters are not in unique cells, return false
      if(!UniqueCellsOnePair(first,second,Dimensions,Binnings)){
        return false;
      }
    }
    // All the sccatters are in unique cells
    return true;
  }

  else{
  // Defaults to "Standard" if somehow, you get an invalid filter value
    return true;
  }
}

void escape(std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> series, std::string &outParam){
  // int trackID,int ParentID,int PDGCode,string processName, double t, float x,float y,float z, double Etot, int identifier)
  // Assuming the events are time sorted, we see if last event was a photoabsorbtion
  // Each series is gaurenteed to be at least 1 (since the gamma ray is part of the series)
    if (std::get<3>(series.at(series.size()-1)) == "phot"){
        outParam =  std::string("AllIn");
    }
    else{
        outParam = std::string("Escape");
    }
}

void FilterWrite(std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > GramsG4Map,
  std::map<std::tuple<int,int,int>, std::vector<std::tuple<double,double, double,double,double>> > GramsDetSimMap,
  std::string outputRootFile, std::string FilterType,
  std::vector<double> Dimensions,  std::vector<int> Binnings, double seperation,  bool verbose){
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
  // Figures out wheather the series is escape or all in
      escape(value,series_type);
    bool keep = filter(value, series_type, FilterType, Dimensions, Binnings,seperation, verbose);
  // Keep easy series
    if(keep){
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