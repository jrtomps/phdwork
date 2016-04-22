#ifndef UNITCONVERTER_H
#define UNITCONVERTER_H

#include "Rtypes.h"
#include "Math/IFunction.h"

typedef Double_t Unit;

struct BadUnit {};

class UnitConverter : public ROOT::Math::IBaseFunctionOneDim
{
private:
    Double_t fUnit1ToUnit2_scalefactor;

public:
    UnitConverter(Unit u1, Unit u2);
    UnitConverter(const UnitConverter& obj);
    UnitConverter& operator=(UnitConverter obj);

    UnitConverter* Clone() const;

private:
    Double_t DoEval(double x) const;

};

#endif // UNITCONVERTER_H
