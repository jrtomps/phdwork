#ifndef MAKEPOSITIVESMOOTHFCN_H
#define MAKEPOSITIVESMOOTHFCN_H

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

class MakePositiveSmoothFcn : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;

public:
    MakePositiveSmoothFcn(ROOT::Math::IBaseFunctionMultiDim const& func);
    MakePositiveSmoothFcn* Clone() const;
    ~MakePositiveSmoothFcn();

    Double_t DoEval(const Double_t *x) const;
    Double_t DoEval(const std::vector<Double_t>& vec) const;

    UInt_t NDim() const;

    ROOT::Math::IBaseFunctionMultiDim* GetPtrToFunction() const;

private:
    MakePositiveSmoothFcn(const MakePositiveSmoothFcn &func);
    MakePositiveSmoothFcn& operator=(MakePositiveSmoothFcn const& obj);
};
#endif // MAKEPOSITIVESMOOTHFCN_H
