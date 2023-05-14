#include "Options.h"
#include "ExtractUtils.h"


int main(){
  std::map<std::tuple<int,int>, std::vector<std::tuple<int,int,int,std::string,double,float,float,float,double,int>> > mapping;
  mapping = ReadGramsG4("/nevis/milne/files/ms6556/Spring2023/Sensitivity/SensitivityJob/Source_0.root");
  print_G4map(mapping);
}