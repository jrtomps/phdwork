#ifndef SPHINTEGRAND_H
#define SPHINTEGRAND_H

#include "Rtypes.h"
#include "Math/IFunction.h"

class SphIntegrand : public ROOT::Math::IBaseFunctionMultiDim
{
private:
       ROOT::Math::IBaseFunctionMultiDim* fFunc;

   public:
       SphIntegrand (ROOT::Math::IBaseFunctionMultiDim* f);

       SphIntegrand* Clone() const { return new SphIntegrand(fFunc);}
       UInt_t NDim() const {return 2;}

   private:
       Double_t DoEval(const Double_t* x) const;

   };

#endif // SPHINTEGRAND_H
