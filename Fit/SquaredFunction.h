#ifndef SQUAREDFUNCTION_H
#define SQUAREDFUNCTION_H

#include "Rtypes.h"
#include <vector>
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost {
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif
#include "Math/IFunction.h"

class SquaredFunction : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;

public:
    SquaredFunction(ROOT::Math::IBaseFunctionMultiDim const& func);
    SquaredFunction* Clone() const;
    ~SquaredFunction();

    Double_t DoEval(const Double_t *x) const;
    Double_t DoEval(const std::vector<Double_t>& vec) const;

    UInt_t NDim() const;

    ROOT::Math::IBaseFunctionMultiDim* GetPtrToFunction() const;

private:
    SquaredFunction(const SquaredFunction &func);
    SquaredFunction& operator=(SquaredFunction const& obj);
};

#endif // SQUAREDFUNCTION_H
