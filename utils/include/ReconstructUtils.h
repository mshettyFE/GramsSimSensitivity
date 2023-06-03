#ifndef ReconstructUtils
#define ReconstructUtils 1

#include "TTree.h"

#include<vector>
#include<tuple>
#include<cmath>

#include "UsefulTypeDefs.h"

// Return distances between adjacent scatters. Ignore first event b/c it is assumed to be a gamma ray
std::vector<R3> Distances(
std::vector<ExtractEntry> &Series);

// Extract Kinetic Energies from Series
std::vector<double> KineticEnergies(std::vector<ExtractEntry> &Series, 
double e_mass = .51099895);

// Calculate Reconstructed angle for a given scatter series
bool RecoAngle(std::vector<R3> Dist, std::vector<double> KEs, double &ReconAngle, bool escape = false, double e_mass = .51099895);

// Convert tuple of spherical coordinates RA,ALT to a tuple of cartesian coordinates x,y,z
R3 SphereToCart(SkyMapLoc Spherical);

// Calculate dot product between two 3D tuples
double dot(R3 A, R3 B);

// Calculate ARM. Returns 2*pi if Source type is Iso.
bool ARM(double RecoAngle, std::vector<R3> Dist, R3 SourceLoc, double &ARM_val, std::string SourceType="Point");

// 
void Reconstruction(
  std::map<std::tuple<int,int>, std::vector<ExtractEntry> > &ScatterSeries,
 TTree* tree,R3 truthLoc,std::string SourceType="Point");

#endif