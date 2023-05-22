#ifndef CalcEffAreaFuncs
#define CalcEffAreaFuncs 1

#include <string>
#include <map>

// Generate file path to root file
std::string gen_path(std::string starting_path, std::string base, int index);

// Read in all root files of a specific format in a directory and create a map between incident gamma energy and effective area
std::map<double,double> ExtractEffArea(std::string starting_path, std::string base, std::string tree_name, int TotalBatches, int NEvents, bool verbose=false, double sphere_rad=300);

// Write map to txt file
void WriteToText(std::string outputName,std::map<double,double> mapping, char seperator='\t');

// Write map to root file
void WriteToROOT(std::string outputName,std::map<double,double> mapping);

#endif