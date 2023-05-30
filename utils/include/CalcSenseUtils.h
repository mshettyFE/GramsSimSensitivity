#ifndef CALC_SENSE_UTILS
#define CALC_SENSE_UTILS 1

#include <string>

#include "TChain.h"
#include "TH2D.h"

bool ReadBackgroundSkyMaps(std::string base_name,int nbatches, TH2D* AggSkyMap);

// Read in Root files for sensitivity calc
void ReadConeData(std::string base_name, int nbatches, TChain &Output);

std::vector<double> ExtractNonZero(TH2D* histogram);

double Mean(std::vector<double> data);

double StdDev(std::vector<double> data);
#endif