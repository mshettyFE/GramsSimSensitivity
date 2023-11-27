#ifndef DET_SIM_ENTRY
#define DET_SIM_ENTRY

#include "Entry.h"
#include <vector>

class GramsDetSimEntry : public Entry{
  public:
    // run, event, TrackID, Energy,x_det,y_det,z_det,t_det
    GramsDetSimEntry(int run , int event, int TrackID, double Energy,
    float x, float y,float z, double t);
    // get key
    std::vector<int> extract_key() const override;

    // print
    void print() const override;

     int get_run() const;
     int get_event() const;
     int get_TrackID() const;
     double get_t() const;
     float get_x() const;
     float get_y() const;
     float get_z() const;
     double get_Energy() const;
};

#endif