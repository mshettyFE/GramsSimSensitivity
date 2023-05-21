#ifndef CalcEffAreaFuncs
#define CalcEffAreaFuncs 1

#include <string>
#include <map>

std::string gen_path(std::string starting_path, std::string base, int index);

std::map<double,double> ExtractEffArea(std::string starting_path, std::string base, std::string tree_name, int TotalBatches, int NEvents, bool verbose=false, double sphere_rad=300);

#endif