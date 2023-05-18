#ifndef ReconstructUtils
#define ReconstructUtils 1

#include<TTree.h>

#include<vector>
#include<tuple>
#include<cmath>

// Return distances between adjacent scatters. Ignore first event b/c it is assumed to be a gamma ray
std::vector<std::tuple<double,double,double>> Distances(
std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> &Series);

// Extract Kinetic Energies from Series
std::vector<double> KineticEnergies(std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> &Series, 
double e_mass = .51099895);

// Calculate Reconstructed angle for a given scatter series
bool RecoAngle(std::vector<std::tuple<double,double,double>> Dist, std::vector<double> KEs, double &ReconAngle, bool escape = false, double e_mass = .51099895);

// Convert tuple of spherical coordinates RA,ALT to a tuple of cartesian coordinates x,y,z
std::tuple<double,double,double> SphereToCart(std::tuple<double,double> Spherical);

// Calculate dot product between two 3D tuples
double dot(std::tuple<double,double,double> A, std::tuple<double,double,double> B);

// Calculate ARM. Returns 2*pi if Source type is Iso.
bool ARM(double RecoAngle, std::vector<std::tuple<double,double,double>> Dist, std::tuple<double,double,double> SourceLoc, double &ARM_val, std::string SourceType="Point");

void Reconstruction(
  std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > &ScatterSeries,
 TTree* tree,std::tuple<double,double,double> truthLoc,std::string SourceType="Point");

#endif