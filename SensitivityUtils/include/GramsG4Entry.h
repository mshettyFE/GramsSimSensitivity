#ifndef G4ENTRY
#define G4ENTRY

#include "Entry.h"
#include <vector>

class GramsG4Entry : public Entry{
  public:
    // Run, Event, TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier
    GramsG4Entry(int, int, int,int,int,std::string,double,float,float,float,double,int);
    // get key
    std::vector<int> extract_key() override;

    // print
    void print() override;

    int get_run();
    int get_event();
    int get_TrackID();
    int  get_ParentID();
    int get_PDGCode();
    std::string get_ProcessName();
    double get_t();
    float get_x();
    float get_y();
    float get_z();
    double get_Energy();
    int get_ID();
};

#endif
