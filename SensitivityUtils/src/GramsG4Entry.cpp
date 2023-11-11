#include <any>
#include <tuple>

#include "Entry.h"
#include "G4Entry.h"

GramsG4Entry::GramsG4Entry(int run,int event, int TrackID, std::string PDGCode,
  double t, float x, float y, float z,double Etot,int identifier){
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
    int run = this->get_value<int>(0);
    int event = this->get_value<int>(1);
    std::vector<int> output =  {run, event};
    return output;
}