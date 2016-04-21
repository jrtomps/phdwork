#ifndef CONSISTENTAXISCONVERTER_H
#define CONSISTENTAXISCONVERTER_H

#include "TH1.h"

namespace ROOTUtils
{

class ConsistentAxisConverter
{
    UInt_t fNbins;
    Double_t fXlow;
    Double_t fXhi;

public:
    ConsistentAxisConverter(const UInt_t nbins, const Double_t xlow, const Double_t xhi);

    TH1* operator()(const TH1* h) const;
};
}
#endif // CONSISTENTAXISCONVERTER_H
