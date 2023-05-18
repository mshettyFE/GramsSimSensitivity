#include<TTree.h>

#include <vector>
#include <tuple>
#include <map>
#include <cstring>
#include <cmath>
#include <iostream>

// Convert tuple of spherical coordinates RA,ALT to a tuple of cartesian coordinates x,y,z
std::tuple<double,double,double> SphereToCart(std::tuple<double,double> Spherical){
    double RA = std::get<0>(Spherical);
    double ALT = std::get<1>(Spherical);
    double x = cos(ALT)*cos(RA);
    double y = cos(ALT)*sin(RA);
    double z = sin(ALT);
    std::tuple<double,double,double> Cartesian = std::make_tuple(x,y,z);
    return Cartesian;
}

double dot(std::tuple<double,double,double> A, std::tuple<double,double,double> B){
    return std::get<0>(A)*std::get<0>(B)+std::get<1>(A)*std::get<1>(B)+std::get<2>(A)*std::get<2>(B);
}


std::vector<std::tuple<double,double,double>> Distances(
  std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> &Series){
  // Value of input
  // < <energy,t,x,y,z,DetEnergy,tDet,xDet,yDet,zDet>,  ...>
  // We define the output vector
    std::vector<std::tuple<double,double,double>> Dist;
  //    <x,y,z,energy>,<x,y,z,energy> ...
  // We ignore the Primary gamma ray and we don't work with the last event since it has no next neighbor
  for(int i =1; i<(Series.size()-1); ++i){ 
  // Calculate the vector between the interaction points
    double dx = std::get<2>(Series[i])-std::get<2>(Series[i+1]);
    double dy = std::get<3>(Series[i])-std::get<3>(Series[i+1]);
    double dz = std::get<4>(Series[i])-std::get<4>(Series[i+1]);
    double norm = sqrt(dx*dx+dy*dy+dz*dz);
  // Return normalized distances b/c calcs become easier and for some, we need to normalize this anyways
    Dist.push_back(std::make_tuple(dx/norm,dy/norm,dz/norm));
  }
  return Dist;
}

// Extract Kinetic Energies of scatters from Series
std::vector<double> KineticEnergies(std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> &Series, 
  double e_mass = .51099895){
  // Returns the kinetic energies of the electrons
  std::vector<double> KEs;
  // We ignore the Primary gamma ray
  for(int i =1; i<(Series.size()); ++i){ 
    // We remove the rest mass of the electron from the energy
    KEs.push_back(std::get<0>(Series[i])-e_mass);
  }
  return KEs;
}

// Calculate Reconstructed angle for a given scatter series
bool RecoAngle(std::vector<std::tuple<double,double,double>> Dist, std::vector<double> KEs, double &ReconAngle, bool escape = false, double e_mass = .51099895){
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
bool ARM(double RecoAngle, std::vector<std::tuple<double,double,double>> Dist, std::tuple<double,double,double> SourceLoc, double &ARM_val, std::string SourceType="Point"){
  // We assume that SourceLoc is normalized
    if(SourceType=="Iso"){
  // Return 2*pi is SourceType is Iso. Remeber that is ARM value has no meaning
        ARM_val = 2*acos(-1);
        return true;
    }
    else if(SourceType=="Point"){
        // Axis of Cone, which we assume is normalized
        std::tuple<double,double,double> Axis = Dist[0];
        ARM_val = acos(dot(Axis,SourceLoc));
        return true;
    }
    else{
        return false;
    }
}

void Reconstruction(std::map<std::tuple<int,int>, std::vector<std::tuple<double,double,double,double,double,double,double,double,double,double,std::string>> > &ScatterSeries,
 TTree* tree,std::tuple<double,double,double> truthLoc,std::string SourceType){

  // Define variables to write to to fill TTree
  int RunNumber,EventNumber,escape;
  double xDir,yDir,zDir,xTip,yTip,zTip,ReconstructionAngle,RA,ALT,RecoEnergy, TruthEnergy,ARM_val;
  // Define the branches of the TTree.
    // Unique Identifiers of Compton Series
  tree->Branch("Run", &RunNumber, "Run/I");
  tree->Branch("Event", &EventNumber, "Event/I");
    // We need to convert the string of Escape type to an integer. 0 for AllIn. 1 for Escape
  tree->Branch("EventType", &escape, "EventType/I");
    // Define the Axis of the cone
  tree->Branch("xDir",&xDir,"xDir/D");
  tree->Branch("yDir",&yDir,"yDir/D");
  tree->Branch("zDir",&zDir,"zDir/D");
    // Define the tip of the cone
  tree->Branch("xTip",&xTip,"xTip/D");
  tree->Branch("yTip",&yTip,"yTip/D");
  tree->Branch("zTip",&zTip,"zTip/D");
    // Define the reconstruction Angle of the cone
  tree->Branch("RecoAngle",&ReconstructionAngle,"RecoAngle/D");
    // Define the reconstructed energies and true original energy of gamma ray
  tree->Branch("RecoEnergy",&RecoEnergy,"RecoEnergy/D");
  tree->Branch("TruthEnergy",&TruthEnergy,"TruthEnergy/D");
  for ( auto Series = ScatterSeries.begin(); Series != ScatterSeries.end(); ++Series ){
    // Sort Series by time
    sort((*Series).second.begin(), (*Series).second.end(),[](std::tuple<double,double,double,double,double,double,double,double,double,double,std::string> a,
    std::tuple<double,double,double,double,double,double,double,double,double,double,std::string> b){
    return (std::get<1>(a) < std::get<1>(b));});
    // Extract ID and Events
    auto id =  (*Series).first;
    RunNumber = std::get<0>(id);
    EventNumber = std::get<1>(id);
    auto Events = (*Series).second;
    std::string EscapeType = std::get<10>(Events[0]);
    std::cout << EscapeType << std::endl;
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
    std::vector<std::tuple<double,double,double>> AdjacentDist;
    std::vector<double> KEs;
    AdjacentDist = Distances(Events);
    KEs = KineticEnergies(Events);
  // Calculate Reconstruction Angle (if possible)
  // Check if Reconstruction Angle could be calculated
    if (!RecoAngle(AdjacentDist,KEs,ReconstructionAngle,escape)){
        continue;
    }
  // Calculate ARM
    if (!ARM(ReconstructionAngle, AdjacentDist, truthLoc,ARM_val, SourceType)){
        continue;
    }
  // Calculate Reconstructed Energy
    RecoEnergy = 0;
    for(auto KE: KEs){RecoEnergy += KE;}
    // Original Gamma Ray Energy
    TruthEnergy = std::get<0>(Events[0]);
    // Save Axis and Tip of Cone
    xDir = std::get<0>(AdjacentDist[0]);
    yDir = std::get<1>(AdjacentDist[0]);
    zDir = std::get<2>(AdjacentDist[0]);
    xTip = std::get<2>(Events[1]);
    yTip = std::get<3>(Events[1]);
    zTip = std::get<4>(Events[1]);
  // Add NTuple to TTree
    tree->Fill();
  }
}
