#include <vector>
#include <iostream>

#include "GramsG4Entry.h"

GramsG4Entry::GramsG4Entry(int run,int event, int TrackID, int ParentID,  int PDGCode, 
 std::string ProcessName, double t, float x, float y, float z,double Etot,int identifier){
    data.push_back(run);
    data.push_back(event);
    data.push_back(TrackID);
    data.push_back(ParentID);
    data.push_back(PDGCode);
    data.push_back(ProcessName);
    data.push_back(t);
    data.push_back(x);
    data.push_back(y);
    data.push_back(z);
    data.push_back(Etot);
    data.push_back(identifier);
}

std::vector<int> GramsG4Entry::extract_key(){
    return {this->get_run(), this->get_event()};
}

void GramsG4Entry::print(){
  std::cout <<"run= " << this->get_run() ;
  std::cout <<" event = " << this->get_event() ;
  std::cout << " TrackID = " << this->get_TrackID();
  std::cout << " ParentID = " << this->get_ParentID();
  std::cout << "PDG:" << this->get_PDGCode();
  std::cout << "Process Name:" << this->get_ProcessName();
  std::cout << " t = " <<  this->get_t();
  std::cout << " x = " <<  this->get_x();
  std::cout << " y = " <<  this->get_y();
  std::cout << " z = " <<  this->get_z();
  std::cout << " Energy = " <<  this->get_Energy();
  std::cout << " ID = " <<  this->get_ID();
  std::cout << "\n" <<  std::endl;
}

int GramsG4Entry::get_run(){
  return this->get_value<int>(0);
}

int GramsG4Entry::get_event(){
    return this->get_value<int>(1);
}

int GramsG4Entry::get_TrackID(){
    return this->get_value<int>(2);
}

int  GramsG4Entry::get_ParentID(){
    return this->get_value<int>(3);
}

int GramsG4Entry::get_PDGCode(){
    return this->get_value<int>(4);
}

std::string GramsG4Entry::get_ProcessName(){
    return this->get_value<std::string>(5);
}

double GramsG4Entry::get_t(){
    return this->get_value<double>(6);
}

float GramsG4Entry::get_x(){
  return this->get_value<float>(7);
}

float GramsG4Entry::get_y(){
  return this->get_value<float>(8);
}
float GramsG4Entry::get_z(){
  return this->get_value<float>(9);
}
double GramsG4Entry::get_Energy(){
  return this->get_value<double>(10);
}
int GramsG4Entry::get_ID(){
  return this->get_value<int>(11);
}