#ifndef EXTRACT_ENTRY
#define EXTRACT_ENTRY

#include "Entry.h"
#include <vector>

class GramsExtractEntry : public Entry{
  public:
    // run, event, energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet, EventType
    GramsExtractEntry(int, int, double,double,double,double,double,double,double,double,double,double,std::string);
    // get key
    std::vector<int> extract_key() override;

    int get_run();
    int get_event();
    std::string get_SeriesType();
    // MC truth variables
    double get_t();
    double get_x();
    double get_y();
    double get_z();
    double get_E();
    // DetSimVariables
    double get_tDet();
    double get_xDet();
    double get_yDet();
    double get_zDet();
    double get_EDet();

};

#endif