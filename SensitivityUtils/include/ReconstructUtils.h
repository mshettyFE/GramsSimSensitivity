#ifndef RECONSTRUCT_UTILS
#define RECONSTRUCT_UTILS 1

#include "TTree.h"

#include<vector>
#include<tuple>
#include<cmath>
#include <memory>

#include "ExtractionEntry.h"
#include "UsefulTypeDefs.h"

// Convert tuple of spherical coordinates RA,ALT to a tuple of cartesian coordinates x,y,z
R3 SphereToCart(const SkyMapLoc& Spherical);

// Calculate dot product between two 3D tuples
double dot(const R3& A, const R3& B);

// Return distances between adjacent scatters. Ignore first event b/c it is assumed to be a gamma ray
std::vector<R3> Distances(const std::vector<GramsExtractEntry> &Series);

// Extract Kinetic Energies from Series
std::vector<double> KineticEnergies(const std::vector<GramsExtractEntry> &Series, bool verbose=false, double e_mass = .51099895);

// Calculate Reconstructed angle for a given scatter series
bool RecoAngle(const std::vector<R3>& Dist, const  std::vector<double>& KEs,
double &ReconAngle, bool escape = false, double e_mass = .51099895);

// Calculate ARM. Returns 2*pi if Source type is Iso.
bool ARM(double RecoAngle, const std::vector<R3>& Dist, R3 SourceLoc, double &ARM_val, std::string SourceType="Point");

void Reconstruction(std::map<EntryKey, std::vector<GramsExtractEntry> > &ScatterSeries,
 std::string FileName, R3 truthLoc, bool verbose, std::string SourceType = "Point");

#endif