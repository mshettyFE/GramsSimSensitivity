#ifndef RECO_ENTRY
#define RECO_ENTRY

#include "Entry.h"
#include "UsefulTypeDefs.h"
#include <vector>
#include <tuple>


class GramsRecoEntry : public Entry{
  public:
    GramsRecoEntry();
    //run, event, xDir,yDir,zDir,xTip,yTip,zTip,RecoAngle, TruthEnergy
    GramsRecoEntry(int,int,double,double,double,double,double,double,double,double);
    // get key
    std::vector<int> extract_key() const override;

    // print
    void print() const override;

    int get_run() const;
    int get_event()  const;

    double get_XDir() const;
    double get_YDir() const;
    double get_ZDir() const;
    R3 get_Axis() const;

    double get_XTip() const;
    double get_YTip() const;
    double get_ZTip() const;
    R3 get_Tip() const;

    double get_RecoAngle() const;
    double get_TruthEnergy() const;
};

#endif