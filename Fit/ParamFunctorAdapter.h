#ifndef ParamFunctorAdapter_H
#define ParamFunctorAdapter_H

#include "Rtypes.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/ParamFunctor.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IParametricFunctionMultiDim>;
}
#endif

class ParamFunctorAdapter : public ROOT::Math::ParamFunctionBase
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;
    Bool_t fIsAParametricFunc;

public:
    ParamFunctorAdapter(const ROOT::Math::IBaseFunctionMultiDim& obj);
//    ParamFunctorAdapter(const ROOT::Math::IParametricFunctionMultiDim& obj);
    ParamFunctorAdapter(const ParamFunctorAdapter& obj);
    ParamFunctorAdapter& operator=(ParamFunctorAdapter obj);
    friend void swap(ParamFunctorAdapter& lhs, ParamFunctorAdapter& rhs);

    virtual ~ParamFunctorAdapter();

    virtual ParamFunctorAdapter* Clone() const;

    virtual Double_t operator()(Double_t* x, Double_t* p);

};

inline void swap(ParamFunctorAdapter &lhs, ParamFunctorAdapter &rhs)
{
    std::swap(lhs.fFunc,rhs.fFunc);
    std::swap(lhs.fIsAParametricFunc, rhs.fIsAParametricFunc);
}

#endif // ParamFunctorAdapter_H
