#include "TTree.h"

#include <vector>
#include <map>
#include <cstring>
#include <cmath>
#include <iostream>

#include "ReconstructUtils.h"
#include "ExtractionEntry.h"
#include "UsefulTypeDefs.h"

R3 SphereToCart(const SkyMapLoc& Spherical){
  // Convert tuple of spherical coordinates RA,ALT to a tuple of cartesian coordinates x,y,z
    double RA = std::get<0>(Spherical);
    double ALT = std::get<1>(Spherical);
    double x = cos(ALT)*cos(RA);
    double y = cos(ALT)*sin(RA);
    double z = sin(ALT);
    R3 Cartesian = std::make_tuple(x,y,z);
    return Cartesian;
}

double dot(const R3& A, const  R3& B){
  // Basic 3d dot product
    return std::get<0>(A)*std::get<0>(B)+std::get<1>(A)*std::get<1>(B)+std::get<2>(A)*std::get<2>(B);
}

std::vector<R3> Distances(const std::vector<GramsExtractEntry> &Series){
  // Returns normalized distances between each interaction. Based on what MC_Truth variable in each GramsExtractEntry is, selects between MC_Truth data or DetSim Data in NTuple
    std::vector<R3> Dist;
  // We ignore the Primary gamma ray and we don't work with the last event since it has no next neighbor
  double dx, dy, dz, norm;
  for(unsigned long int i =1; i<(Series.size()-1); ++i){ 
  // Calculate the vector between the interaction points
    dx = Series[i].get_xpos()-Series[i+1].get_xpos();
    dx = Series[i].get_ypos()-Series[i+1].get_ypos();
    dx = Series[i].get_zpos()-Series[i+1].get_zpos();
    norm = sqrt(dx*dx+dy*dy+dz*dz);
  // Return normalized distances b/c calcs become easier and for some, we need to normalize this anyways
    Dist.push_back(std::make_tuple(dx/norm,dy/norm,dz/norm));
  }
  return Dist;
}

std::vector<double> KineticEnergies(const std::vector<GramsExtractEntry> &Series, double e_mass){
  // Extract Kinetic Energies of scatters from Series. Extract MC energy or DetSim energy depending on MC_Truth flag in each Entry
  std::vector<double> KEs;
  // We ignore the Primary gamma ray
  for(unsigned long int i =1; i<(Series.size()); ++i){ 
    // We remove the rest mass of the electron from the energy
    KEs.push_back(Series[i].get_Energy()-e_mass);
  }
  return KEs;
}

// Calculate Reconstructed angle for a given scatter series
bool RecoAngle(const std::vector<R3>& Dist, const std::vector<double>& KEs, double &ReconAngle, bool escape, double e_mass){
  // Write RecoAngle function that takes in Adj and KEs and returns a boolean flag. Write to outparameter to store RecoAngle
  if(escape){
    if (KEs.size() >= 3){
    // From original GRAMS paper (https://arxiv.org/abs/1901.03430)
      double E1 = KEs[0];
      double E2 = KEs[1];
      double b = E2/2.0;
      // Since Distances are normalized, we have that cos(theta_prime)= (A \cdot B)/(1*1)
      double cos_theta_prime = dot(Dist[0],Dist[1]);
      double E3_prime = -b+sqrt(b*b+E2*e_mass/(1-cos_theta_prime));
      double E_summed = E1+E2+E3_prime;
      double arg = 1.0-e_mass*(1.0/(E2+E3_prime)-1.0/E_summed);
      if ((arg < -1.0) || (arg > 1.0)){
        return false;
      }
      ReconAngle = acos(arg);
    }
    else{
      return false;
    }
  }
  else{
  // Since we have perfect information of the system, we know which order the scatters happened in
  // In reality, we would need to test each possible sequence of scatters to determine which one is correct (Dogan, Wehe, and Knoll 1990)
  // Hopefully, the physical GRAMS detector has sufficient timing resolution that we can detect the order of events
      double E_tot = 0.0;
        for(auto Kinetic: KEs){E_tot += Kinetic;}
      double E_final = E_tot-KEs[0];
      double arg = 1.0-e_mass*(1.0/(E_final)-1.0/E_tot);
      if ((arg <-1.0) || (arg>1.0)){
        return false;
      }
      ReconAngle = acos(arg);
  }
  return true;
}

// Calculate ARM from RecoAngle, Axis of Compton Cone, and True Source Location. Works for Isotropic as well
bool ARM(double RecoAngle, const std::vector<R3>& Dist, R3 SourceLoc, double &ARM_val, std::string SourceType){
  // We assume that SourceLoc is normalized
    if(SourceType=="Iso"){
  // Return 2*pi is SourceType is Iso. Remeber that is ARM value has no meaning
        ARM_val = 2*acos(-1);
        return true;
    }
    else if(SourceType=="Point"){
        // Axis of Cone, which we assume is normalized
        R3 Axis = Dist[0];
        ARM_val = RecoAngle-acos(dot(Axis,SourceLoc));
        return true;
    }
    else{
        return false;
    }
}

void Reconstruction(std::map<EntryKey, std::vector<GramsExtractEntry> > &ScatterSeries,
 TTree* tree,R3 truthLoc,  bool MCTruth, bool verbose, std::string SourceType){
  bool escape;
  double ARM_val, Re_Angle, RecoEnergy;
  for ( auto Series = ScatterSeries.begin(); Series != ScatterSeries.end(); ++Series ){
    // Sort Series by time. MC_Truth toggles which time we want
    sort((*Series).second.begin(), (*Series).second.end(),[](GramsExtractEntry& a,
    GramsExtractEntry& b){
    return (a.get_time() < b.get_time());});
    std::vector<GramsExtractEntry> Events = Series->second;
    std::string EscapeType = Events[0].get_SeriesType();
    if(EscapeType=="AllIn"){
        escape=0;
    }
    else if(EscapeType=="Escape"){
        escape=1;
    }
    else{
        continue;
    }
  
  // Define Adjacent Distances and Kinetic Energies
    std::vector<R3> AdjacentDist;
    std::vector<double> KEs;
    AdjacentDist = Distances(Events);
    if(AdjacentDist.size() < 2){
      continue;
    }
    KEs = KineticEnergies(Events);
  // Calculate Reconstruction Angle (if possible)
  // Check if Reconstruction Angle could be calculated
    if (!RecoAngle(AdjacentDist,KEs,Re_Angle,escape)){
        continue;
    }
  // Calculate ARM
    if (!ARM(Re_Angle, AdjacentDist, truthLoc,ARM_val, SourceType)){
        continue;
    }
  // Calculate Reconstructed Energy
    RecoEnergy = 0;
    for(auto KE: KEs){RecoEnergy += KE;}
  // Add NTuple to TTree
// modify this to use DumpData to std::vector<std::variant> somehow...
//    WriteRecoEntry(tree, Events, AdjacentDist, escape, Re_Angle, ARM_val,  RecoEnergy, verbose);
  }

}
