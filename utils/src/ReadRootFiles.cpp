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

void print_ExtractMap(std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double>> > Mapping){
  // Format of map
  // <Run,Event>, < <energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet>,  ...>
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
      << std::endl;
    }
  }

}

void print_ExtractMap(std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > Mapping){
  // Format of map
  // <Run,Event>, < <energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet>,  ...>
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

void push_to_map(std::tuple<int,int> &key, std::tuple<double,double,double,double,double,double,double,double,double,double> &values, 
  std::map< std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double>> > &Series){
  // Push key,value pair to map
    if(Series.count(key)==0){
        std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double>> Events;
        Events.push_back(values);
        Series[key] = Events;
    }
    else{
        Series[key].push_back(values);
    }
}

void push_to_map(std::tuple<int,int> &key, std::tuple<double,double,double,double,double,double,double,double,double,double,std::string> &values, 
  std::map< std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > &Series){
  // Push key,value pair to map
    if(Series.count(key)==0){
        std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> Events;
        Events.push_back(values);
        Series[key] = Events;
    }
    else{
        Series[key].push_back(values);
    }
}


int ReadExtract(std::string ExtractFileName, 
    std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double>> > &AllInSeries,
    std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double>> > &EscapeSeries,
    bool verbose){
  // Format of AllIn and Escape series
  // <Run,Event>, < <energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet>,  ...>
  /*
  Branches of FilteredSeries  
  *Br    0 :Run       : Run/I                                                  *
  *Br    1 :Event     : Event/I                                                *
  *Br    2 :TrackID   : TrackID/I                                              *
  *Br    3 :ParentID  : ParentID/I                                             *
  *Br    4 :PDGCode   : PDGCode/I                                              *
  *Br    5 :energy    : energy/D                                               *
  *Br    6 :t         : t/D                                                    *
  *Br    7 :x         : x/D                                                    *
  *Br    8 :y         : y/D                                                    *
  *Br    9 :z         : z/D                                                    *
  *Br   10 :tDet      : tDet/D                                                 *
  *Br   11 :xDet      : xDet/D                                                 *
  *Br   12 :yDet      : yDet/D                                                 *
  *Br   13 :zDet      : zDet/D                                                 *
  *Br   14 :DetEnergy : DetEnergy/D                                            *
  *Br   15 :identifier : identifier/I                                          *
  *Br   16 :SeriesType : SeriesType/C                                          *
  */

  ROOT::RDataFrame Series( "FilteredSeries", ExtractFileName, {"Run", "Event", "energy","t",
    "x","y","z","tDet","xDet","yDet","zDet", "DetEnergy","SeriesType"} );
  
  Series.Foreach(
  // Shove data into map depending on series type
  [&EscapeSeries,&AllInSeries](int run, int event, double energy, double t, double x, double y, double z,
  double tDet, double xDet, double yDet, double zDet, double DetEnergy, std::string SeriesType)
  {
    std::tuple<int,int> key = std::make_tuple(run,event);
    std::tuple<double,double,double,double,double,double,double,double,double,double> observables;
    observables = std::make_tuple(energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet);
    std::string EventType;
    for ( auto i = SeriesType.begin(); i != SeriesType.end() && (*i) != 0; ++i ){
      EventType.push_back(*i);
    }
    if(EventType=="AllIn"){push_to_map(key,observables,AllInSeries);}
    else if(EventType=="Escape"){push_to_map(key,observables,EscapeSeries);}
    else{
      std::cerr << "Invalid Series Type" << std::endl;
      exit(EXIT_FAILURE);
      }
  },
  {"Run", "Event", "energy","t","x","y","z","tDet","xDet","yDet","zDet", "DetEnergy","SeriesType"} );
  if(verbose){
    print_ExtractMap(AllInSeries);
    print_ExtractMap(EscapeSeries);
  }
  return 0;
}

std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > ReadExtract(std::string ExtractFileName,bool verbose){
  // Format of Extracted output
  // <Run,Event>, < <energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet,SeriesType(str)>,  ...>

  ROOT::RDataFrame Series( "FilteredSeries", ExtractFileName, {"Run", "Event", "energy","t",
    "x","y","z","tDet","xDet","yDet","zDet", "DetEnergy","SeriesType"} );
  
  std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > Output;

  Series.Foreach(
  // Shove data into map depending on series type
  [&Output](int run, int event, double energy, double t, double x, double y, double z,
  double tDet, double xDet, double yDet, double zDet, double DetEnergy, std::string SeriesType)
  {
    std::tuple<int,int> key = std::make_tuple(run,event);
    std::tuple<double,double,double,double,double,double,double,double,double,double,std::string> observables;
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
