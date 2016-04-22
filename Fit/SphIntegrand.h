#ifndef SPHINTEGRAND_H
#define SPHINTEGRAND_H

#include "Rtypes.h"
#include "Math/IFunction.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif

class SphIntegrand : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;

   public:
       SphIntegrand (const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& f);

       SphIntegrand* Clone() const { return new SphIntegrand(fFunc);}
       UInt_t NDim() const {return 2;}

   private:
       Double_t DoEval(const Double_t* x) const;

   };

#endif // SPHINTEGRAND_H
