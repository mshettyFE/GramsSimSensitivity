#ifndef DET_SIM_ENTRY
#define DET_SIM_ENTRY

#include "Entry.h"
#include <vector>

class GramsDetSimEntry : public Entry{
  public:
    // run, event, TrackID, Energy,x_det,y_det,z_det,t_det
    GramsDetSimEntry(int, int, int,double,double, double,double,double);
    // get key
    std::vector<int> extract_key() override;

    int get_run();
    int get_event();
    int get_TrackID();
    double get_t();
    float get_x();
    float get_y();
    float get_z();
    double get_E();
};

#endif