#include <vector>
#include <iostream>

#include "ROOT/RDataFrame.hxx"
#include "TFile.h"
#include "TTree.h"

#include "DetSimEntry.h"
#include "GramsG4Entry.h"
#include "ExtractionEntry.h"

GramsExtractEntry::GramsExtractEntry(){
  MCTruth_flag = false;
  for(int i=0; i<2; ++i){
    data.push_back(0);    
  }
  for(int i=0; i<10; ++i){
    data.push_back(0.0);
  }
  data.push_back("");
}


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

GramsExtractEntry::GramsExtractEntry(GramsG4Entry& G4, GramsDetSimEntry& DetSim, std::string SeriesType, bool MCflag){
    this->set_MCTruth(MCflag);
    data.push_back(G4.get_run());
    data.push_back(G4.get_event());
    data.push_back(G4.get_t());
    data.push_back(G4.get_x());
    data.push_back(G4.get_y());
    data.push_back(G4.get_z());
    data.push_back(G4.get_Energy());
    data.push_back(DetSim.get_t());
    data.push_back(DetSim.get_x());
    data.push_back(DetSim.get_y());
    data.push_back(DetSim.get_z());
    data.push_back(DetSim.get_Energy());
    data.push_back(SeriesType);
}

/*
GramsExtractEntry& GramsExtractEntry::operator=(GramsExtractEntry& other){
  bool cur_flag = other.get_MCTruth();
  data[0] = other.get_run();
  data[1] = other.get_event();
  other.set_MCTruth(true);
  data[2] = other.get_time();
  data[0] = other.get_xpos();
  data[0] = other.get_ypos();
  data[0] = other.get_zpos();
  data[0] = other.get_Energy();
  other.set_MCTruth(false);
  data[2] = other.get_time();
  data[0] = other.get_xpos();
  data[0] = other.get_ypos();
  data[0] = other.get_zpos();
  data[0] = other.get_Energy();
  data[0] = other.get_SeriesType();
  this->set_MCTruth(cur_flag);
  other.set_MCTruth(cur_flag);
  return *this;
}
GramsExtractEntry::GramsExtractEntry(GramsExtractEntry& other){
  bool cur_flag = other.get_MCTruth();
  data[0] = other.get_run();
  data[1] = other.get_event();
  other.set_MCTruth(true);
  data[2] = other.get_time();
  data[0] = other.get_xpos();
  data[0] = other.get_ypos();
  data[0] = other.get_zpos();
  data[0] = other.get_Energy();
  other.set_MCTruth(false);
  data[2] = other.get_time();
  data[0] = other.get_xpos();
  data[0] = other.get_ypos();
  data[0] = other.get_zpos();
  data[0] = other.get_Energy();
  data[0] = other.get_SeriesType();
  this->set_MCTruth(cur_flag);
  other.set_MCTruth(cur_flag);
}
*/

std::vector<int> GramsExtractEntry::extract_key() const{
    return {this->get_run(), this->get_event() };
}

void GramsExtractEntry::print() const{
  std::cout << "Extract: ";
  std::cout <<"run = " << this->get_run() ;
  std::cout <<" event = " << this->get_event() ;
  std::cout << " t = " << this->get_time();
  std::cout << " x = " << this->get_xpos();
  std::cout <<  "y = " << this->get_ypos();
  std::cout << " z = " << this->get_zpos();
  std::cout << " energy = " << this->get_Energy();
  std::cout << " MCTruth = " << this->get_MCTruth();
  std::cout << "\n" <<  std::endl;
}

void GramsExtractEntry::print_all(){
  bool currentMC = this->get_MCTruth();
  this->set_MCTruth(true);
  std::cout << "Extract: ";
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
  this->set_MCTruth(currentMC);
  std::cout << " MCTruth = " << currentMC;
  std::cout << "\n" <<  std::endl;
}


int GramsExtractEntry::get_run() const{
  return this->get_value<int>(0);
}

int  GramsExtractEntry::get_event() const{
  return this->get_value<int>(1);
}

double GramsExtractEntry::get_time() const{
  if(this->get_MCTruth()){
    return this->get_value<double>(2);
  }
  return this->get_value<double>(7);
}

double GramsExtractEntry::get_xpos() const{
  if(this->get_MCTruth()){
    return this->get_value<double>(3);
  }
  return this->get_value<double>(8);
}

double GramsExtractEntry::get_ypos() const{
  if(this->get_MCTruth()){
    return this->get_value<double>(4);
  }
  return this->get_value<double>(9);

}

double GramsExtractEntry::get_zpos() const{
  if(this->get_MCTruth()){
    return this->get_value<double>(5);
  }
  return this->get_value<double>(10);
}

double GramsExtractEntry::get_Energy() const{
  if(this->get_MCTruth()){
    return this->get_value<double>(6);
  }
  return this->get_value<double>(11);
}

std::string GramsExtractEntry::get_SeriesType() const{
  return this->get_value<std::string>(12);
}

void GramsExtractEntry::set_MCTruth(bool flag){
  MCTruth_flag = flag;
}

bool GramsExtractEntry::get_MCTruth() const{
  return MCTruth_flag;
}
