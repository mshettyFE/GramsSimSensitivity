#include "TH2D.h"
#include "TCanvas.h"

#include<cstring>
#include<cmath>
#include<iostream>

#include "RecoEntry.h"

// The Eigen Import
#include <eigen3/Eigen/Dense>

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

Eigen::Matrix3d CreateRotMat(Eigen::Vector3d Axis ,int NPts){
  Eigen::Matrix3d RotationMat;
  // Define cos_theta to be what angle you rotate the Current vector around the axis
  // We discretize 2pi into N segments
  double pi = acos(-1);
  double arg  = 2.0*pi/(double) NPts;
  double cos_theta = cos(arg);
  double sin_theta = sin(arg);
  double subtracted_cos = 1-cos_theta;
  // https://en.wikipedia.org/wiki/Rotation_matrix under section Rotation Matrix from axis and angle
  RotationMat << cos_theta+Axis(0)*Axis(0)*subtracted_cos, Axis(0)*Axis(1)*subtracted_cos-Axis(2)*sin_theta, Axis(0)*Axis(2)*subtracted_cos+Axis(1)*sin_theta,
  Axis(1)*Axis(0)*subtracted_cos+Axis(2)*sin_theta, cos_theta+Axis(1)*Axis(1)*subtracted_cos, Axis(1)*Axis(2)*subtracted_cos-Axis(0)*sin_theta,
  Axis(2)*Axis(0)*subtracted_cos-Axis(1)*sin_theta, Axis(2)*Axis(1)*subtracted_cos+Axis(0)*sin_theta, cos_theta+Axis(2)*Axis(2)*subtracted_cos;
  return RotationMat;
}

Eigen::Vector3d Project( Eigen::Vector3d Current, Eigen::Vector3d Tip,double sphere_rad){
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
    // We normalize the projected vector since we only care about the RA and ALT of the vector, hence normalizing makes these calcs easier
    Proj.normalize();
    return Proj;
}

double reweight_energy(double energy, TH1D* Reference, TH1D* Physical, double epsilon=1E-7){
  // Takes in normalized histograms and returns the weight that needs to be assigned to 
  double ProbRef = Reference->GetBinContent(Reference->FindBin(energy));
  double ProbPhysical = Physical->GetBinContent(Physical->FindBin(energy));
  double output;
  if (ProbRef<epsilon){
    std::cout << "Reference Probability too small. Defaulting to 1" << std::endl;
    output = 1.0;
  }
  else{
    output = ProbPhysical/ProbRef; 
  }
  //  std::cout << energy<< '\t' <<  ProbPhysical << '\t' << ProbRef << '\t'<<  output << std::endl;
  return output;
}

double reweight_duration(TH1D* EffArea,TH1D* EnergyDepFlux, double exposure_time, long NEvents){
  // Assumes that you can multiply EffArea and EnergyDepFlux, we take the integral of the product
  TH1D Multiplied;
    Multiplied = (*EffArea)*(*EnergyDepFlux);
  // integrated_flux now has units of 1/(s sr)
  double integrated_flux = Multiplied.Integral( "width");
//  double projected_photons = (integrated_flux*exposure_time*4*acos(-1));
//  double weight = projected_photons/((double) NEvents);
  //  std::cout << EnergyDepFlux->Integral("width") << '\t' << integrated_flux <<  std::endl;
  //  std::cout << integrated_flux << '\t' << exposure_time << '\t' << 4*acos(-1) << '\t' <<  projected_photons << '\t' << NEvents << std::endl;
  // We multiply by the exposure time and 4*pi steradians, and then divide by NEvents casted to a double. This give the weight
  return (integrated_flux*exposure_time*4*acos(-1))/((double) NEvents);
}

TH2D ConeToSkyMap(GramsRecoEntry &Cone, 
int RA_Bins, int ALT_Bins, int NPts,double weight,double sphere_rad,std::string title){
  // Tip of cone where all surface vector eminate from
    double pi = acos(-1);
    TH2D SkyMap = TH2D("SkyMap",title.c_str(),RA_Bins, -pi, pi, ALT_Bins, -pi/2.0, pi/2.0);
    double RecoAngle = Cone.get_RecoAngle();

  // Tip of Cone
    Eigen::Vector3d Tip(Cone.get_XTip(), Cone.get_YTip(), Cone.get_ZTip());
  // Axis of Cone
    Eigen::Vector3d Axis(Cone.get_XDir(), Cone.get_YDir(), Cone.get_ZDir());
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

  Eigen::Matrix3d Rot = CreateRotMat(Axis,NPts);
  for(int pt=0; pt<NPts; ++pt){
    Current = Rot*Current;
    Eigen::Vector3d Proj = Project(Current,Tip,sphere_rad); 
  //    std::cout << Proj.dot(Proj) << '\t' << sphere_rad*sphere_rad <<  std::endl;
    double RA_val = atan2(Proj(1),Proj(0));
    double ALT_val = asin(Proj(2));
  //    std::cout <<pt << '\t' <<Proj(0) << '\t'<<Proj(1) << '\t'<< Proj(2) << '\t' << RA_val << '\t' << ALT_val << std::endl;
    int bin_num = SkyMap.FindBin(RA_val,ALT_val);
    if (SkyMap.GetBinContent(bin_num)<=0){
        SkyMap.SetBinContent(bin_num,weight);
    } 
  }
  return SkyMap;
}

/*
TH2D ConeToSkyMap(double xDir, double yDir, double zDir, double xTip, double yTip, double zTip, double RecoAngle, 
int RA_Bins, int ALT_Bins, int NPts,double weight,double sphere_rad,std::string title){
  // Tip of cone where all surface vector eminate from
    double pi = acos(-1);
    TH2D SkyMap = TH2D("SkyMap",title.c_str(),RA_Bins, -pi, pi, ALT_Bins, -pi/2.0, pi/2.0);
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

  Eigen::Matrix3d Rot = CreateRotMat(Axis,NPts);
  for(int pt=0; pt<NPts; ++pt){
    Current = Rot*Current;
    Eigen::Vector3d Proj = Project(Current,Tip,sphere_rad); 
  //    std::cout << Proj.dot(Proj) << '\t' << sphere_rad*sphere_rad <<  std::endl;
    double RA_val = atan2(Proj(1),Proj(0));
    double ALT_val = asin(Proj(2));
  //    std::cout <<pt << '\t' <<Proj(0) << '\t'<<Proj(1) << '\t'<< Proj(2) << '\t' << RA_val << '\t' << ALT_val << std::endl;
    int bin_num = SkyMap.FindBin(RA_val,ALT_val);
    if (SkyMap.GetBinContent(bin_num)<=0){
        SkyMap.SetBinContent(bin_num,weight);
    } 
  }
  return SkyMap;
}
*/

void CountsHistsWeighted(std::map<std::vector<int>, GramsRecoEntry> &ConeData,
 int NPts,
 TH1D* EffArea, TH1D* EnergyDepFlux, TH1D* ReferenceFlux, TH2D*Mask,double exposure_time, long NEvents, 
 TH1D* OutputHist, TH2D* OutputSkyMap, double proj_sphere_rad){
  // Define pi
  int RA_Bins = Mask->GetNbinsX();
  int ALT_Bins = Mask->GetNbinsY();
  // Calculate the exposure weight
  double exposure_weight = reweight_duration(EffArea,EnergyDepFlux, exposure_time,NEvents);
  // Need to normalize Energy DepFlux and ReferenceFlux before calculating energy weight
  EnergyDepFlux->Scale(1./EnergyDepFlux->Integral());
  ReferenceFlux->Scale(1./ReferenceFlux->Integral());
  // placeholder for final weight
  double weight = 0.0;
  // For each cone in Cone data, calculate the sky map for that cone
  std::string title =  "Tmp";
  for(auto Cone=ConeData.begin(); Cone!= ConeData.end(); ++Cone){
    double TEnergy = Cone->second.get_TruthEnergy();
    // Calculate the energy dependent weight
    double energy_weight = reweight_energy(TEnergy, ReferenceFlux,EnergyDepFlux); 
    weight = energy_weight*exposure_weight;
    //    std::cout << TEnergy << '\t' <<  weight << std::endl;
    TH2D AddOn = ConeToSkyMap(Cone->second,RA_Bins,ALT_Bins,NPts,weight, proj_sphere_rad, title);
    //Apply mask to new cone
    AddOn.Multiply(Mask);
    // Add generated sky map to aggregate output
    OutputSkyMap->Add(&AddOn);
    // If there is anything inside the mask, then AddOn.Integral is greater than 0. If so, fill output at TEnergy with weight
    if(AddOn.Integral() >0){
      OutputHist->Fill(TEnergy,weight);
    }
  }
}

void CountsHistsUnweighted(std::map<std::vector<int>, GramsRecoEntry> &ConeData,
 int NPts, TH2D* Mask, TH1D* OutputHist, TH2D* OutputSkyMap, double proj_sphere_rad){
  // Define pi 
  int RA_Bins = Mask->GetNbinsX();
  int ALT_Bins = Mask->GetNbinsY();
  // Fix weight to be 1
  double weight = 1.0;
  // For each cone in Cone data, calculate the sky map for that cone
  std::string title =  "Tmp";
  for(auto Cone=ConeData.begin(); Cone!= ConeData.end(); ++Cone){
    double TEnergy = Cone->second.get_TruthEnergy();
    TH2D AddOn = ConeToSkyMap(Cone->second,RA_Bins,ALT_Bins,NPts,weight, proj_sphere_rad, title);
    // Apply mask
    AddOn.Multiply(Mask);
    OutputSkyMap->Add(&AddOn);
    // Check if anything within neighborhood of background and fill if there is
    if(AddOn.Integral() >0){
      OutputHist->Fill(TEnergy,weight);
    }
  }
 }
