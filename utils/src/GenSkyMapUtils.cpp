#include "TH2D.h"
#include "TCanvas.h"

#include<cstring>
#include<cmath>

// The Eigen Import
#include "eigen3/Eigen/Dense"

Eigen::Vector3d CalcPerp(Eigen::Vector3d Axis){
      // Somewhat convoluted method to create a perpendicular vector to the axis vector
  // We first store the origin Axis vector to remember the original ordering
    std::map<double,int> indices;
    indices.insert({Axis(0),0});
    indices.insert({Axis(1),1});
    indices.insert({Axis(2),2});
  // We now sort the components from lowest to highest on the real number line
    std::vector<double> Director;
    Director.push_back(Axis(0));
    Director.push_back(Axis(1));
    Director.push_back(Axis(2));
    sort(Director.begin(), Director.end(),[](double a,
            double b){
            return (a<b);});
  // Extracts the index of the smallest, middle, and largest compnents of the axis vector
    int min_index = indices[Director[0]];
    int inter_index = indices[Director[1]];
    int max_index = indices[Director[2]];
    // Create a unit vector that is perpendicular to the axis vector by swapping the smallest and largest values, setting  the middle value to zero, and negating largest value
    /*This works as follows:
        Assume that axis vector A=<x,y,z>. WLOG, assume that x<y<z. Construct the vector P = <-z,0,x> A dot P =0, hence P is perpendicular to A.
    */
  double normalization_fact = sqrt(Director[0]*Director[0]+Director[2]*Director[2]);
  std::vector<double> perp;
  perp.resize(3);
  perp[min_index] = -1.0*Director[2]/normalization_fact;
  perp[inter_index] = 0;
  perp[max_index] = Director[0]/normalization_fact;
  Eigen::Vector3d Output(perp[0],perp[1],perp[2]);
  return Output;
}

TH2D ConeToSkyMap(std::tuple<double,double,double,double,double,double,double> &Cone, 
int RA_Bins, int ALT_Bins, int NPts,double sphere_rad,std::string title){
  // Tip of cone where all surface vector eminate from
    double pi = acos(-1);
    TH2D SkyMap = TH2D("SkyMap",title.c_str(),RA_Bins, -pi, pi, ALT_Bins, -pi/2.0, pi/2.0);
    double xDir = std::get<0>(Cone);
    double yDir = std::get<1>(Cone);
    double zDir = std::get<2>(Cone);
    double xTip = std::get<3>(Cone);
    double yTip = std::get<4>(Cone);
    double zTip = std::get<5>(Cone);
    double RecoAngle = std::get<6>(Cone);

  // Tip of Cone
    Eigen::Vector3d Tip(xTip, yTip, zTip);
  // Axis of Cone
    Eigen::Vector3d Axis(xDir, yDir, zDir);
    Axis.normalize();
    Eigen::Vector3d perp = CalcPerp(Axis);

    // To Generate our cone, we need a vector that lies on the surface of the cone
    // We take a linear combination of the axis of the cone and the perpendicular vector
    // ie. r = cos(Angle)*Axis+sin(Angle)*Perp
    // NOTE: this assumes that both Director and Perp and normalized
    // We also normalize it because this will make projection easier (and make numerical stability verification easier)
    Eigen::Vector3d Current;
    Current = cos(RecoAngle)*Axis+sin(RecoAngle)*perp;
    Current.normalize();

  double initial_dot = Current.dot(Axis);
  // Define cos_theta to be what angle you rotate the Current vector around the axis
  // We discretize 2pi into N segments
  double arg  = 2.0*pi/(double) NPts;
  double cos_theta = cos(arg);
  double sin_theta = sin(arg);
  double subtracted_cos = 1-cos_theta;
  // https://en.wikipedia.org/wiki/Rotation_matrix under section Rotation Matrix from axis and angle
  Eigen::Matrix3d Rot;
  Rot << cos_theta+xDir*xDir*subtracted_cos, xDir*yDir*subtracted_cos-zDir*sin_theta, xDir*zDir*subtracted_cos+yDir*sin_theta,
  yDir*xDir*subtracted_cos+zDir*sin_theta, cos_theta+yDir*yDir*subtracted_cos, yDir*zDir*subtracted_cos-xDir*sin_theta,
  zDir*xDir*subtracted_cos-yDir*sin_theta, zDir*yDir*subtracted_cos+xDir*sin_theta, cos_theta+zDir*zDir*subtracted_cos;
  for(int pt=0; pt<NPts; ++pt){
    Current = Rot*Current;
    // Check to make sure that we remain on the cone (the dot produce should not change)
    //    std::cout << initial_dot << '\t' << Current.dot(Axis) << std::endl;
    //    getchar();
    // https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    // Assuming that Current remains normal overtime, we need to project all the rays onto the same sphere for accurate comparison
    // Let vec{D} be the surface vector on the cone, vec{P} be the first interaction point and R be the radius of a sphere that encompasses the detector (default 6000)
    // We  write the surface vector as r(t) = Dt+P. We want to find t such that |r(t)|^2 = R^2. This reduces to solving a quadratic equation
    // t = (-b+-sqrt(b^2-4ac))/2a. a = |D|^2 b = 2 \vec{P} dot \vec{D} and c = |P|^{2}-R^2.
    // Since D is normalized (hopefully) and we want the forward extension, this means that t = -b/2+sqrt(b^2/4-c)
    double c = Tip.dot(Tip)-sphere_rad*sphere_rad;
    double b= 2*Current.dot(Tip);
    double t = -b/2.0 + sqrt(b*b/4.0-c);
    Eigen::Vector3d Proj = Tip+t*Current;
    double norm = sqrt(Proj.dot(Proj));
    Proj = Proj/norm;
  //    std::cout << Proj.dot(Proj) << '\t' << sphere_rad*sphere_rad <<  std::endl;
    double RA_val = atan2(Proj(1),Proj(0));
    double ALT_val = asin(Proj(2));
  //    std::cout <<pt << '\t' <<Proj(0) << '\t'<<Proj(1) << '\t'<< Proj(2) << '\t' << RA_val << '\t' << ALT_val << std::endl;

    int bin_num = SkyMap.FindBin(RA_val,ALT_val);
    if (SkyMap.GetBinContent(bin_num)<=0){
        SkyMap.SetBinContent(bin_num,1);
    }
 
  }
  return SkyMap;
}

TH2D MultipleConesToSkyMap(std::map<std::tuple<int,int>,std::tuple<double,double,double,double,double,double,double>> &ConeData, int RA_Bins, int ALT_Bins, int NPts,
double sphere_rad=600,std::string title = "Reconstructed Sky Map"){
    double pi = acos(-1);
    TH2D Seed = TH2D("SkyMap",title.c_str(),RA_Bins, -pi, pi, ALT_Bins, -pi/2.0, pi/2.0);
    for(auto i=ConeData.begin(); i!= ConeData.end(); ++i){
      TH2D AddOn = ConeToSkyMap(i->second,RA_Bins,ALT_Bins,NPts, sphere_rad, title);
      Seed.Add(&AddOn);
    }
    return Seed;
}


void SaveImage(TH2D &SkyMap,std::string pic_name){
        TCanvas *c1 = new TCanvas();
        SkyMap.SetStats(0);
        SkyMap.Draw("colz");
        c1->SaveAs(pic_name.c_str());
        delete c1;
}