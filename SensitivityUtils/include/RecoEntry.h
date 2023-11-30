#ifndef RECO_ENTRY
#define RECO_ENTRY

#include "Entry.h"
#include "ExtractionEntry.h"
#include "UsefulTypeDefs.h"
#include <vector>
#include <tuple>
#include "TTree.h"

class GramsRecoEntry : public Entry{
  public:
    //run, event,escape, 
    //xDir,yDir,zDir,
    //xTip,yTip,zTip,RecoAngle,ARM,RecoEnergy TruthEnergy
    GramsRecoEntry();
    GramsRecoEntry(int run,int event,int escape,
     double xDir, double yDir, double zDir,
     double xTip, double yTip, double zTip,
     double RecoAngle, double ARM, double RecoEnergy,double TruthEnergy);

    GramsRecoEntry(std::vector<GramsExtractEntry> data, std::vector<R3> AdjDists, int escape, double ReconAngle, double arm, double RecEnergy );

    // get key
    std::vector<int> extract_key() const override;

    void Fill(TTree* ntuple, int& run, int& event, int& escape, double& xDir,double& yDir,double& zDir,
    double& xTip, double& yTip,double& zTip, double& RecoAngle, double& ARM, double& RecoEnergy, double& TruthEnergy,
    bool verbose=false);

    // print
    void print() const override;

    int get_run() const;
    int get_event()  const;
    int get_escape() const;

    double get_XDir() const;
    double get_YDir() const;
    double get_ZDir() const;
    R3 get_Axis() const;

    double get_XTip() const;
    double get_YTip() const;
    double get_ZTip() const;
    R3 get_Tip() const;

    double get_RecoAngle() const;
    double get_ARM() const;
    double get_RecoEnergy() const;
    double get_TruthEnergy() const;
};

#endif