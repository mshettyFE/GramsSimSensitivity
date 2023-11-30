#include <vector>
#include <iostream>

#include "DetSimEntry.h"

GramsDetSimEntry::GramsDetSimEntry(int run, int event, int TrackID, double energy,
float x_det, float y_det, float z_det, double t_det){
    data.push_back(run);
    data.push_back(event);
    data.push_back(TrackID);
    data.push_back(energy);
    data.push_back(x_det);
    data.push_back(y_det);
    data.push_back(z_det);
    data.push_back(t_det);
}

std::vector<int> GramsDetSimEntry::extract_key() const{
  std::vector<int> output = {this->get_run(), this->get_event(), this->get_TrackID()};
    return output;
}

void GramsDetSimEntry::print() const{
  std::cout << "Det: ";
  std::cout <<"run= " << this->get_run() ;
  std::cout <<" event = " << this->get_event() ;
  std::cout << " TrackID = " << this->get_TrackID();
  std::cout << " energy = " << this->get_Energy();
  std::cout << " xdet = " << this->get_x();
  std::cout << " ydet = " << this->get_y();
  std::cout << " zdet = " << this->get_z();
  std::cout << " tdet = " << this->get_t();
  std::cout << "\n" <<  std::endl;
}

int GramsDetSimEntry::get_run() const{
  return this->get_value<int>(0);
}

 int GramsDetSimEntry::get_event() const{
  return this->get_value<int>(1);
}

int GramsDetSimEntry::get_TrackID()  const{
  return this->get_value<int>(2);
}

double GramsDetSimEntry::get_Energy() const{
  return this->get_value<double>(3);
}

float GramsDetSimEntry::get_x() const{
  return this->get_value<float>(4);
}

float GramsDetSimEntry::get_y() const{
  return this->get_value<float>(5);
}

float GramsDetSimEntry::get_z() const{
  return this->get_value<float>(6);
}

double GramsDetSimEntry::get_t() const{
  return this->get_value<double>(7);
}
