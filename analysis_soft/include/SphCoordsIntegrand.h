#ifndef SPHCOORDSINTEGRAND_H
#define SPHCOORDSINTEGRAND_H

#include "Rtypes.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
    struct shared_ptr<ROOT::Math::IParametricFunctionMultiDim>;
}
#endif

class SphCoordsIntegrand : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;

public:
    SphCoordsIntegrand(const ROOT::Math::IBaseFunctionMultiDim& func);
    SphCoordsIntegrand(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& func);
    SphCoordsIntegrand(const SphCoordsIntegrand& obj);
    SphCoordsIntegrand& operator=(SphCoordsIntegrand obj);
    friend void swap(SphCoordsIntegrand& lhs, SphCoordsIntegrand& rhs);

    virtual ~SphCoordsIntegrand();

    virtual UInt_t NDim() const;
    virtual SphCoordsIntegrand* Clone() const;

private:
    Double_t DoEval(const Double_t* x) const;
};


inline void swap(SphCoordsIntegrand &lhs, SphCoordsIntegrand &rhs)
{
    lhs.fFunc.swap(rhs.fFunc);
}

// ..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo.. //
//
class SphCoordsParamIntegrand : public ROOT::Math::IParametricFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IParametricFunctionMultiDim> fFunc;

public:
    SphCoordsParamIntegrand(const ROOT::Math::IParametricFunctionMultiDim& func);
    SphCoordsParamIntegrand(const SphCoordsParamIntegrand& obj);
    SphCoordsParamIntegrand& operator=(SphCoordsParamIntegrand obj);
    friend void swap(SphCoordsParamIntegrand& lhs, SphCoordsParamIntegrand& rhs);

    virtual ~SphCoordsParamIntegrand();

    virtual UInt_t NDim() const;
    virtual SphCoordsParamIntegrand* Clone() const;

    virtual UInt_t NPar() const;
    virtual const Double_t* Parameters() const;
    virtual void SetParameters(const Double_t* par);

private:
    Double_t DoEvalPar(const Double_t* x, const Double_t* p) const;
};


inline void swap(SphCoordsParamIntegrand &lhs, SphCoordsParamIntegrand &rhs)
{
    lhs.fFunc.swap(rhs.fFunc);
}



#endif // SPHCOORDSINTEGRAND_H
