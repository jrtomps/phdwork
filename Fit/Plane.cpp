#include <vector>
#include <algorithm>
#include "Rtypes.h"
#include "Math/IParamFunction.h"
#include "Plane.h"

Plane::Plane()
    : ROOT::Math::IParametricFunctionMultiDim(),
    fNPars(4),
    fNDim(2),
    fPars(fNPars,0)
{}

Plane::Plane(const std::vector<Double_t>& params)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fNPars(4),
    fNDim(2),
    fPars(fNPars,0)
{
    if (params.size()==fNPars)
        SetParameters(&params[0]);
    else
        throw ParamMismatch();

}

Plane::Plane(const Plane& obj)
    : ROOT::Math::IParametricFunctionMultiDim(obj),
    fNPars(obj.fNPars),
    fNDim(obj.fNDim),
    fPars(fNPars,0)
{
    if (obj.fPars.size() == fNPars)
        fPars = obj.fPars;
    else
        throw ParamMismatch();
}

Plane& Plane::operator=(Plane that)
{
    fPars.swap(that.fPars);
    ROOT::Math::IParametricFunctionMultiDim::operator=(that);

    return *this;
}

Plane::~Plane()
{}

Plane* Plane::Clone() const
{
    return new Plane(*this);
}

UInt_t Plane::NDim() const
{
    return fNDim;
}

const Double_t* Plane::Parameters() const
{
    return &fPars[0];
}

void Plane::SetParameters(const Double_t *p)
{
    std::copy(p, p+fNPars, fPars.begin());
}

UInt_t Plane::NPar() const
{
    return fNPars;
}

Double_t Plane::DoEvalPar(const Double_t *x, const Double_t *p) const
{
    if (p[2]!=0)
        return (p[3]-p[0]*x[0]-p[1]*x[1])/p[2];
    else
        return 0;
}


