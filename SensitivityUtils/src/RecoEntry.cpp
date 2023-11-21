#include <vector>
#include <iostream>
#include "RecoEntry.h"

GramsRecoEntry::GramsRecoEntry(){
  for(int i=0; i<2; ++i){
    data.push_back(0);    
  }
  for(int i=0; i<8; ++i){
    data.push_back(0.0);
  }
}

GramsRecoEntry::GramsRecoEntry(int run, int event, double xDir, double yDir, double zDir,
double xTip, double yTip, double zTip, double RecoAngle, double TruthEnergy){
    data.push_back(run);
    data.push_back(event);
    data.push_back(xDir);
    data.push_back(yDir);
    data.push_back(zDir);
    data.push_back(xTip);
    data.push_back(yTip);
    data.push_back(zTip);
    data.push_back(RecoAngle);
    data.push_back(TruthEnergy);
}

std::vector<int> GramsRecoEntry::extract_key() const{
    return {this->get_run(), this->get_event() };
}

void GramsRecoEntry::print() const{
  std::cout << "Reco: ";
  std::cout << " run= " << this->get_run() ;
  std::cout << " event = " << this->get_event() ;
  std::cout << " xDir = " << this->get_XDir();
  std::cout << " yDir = " << this->get_YDir();
  std::cout << " zDir = " << this->get_ZDir();
  std::cout << " xTip = " << this->get_XTip();
  std::cout << " yTip = " << this->get_YTip();
  std::cout << " zTip = " << this->get_ZTip();
  std::cout << " RecoAngle = " << this->get_RecoAngle();
  std::cout << " TruthEnergy = " << this->get_TruthEnergy();
  std::cout << '\n' << std::endl;
}

int GramsRecoEntry::get_run() const{
  return this->get_value<int>(0);
}

int GramsRecoEntry::get_event() const{
  return this->get_value<int>(1);
}

double GramsRecoEntry::get_XDir() const{
  return this->get_value<double>(2);
}

double GramsRecoEntry::get_YDir() const{
  return this->get_value<double>(3);
}

double GramsRecoEntry::get_ZDir() const{
  return this->get_value<double>(4);
}

R3 GramsRecoEntry::get_Axis() const{
    return {this->get_value<double>(2),  this->get_value<double>(3),  this->get_value<double>(4)};
}

double GramsRecoEntry::get_XTip() const{
  return this->get_value<double>(5);
}

double GramsRecoEntry::get_YTip() const{
  return this->get_value<double>(6);
}

double GramsRecoEntry::get_ZTip() const{
  return this->get_value<double>(7);
}

R3 GramsRecoEntry::get_Tip() const{
    return {this->get_value<double>(5),  this->get_value<double>(6),  this->get_value<double>(7)};
}

double GramsRecoEntry::get_RecoAngle() const{
  return this->get_value<double>(8);
}
 
double GramsRecoEntry::get_TruthEnergy() const{
  return this->get_value<double>(9);
}