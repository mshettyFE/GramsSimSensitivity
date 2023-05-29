#ifndef CALC_SENSE_UTILS
#define CALC_SENSE_UTILS 1

#include <string>

#include "TChain.h"
#include "TH2D.h"

bool ReadBackgroundSkyMaps(std::string base_name,int nbatches, TH2D* AggSkyMap);

// Read in Root files for sensitivity calc
void ReadConeData(std::string base_name, int nbatches, TChain &Output);

#endif