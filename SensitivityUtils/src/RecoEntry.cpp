#include <vector>
#include <iostream>
#include "RecoEntry.h"

void GramsRecoEntry::Fill(TTree* ntuple, int& run, int& event, int& escape, double& xDir,double& yDir,double& zDir,
  double& xTip, double& yTip,double& zTip, 
  double& RecoAngle, double& ARM, double& RecoEnergy, double& TruthEnergy,
  bool verbose){
    run = this->get_run();
    event = this->get_event();
    escape = this->get_escape();
    xDir = this->get_XDir();
    yDir = this->get_YDir();
    zDir = this->get_ZDir();
    xTip = this->get_XTip();
    yTip = this->get_YTip();
    zTip = this->get_ZTip();
    RecoAngle = this->get_RecoAngle();
    ARM = this->get_ARM();
    RecoEnergy = this->get_RecoEnergy();
    TruthEnergy = this->get_TruthEnergy();
    if(verbose){
      this->print();
    }
    ntuple->Fill();
  }

GramsRecoEntry::GramsRecoEntry(){}

GramsRecoEntry::GramsRecoEntry(int run, int event, int escape, double xDir, double yDir, double zDir,
double xTip, double yTip, double zTip, 
double RecoAngle, double ARM, double RecoEnergy, double TruthEnergy){
    data.push_back(run);
    data.push_back(event);
    data.push_back(escape);
    data.push_back(xDir);
    data.push_back(yDir);
    data.push_back(zDir);
    data.push_back(xTip);
    data.push_back(yTip);
    data.push_back(zTip);
    data.push_back(RecoAngle);
    data.push_back(ARM);
    data.push_back(RecoEnergy);
    data.push_back(TruthEnergy);
}

GramsRecoEntry::GramsRecoEntry(std::vector<GramsExtractEntry> entry, std::vector<R3> AdjDists, int escape, double ReconAngle, double arm, double RecEnergy ){
  if(entry.size() < 3){
    throw std::invalid_argument("must  be at least 3 events");
  }
  if(AdjDists.size() < 1){
    throw std::invalid_argument("AdjDists must be at least of length 1");
  }
  data.push_back(entry[0].get_run());
  data.push_back(entry[0].get_event());
  data.push_back(escape);
  data.push_back(std::get<0>(AdjDists[0]));
  data.push_back(std::get<1>(AdjDists[0]));
  data.push_back(std::get<2>(AdjDists[0]));
  data.push_back(entry[1].get_xpos());
  data.push_back(entry[1].get_ypos());
  data.push_back(entry[1].get_zpos());
  data.push_back(ReconAngle);
  data.push_back(arm);
  data.push_back(RecEnergy);
  bool a = entry[1].get_MCTruth();
  entry[1].set_MCTruth(true);
  data.push_back(entry[0].get_Energy());
  entry[1].set_MCTruth(a);
}


std::vector<int> GramsRecoEntry::extract_key() const{
    return {this->get_run(), this->get_event() };
}

void GramsRecoEntry::print() const{
  std::cout << "Reco: ";
  std::cout << " run= " << this->get_run() ;
  std::cout << " event = " << this->get_event() ;
  std::cout << " escape = " << this->get_escape() ;
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

int GramsRecoEntry::get_escape() const{
  return this->get_value<int>(2);
}

double GramsRecoEntry::get_XDir() const{
  return this->get_value<double>(3);
}

double GramsRecoEntry::get_YDir() const{
  return this->get_value<double>(4);
}

double GramsRecoEntry::get_ZDir() const{
  return this->get_value<double>(5);
}

R3 GramsRecoEntry::get_Axis() const{
    return {this->get_XDir(), this->get_YDir()  , this->get_ZDir()  };
}

double GramsRecoEntry::get_XTip() const{
  return this->get_value<double>(6);
}

double GramsRecoEntry::get_YTip() const{
  return this->get_value<double>(7);
}

double GramsRecoEntry::get_ZTip() const{
  return this->get_value<double>(8);
}

R3 GramsRecoEntry::get_Tip() const{
    return {this->get_XTip(), this->get_YTip()  , this->get_ZTip()  };
}

double GramsRecoEntry::get_RecoAngle() const{
  return this->get_value<double>(9);
}

double GramsRecoEntry::get_ARM() const{
  return this->get_value<double>(10);
}

double GramsRecoEntry::get_RecoEnergy() const{
  return this->get_value<double>(11);
}

double GramsRecoEntry::get_TruthEnergy() const{
  return this->get_value<double>(12);
}