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
    GramsExtractEntry(bool, int, int, double,double,double,double,double,double,double,double,double,double,std::string);
    GramsExtractEntry( GramsG4Entry& G4,  GramsDetSimEntry& DetSim, std::string SType, bool MCflag=true);

    void Fill(TTree* ntuple, int& Run, int& Event, 
      double& t, double& x,double& y,double& z,double& energy,
  double& tDet,double& xDet, double& yDet,double&  zDet, double& DetEnergy,
  std::string& SeriesType,
  bool verbose = false);

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