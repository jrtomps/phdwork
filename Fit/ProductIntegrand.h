#ifndef PRODUCTINTEGRAND_H
#define PRODUCTINTEGRAND_H

#include "Rtypes.h"
#include "Math/IFunction.h"
#include <vector>

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}

namespace std
{
    struct vector<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> >;
}

#endif

class ProductIntegrand : public ROOT::Math::IBaseFunctionMultiDim
{

private:
    std::vector<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> > fFuncs;

public:
    ProductIntegrand();
    ProductIntegrand(const std::vector<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> >& funcs);
    ProductIntegrand(const ProductIntegrand& obj);
    friend void swap(ProductIntegrand& lhs, ProductIntegrand& rhs);
    ProductIntegrand& operator=(ProductIntegrand obj);

    ProductIntegrand* Clone() const;
    UInt_t NDim() const;

    void AddFunction(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& func);
    std::vector<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> >& GetFuncs();

private:
    Double_t DoEval(const Double_t* x) const;
};

inline void swap(ProductIntegrand &lhs, ProductIntegrand &rhs)
{
    std::swap(lhs.fFuncs, rhs.fFuncs);
}

#endif // PRODUCTINTEGRAND_H
