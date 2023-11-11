#ifndef GRAMS_SENSE_TYPEDEFS
#define GRAMS_SENSE_TYPEDEFS 1
#include <variant>

// Typedefs so I don't have to type out all of these.
// Also so that I have one place where I can refer to to remember which variable is which

// contains data associated with a hit event
typedef std::variant<int, double, float, std::string> EntryData;
// groups EntryData into 1 event hit
typedef std::vector<EntryData>  EntryRow;
// <TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier>
typedef std::tuple<int,int,int,std::string,double,float,float,float,double,int> G4Entry;
// <Energy,x_det,y_det,z_det,t_det>
typedef std::tuple<double,double, double,double,double> DetSimEntry;
//<energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet>
typedef std::tuple<double,double,double,double,double,double,double,double,double,double,std::string> ExtractEntry;
//<xDir,yDir,zDir,xTip,yTip,zTip,RecoAngle, TruthEnergy>
typedef std::tuple<double,double,double,double,double,double,double,double> ReconstructEntry;
// cartesian coordinates <x,y,z>
typedef std::tuple<double,double,double> R3;
// RA,ALT coordinates <RA,ALT>
typedef std::tuple<double,double> SkyMapLoc;
#endif