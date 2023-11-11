#ifndef RECO_ENTRY
#define RECO_ENTRY

#include "Entry.h"
#include "UsefulTypeDefs.h"
#include <vector>
#include <tuple>


class GramsRecoEntry : public Entry{
  public:
    // MCTruth_flag, run, event, xDir,yDir,zDir,xTip,yTip,zTip,RecoAngle, TruthEnergy
    GramsRecoEntry(int,int,double,double,double,double,double,double,double,double);
    // get key
    std::vector<int> extract_key() override;

    // print
    void print() override;

    int get_run();
    int get_event();

    double get_XDir();
    double get_YDir();
    double get_ZDir();
    R3 get_Axis();

    double get_XTip();
    double get_YTip();
    double get_ZTip();
    R3 get_Tip();

    double get_RecoAngle();
    double get_TruthEnergy();



};

#endif