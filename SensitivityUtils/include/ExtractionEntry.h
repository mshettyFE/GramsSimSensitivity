#ifndef EXTRACT_ENTRY
#define EXTRACT_ENTRY

#include "Entry.h"
#include <vector>

class GramsExtractEntry : public Entry{
  private:
    bool MCTruth_flag;
  public:
    // MC_Truth_flag, run, event, energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet, EventType
    GramsExtractEntry(bool, int, int, double,double,double,double,double,double,double,double,double,double,std::string);
    // get key
    std::vector<int> extract_key() override;

    int get_run();
    int get_event();
    std::string get_SeriesType();
    // MC truth variables
    double get_time();
    double get_xpos();
    double get_ypos();
    double get_zpos();
    double get_Energy();

    void set_MCTruth(bool flag);
    bool get_MCTruth();
};

#endif