#include "ReadRootFiles.h"

#include<vector>
#include<map>
#include<tuple>
#include<string>

int main(int argc, char** argv){
  std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double>> > AllInSeries;
  std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double>> > EscapeSeries;
  std::string fname = "/nevis/milne/files/ms6556/Spring2023/Sensitivity/SensitivityJob/SourceExtracted_0.root";
  ReadExtract(fname, AllInSeries, EscapeSeries,true);

  std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > Input;
  Input = ReadExtract(fname,true);

    return 0;
}