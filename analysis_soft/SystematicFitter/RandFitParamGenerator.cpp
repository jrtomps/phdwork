
#include "TRandom3.h"
#include <algorithm>
#include <utility>
#include <iostream>
#include <Parameter.h>
#include "Parameters.h"
#include "FitParameterGenerator.h"
#include "RandFitParamGenerator.h"

RandFitParamGenerator::RandFitParamGenerator(UInt_t nparams)
    : FitParameterGenerator(),
    fGenerator(),
    fDefaultParams()
{
    for (UInt_t i=0; i<nparams; i++)
    {
        FitParameter fp(i,0,"",0.1,0.1,0,2.0);
        fDefaultParams.AddParameter(fp);
    }
}

RandFitParamGenerator::RandFitParamGenerator(const RandFitParamGenerator &obj)
    : FitParameterGenerator(obj),
    fGenerator(obj.fGenerator),
    fDefaultParams(obj.fDefaultParams)
{
}

RandFitParamGenerator& RandFitParamGenerator::operator=(RandFitParamGenerator obj)
{
    swap(obj);
    return *this;
}

void RandFitParamGenerator::swap(RandFitParamGenerator &obj) throw()
{
    TRandom3 rand = obj.GetGenerator();
    obj.SetGenerator(fGenerator);
    SetGenerator(rand);

    Parameters pars = obj.GetDefaultParameters();
    obj.SetDefaultParameters(fDefaultParams);
    fDefaultParams.swap(pars);
}

RandFitParamGenerator::~RandFitParamGenerator() throw()
{
#ifdef DEBUG
    std::cout << "RandFitParamGenerator::~RandFitParamGenerator()" << std::endl;
#endif
}

TRandom3 RandFitParamGenerator::GetGenerator() const
{
    return fGenerator;
}

FitParameter RandFitParamGenerator::GetDefaultParameter(UInt_t index) const
{
    Parameters::const_iterator it = fDefaultParams.find(index);

    if (it == fDefaultParams.end())
    {
        std::cerr << "Failed to find parameter at index=" << index << std::endl;
        std::cerr << "Returning first element" << std::endl;
        it = fDefaultParams.begin();
    }

    return dynamic_cast<FitParameter&>(*it->second);
}

Parameters RandFitParamGenerator::GetDefaultParameters() const
{
    return fDefaultParams;
}

void RandFitParamGenerator::SetGenerator(TRandom3 rand)
{
   fGenerator = rand;
}

void RandFitParamGenerator::SetDefaultParameter(FitParameter par)
{
    typedef std::shared_ptr<Parameter> ParamPtr;

    std::pair<UInt_t,ParamPtr> my_pair;
    my_pair = std::make_pair(par.GetIndex(),ParamPtr(new FitParameter(par)));

    std::pair<Parameters::iterator, bool> res;
    res = fDefaultParams.insert(my_pair);

    if (! res.second)
    {
        fDefaultParams.erase(res.first);
        fDefaultParams.insert(res.first, my_pair);
    }
}

void RandFitParamGenerator::SetDefaultParameters(Parameters pars)
{
    fDefaultParams.swap(pars);
}


FitParameter RandFitParamGenerator::GenerateFitParameter(UInt_t index)
{
    Parameters::iterator it = fDefaultParams.find(index);
    if (it==fDefaultParams.end())
    {
        std::cerr << "Failed to find parameter at index=" << index << std::endl;
        std::cerr << "Using first element instead" << std::endl;
        it = fDefaultParams.begin();
    }

    FitParameter& fitpar = dynamic_cast<FitParameter&>(*it->second);

    if (fitpar.GetLowerBound() != fitpar.GetUpperBound())
        fitpar.SetValue(fGenerator.Uniform(fitpar.GetLowerBound(),
                                            fitpar.GetUpperBound()));
    else
    {
        std::cout << "Warning! Default parameter has no range"
                << " --> value set to default" << std::endl;
    }

    return fitpar;
}

Parameters RandFitParamGenerator::GenerateParameters()
{
    Parameters pars;
    for (UInt_t i=0; i<fDefaultParams.size(); i++)
    {
        FitParameter fp = GenerateFitParameter(i);
        pars.AddParameter(fp);
    }

    return pars;
}
