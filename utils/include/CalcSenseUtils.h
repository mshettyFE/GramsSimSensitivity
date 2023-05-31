#ifndef CALC_SENSE_UTILS
#define CALC_SENSE_UTILS 1

#include <string>
#include <tuple>

#include "TChain.h"
#include "TH2D.h"

bool ReadBackgroundSkyMaps(std::string base_name,int nbatches, TH1D* AggSkyMap);

// Read in Root files for sensitivity calc
void ReadConeData(std::string base_name, int nbatches, TChain &Output);

double ExtractEffArea(double SourceEnergy, TH1D* EffAreaHist);

std::tuple<long,long> CalculateUsedPhotons(std::string base_name, int nbatches, int PhotonsPerBatch, double threshold);

int ExtractBinNum(double SourceEnergy, TH1D* Histogram);
#endif