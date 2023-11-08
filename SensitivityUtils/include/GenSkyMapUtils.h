#ifndef GenSkyMap
#define GenSkyMap 1

#include "TH2D.h"

#include <eigen3/Eigen/Dense>

// Generate a vector that is perpendicular to the input
Eigen::Vector3d CalcPerp(Eigen::Vector3d Axis);

// Create rotation matrix used to generate cone
Eigen::Matrix3d CreateRotMat(int NPts);

// Project a vector outwards until it collides with a sphere
Eigen::Vector3d Project( Eigen::Vector3d Current, Eigen::Vector3d Tip,double sphere_rad);

// Reweight an event based on it's energy in comparison to a reference
double reweight_energy(double energy, TH1D* Reference, TH1D* Background,double epsilon=1E-7);

// Reweight an event based on the projected number of photons in comparison to the number generated
double reweight_duration(TH1D* EffArea,TH1D* EnergyDepFlux, double exposure_time, long NEvents);

//Given a compton cone, generate the associated sky map histogram
TH2D ConeToSkyMap(std::tuple<double,double,double,double,double,double,double> &Cone, int RA_Bins, int ALT_Bins, int NPts, 
double weight=1,double sphere_rad=600,std::string title = "Reconstructed Sky Map");

// Overloaded to read in variables individually
TH2D ConeToSkyMap(double xDir, double yDir, double zDir, double xTip, double yTip, double zTip, double RecoAngle, 
int RA_Bins, int ALT_Bins, int NPts, double weight=1,double sphere_rad=600,std::string title = "Reconstructed Sky Map");

// Calculate the counts and sky map within a masked region with reweighting
void CountsHistsWeighted(std::map<std::tuple<int,int>,std::tuple<double,double,double,double,double,double,double,double>> &ConeData,
 int NPts,
TH1D* EffArea, TH1D* EnergyDepFlux, TH1D* ReferenceFlux, TH2D*Mask,double exposure_time, long NEvents, 
TH1D* OutputHist, TH2D* OutputSkyMap, double proj_sphere_rad=600);

// Calculate the counts and sky map within a masked region without reweighting
void CountsHistsUnweighted(std::map<std::tuple<int,int>,std::tuple<double,double,double,double,double,double,double,double>> &ConeData,
 int NPts, TH2D*Mask, TH1D* OutputHist, TH2D* OutputSkyMap, double proj_sphere_rad=600);


#endif