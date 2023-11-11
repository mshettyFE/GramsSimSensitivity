#include <vector>

#include "DetSimEntry.h"

GramsDetSimEntry::GramsDetSimEntry(int run, int event, int TrackID, double energy,
double x_det, double y_det, double z_det, double t_det){
    data.push_back(run);
    data.push_back(event);
    data.push_back(TrackID);
    data.push_back(energy);
    data.push_back(x_det);
    data.push_back(y_det);
    data.push_back(z_det);
    data.push_back(t_det);
}

std::vector<int> GramsDetSimEntry::extract_key(){
    return {this->get_run(), this->get_event(), this->get_TrackID()};
}

int GramsDetSimEntry::get_run(){
  return this->get_value<int>(0);
}

int GramsDetSimEntry::get_event(){
    return this->get_value<int>(1);
}

int GramsDetSimEntry::get_TrackID(){
    return this->get_value<int>(2);
}

double GramsDetSimEntry::get_E(){
  return this->get_value<double>(3);
}

float GramsDetSimEntry::get_x(){
  return this->get_value<float>(4);
}

float GramsDetSimEntry::get_y(){
  return this->get_value<float>(5);
}

float GramsDetSimEntry::get_z(){
  return this->get_value<float>(6);
}

double GramsDetSimEntry::get_t(){
    return this->get_value<double>(7);
}
