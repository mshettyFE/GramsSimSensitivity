#ifndef GenSkyMap
#define GenSkyMap 1

#include <TH2D.h>

TH2D ConeToSkyMap(std::tuple<double,double,double,double,double,double,double> &Cone, int RA_Bins, int ALT_Bins, int NPts,
double sphere_rad=600,std::string title = "Reconstructed Sky Map");
void SaveImage(TH2D &SkyMap,std::string pic_name="Default.png");
#endif