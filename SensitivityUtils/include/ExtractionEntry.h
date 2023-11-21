#ifndef EXTRACT_ENTRY
#define EXTRACT_ENTRY

#include "Entry.h"
#include "GramsG4Entry.h"
#include "DetSimEntry.h"
#include <vector>
#include <string>

#include "TTree.h"


class GramsExtractEntry : public Entry{
  private:
    bool MCTruth_flag;
  public:
    // MC_Truth_flag, run, event, energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet, EventType
    GramsExtractEntry();
    GramsExtractEntry(bool, int, int, double,double,double,double,double,double,double,double,double,double,std::string);
    GramsExtractEntry( GramsG4Entry& G4,  GramsDetSimEntry& DetSim, std::string SType, bool MCflag=true);

    // get key
    std::vector<int> extract_key() const override;

    void print() const override;
    void print_all();

     int get_run() const;
     int get_event() const;
     std::string get_SeriesType() const;
    // MC truth variables
     double get_time() const;
     double get_xpos() const;
     double get_ypos() const;
     double get_zpos() const;
     double get_Energy() const;

    void set_MCTruth(bool flag);
    bool get_MCTruth() const;
};

#endif