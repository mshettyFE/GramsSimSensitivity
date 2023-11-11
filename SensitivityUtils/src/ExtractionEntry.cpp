#include <vector>

#include "ExtractionEntry.h"

GramsExtractEntry::GramsExtractEntry(bool MCTruth, int run, int event, double t, double x, double y, double z, double E,
 double tDet, double xDet, double yDet, double zDet, double EDet, std::string SeriesType){
    MCTruth_flag = MCTruth;
    data.push_back(run);
    data.push_back(event);
    data.push_back(t);
    data.push_back(x);
    data.push_back(y);
    data.push_back(z);
    data.push_back(E);
    data.push_back(tDet);
    data.push_back(xDet);
    data.push_back(yDet);
    data.push_back(zDet);
    data.push_back(EDet);
    data.push_back(SeriesType);
}

std::vector<int> GramsExtractEntry::extract_key(){
    return {this->get_run(), this->get_event() };
}

int GramsExtractEntry::get_run(){
  return this->get_value<int>(0);
}

int GramsExtractEntry::get_event(){
  return this->get_value<int>(1);
}

std::string GramsExtractEntry::get_SeriesType(){
  return this->get_value<std::string>(12);
}

double GramsExtractEntry::get_time(){
  if(this->get_MCTruth()){
    return this->get_value<double>(2);
  }
  return this->get_value<double>(7);
}

double GramsExtractEntry::get_xpos(){
  if(this->get_MCTruth()){
    return this->get_value<double>(3);
  }
  return this->get_value<double>(8);
}

double GramsExtractEntry::get_ypos(){
  if(this->get_MCTruth()){
    return this->get_value<double>(4);
  }
  return this->get_value<double>(9);

}

double GramsExtractEntry::get_zpos(){
  if(this->get_MCTruth()){
    return this->get_value<double>(5);
  }
  return this->get_value<double>(10);
}

double GramsExtractEntry::get_Energy(){
  if(this->get_MCTruth()){
    return this->get_value<double>(6);
  }
  return this->get_value<double>(11);
}

void GramsExtractEntry::set_MCTruth(bool flag){
  MCTruth_flag = flag;
}

bool GramsExtractEntry::get_MCTruth(){
  return MCTruth_flag;
}
