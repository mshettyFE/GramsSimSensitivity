#include <vector>

#include "GramsG4Entry.h"

GramsG4Entry::GramsG4Entry(int run,int event, int TrackID, int ParentID,  int PDGCode, 
 std::string ProcessName, double t, float x, float y, float z,double Etot,int identifier){
    data.push_back(run);
    data.push_back(event);
    data.push_back(TrackID);
    data.push_back(PDGCode);
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
double GramsG4Entry::get_E(){
  return this->get_value<double>(10);
}
int GramsG4Entry::get_ID(){
  return this->get_value<int>(11);
}