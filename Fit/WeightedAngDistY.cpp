#include "WeightedAngDistY.h"
#include "AngMarginalEta2C.h"

WeightedAngDistY::WeightedAngDistY(const Eta2C& eta, Double_t polarization, POLARITY multipol)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fInteg(new ProductIntegrand)
{
    std::shared_ptr<AngDistY> angdist(new AngDistY(polarization, multipol));
    fInteg->AddFunction(angdist);

    std::shared_ptr<AngMarginalEta2C> marg_eta(new AngMarginalEta2C(eta));
    fInteg->AddFunction(marg_eta);
}

WeightedAngDistY::WeightedAngDistY(const WeightedAngDistY &obj)
    : ROOT::Math::IParametricFunctionMultiDim(obj),
    fInteg(obj.fInteg)
{
}

WeightedAngDistY* WeightedAngDistY::Clone() const
{
    return new WeightedAngDistY(*this);
}


const Double_t* WeightedAngDistY::Parameters() const
{
    ROOT::Math::IParametricFunctionMultiDim* f
            = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fInteg->GetFuncs().front().get());
    return f->Parameters();
}

std::string WeightedAngDistY::ParameterName(UInt_t i) const
{
    ROOT::Math::IParametricFunctionMultiDim* f
            = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fInteg->GetFuncs().front().get());
    return f->ParameterName(i);
}

void WeightedAngDistY::SetParameters(const Double_t *pars)
{
    ROOT::Math::IParametricFunctionMultiDim* f
            = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fInteg->GetFuncs().front().get());
    f->SetParameters(pars);
}

Double_t WeightedAngDistY::DoEvalPar(const Double_t* x, const Double_t* p) const
{
    ROOT::Math::IParametricFunctionMultiDim* f
            = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fInteg->GetFuncs().front().get());
    f->SetParameters(p);

    return (*fInteg)(x);
}
