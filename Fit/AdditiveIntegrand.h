#ifndef AdditiveIntegrand_H
#define AdditiveIntegrand_H

#include "Rtypes.h"
#include "Math/IFunction.h"
#include <vector>
#include <utility>

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}

namespace std
{
    struct vector<std::pair<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>, Double_t> >;
}

#endif

class AdditiveIntegrand : public ROOT::Math::IBaseFunctionMultiDim
{

private:
    std::vector<std::pair<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>, Double_t> > fFuncs;

public:
    AdditiveIntegrand();
    AdditiveIntegrand(const std::vector<std::pair<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>, Double_t> >& funcs);
    AdditiveIntegrand(const AdditiveIntegrand& obj);
    friend void swap(AdditiveIntegrand& lhs, AdditiveIntegrand& rhs);
    AdditiveIntegrand& operator=(AdditiveIntegrand obj);

    AdditiveIntegrand* Clone() const;
    UInt_t NDim() const;

    void AddFunction(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& func, Double_t scale);
    void PrintFunction(void) const;

    std::vector<std::pair<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>, Double_t> >& GetFuncs();

private:
    Double_t DoEval(const Double_t* x) const;
};

inline void swap(AdditiveIntegrand &lhs, AdditiveIntegrand &rhs)
{
    std::swap(lhs.fFuncs, rhs.fFuncs);
}

#endif // AdditiveIntegrand_H
