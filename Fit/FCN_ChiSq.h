#ifndef FCN_CHISQ_H
#define FCN_CHISQ_H

#include <vector>
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#else
namespace boost
{
class shared_ptr<TGraph2DErrors>;
class shared_array<Double_t>;
}
#endif
#include "Rtypes.h"
#include "TPolyLine.h"
#include "Minuit2/FCNBase.h"
#include "Math/IntegratorMultiDim.h"

class TGraph2DErrors;

class FCN_ChiSq : public ROOT::Minuit2::FCNBase
{
private:
    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    std::vector<Double_t> fZ;
    std::vector<Double_t> fEX;
    std::vector<Double_t> fEY;
    std::vector<Double_t> fEZ;
    void EnforceRangeLimits();
    boost::shared_array<Double_t> fIntLimitsLow;
    boost::shared_array<Double_t> fIntLimitsHigh;

public:
    FCN_ChiSq(TGraph2DErrors* gr);
    ~FCN_ChiSq();

    virtual Double_t operator() (const std::vector<Double_t> & x) const;
    virtual double Up() const;
    void CopyDataFromGraph(TGraph2DErrors* gr);
    void RemoveZeroesFromData(void);


private:
    FCN_ChiSq(const FCN_ChiSq& rhs);
    FCN_ChiSq& operator=(const FCN_ChiSq& rhs) { return *this;}
};

#endif // FCN_CHISQ_H
