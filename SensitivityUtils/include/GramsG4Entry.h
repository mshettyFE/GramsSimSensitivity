#ifndef G4ENTRY
#define G4ENTRY

#include "Entry.h"
#include <vector>

class GramsG4Entry : public Entry{
  public:
    // Run, Event, TrackID,ParentID,PDGCode,ProcessName,t,x,y,z,Etot,identifier
    GramsG4Entry(int run, int event, int TrackID, int ParentID, int PDGCode, std::string ProcessName,
     double t, float x, float y, float z, double Etot, int id);
    // get key
    std::vector<int> extract_key() const override;

    // print
    void print() const override;

     int get_run() const;
     int get_event() const;
     int get_TrackID() const;
     int  get_ParentID() const;
     int get_PDGCode() const;
     std::string get_ProcessName() const;
     double get_t() const;
     float get_x() const;
     float get_y() const;
     float get_z() const;
     double get_Energy() const;
     int get_ID() const;
};

#endif
