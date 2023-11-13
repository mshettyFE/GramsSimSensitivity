#include <vector>
#include <iostream>

#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"

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

void GramsExtractEntry::print(){
  bool currentMC = this->get_MCTruth();
  this->set_MCTruth(true);
  std::cout <<"run = " << this->get_run() ;
  std::cout <<" event = " << this->get_event() ;
  std::cout << " MC_t = " << this->get_time();
  std::cout << " MC_x = " << this->get_xpos();
  std::cout <<  "MC_y = " << this->get_ypos();
  std::cout << " MC_z = " << this->get_zpos();
  std::cout << " MC_energy = " << this->get_Energy();
  this->set_MCTruth(false);
  std::cout << " Det_t = " << this->get_time();
  std::cout << " Det_x = " << this->get_xpos();
  std::cout << " Det_y = " << this->get_ypos();
  std::cout << " Det_z = " << this->get_zpos();
  std::cout << " Det_energy = " << this->get_Energy();
  std::cout << "\n" <<  std::endl;
  this->set_MCTruth(currentMC);
}

int GramsExtractEntry::get_run(){
  return this->get_value<int>(0);
}

int GramsExtractEntry::get_event(){
  return this->get_value<int>(1);
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

std::string GramsExtractEntry::get_SeriesType(){
  return this->get_value<std::string>(12);
}

void GramsExtractEntry::set_MCTruth(bool flag){
  MCTruth_flag = flag;
}

bool GramsExtractEntry::get_MCTruth(){
  return MCTruth_flag;
}
