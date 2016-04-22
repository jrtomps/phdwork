#include "Rtypes.h"
#include "Math/IFunction.h"
#include "UnitConverter.h"

UnitConverter::UnitConverter(Unit u1, Unit u2)
    : ROOT::Math::IBaseFunctionOneDim(),
    fUnit1ToUnit2_scalefactor(0)
{
    if (u1!=0 && u2!=0)
    {
        fUnit1ToUnit2_scalefactor = u2/u1;
    }
    else
        throw BadUnit();
}

UnitConverter::UnitConverter(const UnitConverter &obj)
    : ROOT::Math::IBaseFunctionOneDim(obj),
    fUnit1ToUnit2_scalefactor(obj.fUnit1ToUnit2_scalefactor)
{}

UnitConverter& UnitConverter::operator=(UnitConverter obj)
{
    fUnit1ToUnit2_scalefactor = obj.fUnit1ToUnit2_scalefactor;
}

UnitConverter* UnitConverter::Clone() const
{
    return new UnitConverter(*this);
}

Double_t UnitConverter::DoEval(double x) const
{
    return x*fUnit1ToUnit2_scalefactor;
}
