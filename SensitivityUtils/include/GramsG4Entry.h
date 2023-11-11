#ifndef G4ENTRY
#define G4ENTRY

#include "Entry.h"
#include <any>
#include <vector>

class GramsG4Entry : public Entry{
  public:
    // Run, Event, TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier
    GramsG4Entry(int,int,int,std::string,double,float,float,float,double,int);
    // get key
    std::vector<int> extract_key() override;
};

#endif
