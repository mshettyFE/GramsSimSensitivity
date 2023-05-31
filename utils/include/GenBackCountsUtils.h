#ifndef GenSkyMap
#define GenSkyMap 1

#include "TH2D.h"

#include "eigen3/Eigen/Dense"

Eigen::Vector3d CalcPerp(Eigen::Vector3d Axis);

Eigen::Matrix3d CreateRotMat(int NPts);

Eigen::Vector3d Project( Eigen::Vector3d Current, Eigen::Vector3d Tip,double sphere_rad);

double reweight_energy(double energy, TH1D* Reference, TH1D* Background,double epsilon=1E-7);

double reweight_duration(TH1D* EffArea,TH1D* EnergyDepFlux, double exposure_time, long NEvents);

TH2D ConeToSkyMap(std::tuple<double,double,double,double,double,double,double> &Cone, int RA_Bins, int ALT_Bins, int NPts, 
double weight=1,double sphere_rad=600,std::string title = "Reconstructed Sky Map");

TH2D ConeToSkyMap(double xDir, double yDir, double zDir, double xTip, double yTip, double zTip, double RecoAngle, 
int RA_Bins, int ALT_Bins, int NPts, double weight=1,double sphere_rad=600,std::string title = "Reconstructed Sky Map");

TH2D MultipleConesToSkyMapWeighted(std::map<std::tuple<int,int>,std::tuple<double,double,double,double,double,double,double,double>> &ConeData, int RA_Bins, int ALT_Bins, int NPts,
TH1D* EffArea, TH1D* EnergyDepFlux, TH1D* ReferenceFlux, double exposure_time=3600, long NEvents=100000,
double sphere_rad=600,std::string title = "Reconstructed Sky Map");

TH2D MultipleConesToSkyMapUnweighted(std::map<std::tuple<int,int>,std::tuple<double,double,double,double,double,double,double,double>> &ConeData, int RA_Bins, int ALT_Bins, int NPts,
double sphere_rad=600,std::string title = "Reconstructed Sky Map");


void ConesToCountsWeighted(std::map<std::tuple<int,int>,std::tuple<double,double,double,double,double,double,double,double>> &ConeData, int NPts,
TH1D* EffArea, TH1D* EnergyDepFlux, TH1D* ReferenceFlux, TH2D*Mask, TH1D* OutputHist,
double exposure_time=3600, long NEvents=100000, double proj_sphere_rad=600);

#endif