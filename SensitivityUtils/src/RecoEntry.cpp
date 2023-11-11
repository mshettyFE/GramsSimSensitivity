#include <vector>
#include <iostream>
#include "RecoEntry.h"


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

std::vector<int> GramsRecoEntry::extract_key(){
    return {this->get_run(), this->get_event() };
}

void GramsRecoEntry::print(){
  std::cout << "run= " << this->get_run() ;
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

int GramsRecoEntry::get_run(){
  return this->get_value<int>(0);
}

int GramsRecoEntry::get_event(){
  return this->get_value<int>(1);
}

double GramsRecoEntry::get_XDir(){
  return this->get_value<double>(2);
}

double GramsRecoEntry::get_YDir(){
  return this->get_value<double>(3);
}

double GramsRecoEntry::get_ZDir(){
  return this->get_value<double>(4);
}

R3 GramsRecoEntry::get_Axis(){
    return {this->get_value<double>(2),  this->get_value<double>(3),  this->get_value<double>(4)};
}

double GramsRecoEntry::get_XTip(){
  return this->get_value<double>(5);
}

double GramsRecoEntry::get_YTip(){
  return this->get_value<double>(6);
}

double GramsRecoEntry::get_ZTip(){
  return this->get_value<double>(7);
}

R3 GramsRecoEntry::get_Tip(){
    return {this->get_value<double>(5),  this->get_value<double>(6),  this->get_value<double>(7)};
}

double GramsRecoEntry::get_RecoAngle(){
  return this->get_value<double>(8);
}
double GramsRecoEntry::get_TruthEnergy(){
  return this->get_value<double>(9);
}