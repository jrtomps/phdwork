#include "Rtypes.h"
#include "Math/IParamFunction.h"
#include "Math/ParamFunctor.h"
#include <iostream>


struct Func : public ROOT::Math::IParametricFunctionMultiDim, public ROOT::Math::ParamFunctionBase
{
    virtual UInt_t NDim() const {return 1;}
    virtual UInt_t NPar() const {return 1;}

    virtual Func* Clone() const { return new Func(*this);}

    virtual const Double_t* Parameters() const {return 0;}
    virtual void SetParameters(const Double_t*) {};

    virtual Double_t operator()(Double_t* x, Double_t* p) { return DoEvalPar(x,p);}

private:
    virtual Double_t DoEvalPar(const Double_t*, const Double_t*) const
    {
        std::cout << "Func::DoEvalPar()" << std::endl;
    }

};


Int_t main()
{

    Double_t x[2] = {1,1};
    Func().IBaseFunctionMultiDim::operator()(x);
    return 0;
}

