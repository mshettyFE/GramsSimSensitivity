#ifndef CALC_SENSE_UTILS
#define CALC_SENSE_UTILS 1

#include <string>
#include <tuple>

#include "TChain.h"
#include "TH2D.h"

// Fill out AggSkyMap with the weighted counts at each energy level
bool ReadBackgroundCounts(std::string base_name,int nbatches, TH1D* AggSkyMap, bool verbose=false);

// Given an energy, extract the effective area from EffArea Hist (error checking on upper bound)
double ExtractEffArea(double SourceEnergy, TH1D* EffAreaHist);

// Extract bin number given Energy and histogram. edge checking included
int ExtractBinNum(double SourceEnergy, TH1D* Histogram);
#endif