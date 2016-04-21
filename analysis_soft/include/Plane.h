#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include "Rtypes.h"
#include "Math/IParamFunction.h"

struct ParamMismatch{};


//! Plane
/*!
  *  Assumes parameters a, b, c, and d for the plane equation
  *   d = a*x + b*y + c*z

  * in other words,
  *  a is the slope along x
  *  b is the slope along y,
  *  c is the slope along z,
  *  d/c is the z intercept.
  */

class Plane : public ROOT::Math::IParametricFunctionMultiDim
{
public:
    const UInt_t fNPars;
    const UInt_t fNDim;
private:
    std::vector<Double_t> fPars;

public:
    Plane();
    Plane(const std::vector<Double_t>& params);
    Plane(const Plane& obj);
    Plane& operator=(Plane obj);
    virtual ~Plane();

    // Methods needing implementation from ROOT::Math::IBaseFunctionMultiDim
    virtual Plane* Clone() const;
    virtual UInt_t NDim() const;

    // Methods needing implementation from ROOT::Math::IBaseParam
    virtual const Double_t* Parameters() const;
    virtual void SetParameters(const Double_t* p);
    virtual UInt_t NPar() const;

private:
    virtual Double_t DoEvalPar(const Double_t* x, const Double_t* p) const;

};

#endif // PLANE_H
