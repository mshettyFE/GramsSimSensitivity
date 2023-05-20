#ifndef GenSkyMap
#define GenSkyMap 1

#include "TH2D.h"

#include "eigen3/Eigen/Dense"

Eigen::Vector3d CalcPerp(Eigen::Vector3d Axis);

Eigen::Matrix3d CreateRotMat(int NPts);

Eigen::Vector3d Project( Eigen::Vector3d Current, Eigen::Vector3d Tip,double sphere_rad);

TH2D ConeToSkyMap(std::tuple<double,double,double,double,double,double,double> &Cone, int RA_Bins, int ALT_Bins, int NPts,
double sphere_rad=600,std::string title = "Reconstructed Sky Map");
TH2D MultipleConesToSkyMap(std::map<std::tuple<int,int>,std::tuple<double,double,double,double,double,double,double>> &Cone, int RA_Bins, int ALT_Bins, int NPts,
double sphere_rad=600,std::string title = "Reconstructed Sky Map");
void SaveImage(TH2D &SkyMap,std::string pic_name="Default.png");
#endif