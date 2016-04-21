#ifndef RANDFITPARAMGENERATOR_H
#define RANDFITPARAMGENERATOR_H

#include "FitParameterGenerator.h"
#include "TRandom3.h"
#include "Parameter.h"
#include "Parameters.h"

class RandFitParamGenerator : public FitParameterGenerator
{

private:
    TRandom3 fGenerator;
    Parameters  fDefaultParams;

public:
    RandFitParamGenerator(UInt_t nparams=4);
    RandFitParamGenerator(const RandFitParamGenerator& obj);
    RandFitParamGenerator& operator=(RandFitParamGenerator obj);
    void swap(RandFitParamGenerator& obj) throw();

    ~RandFitParamGenerator() throw();

    TRandom3 GetGenerator() const;
    FitParameter GetDefaultParameter(UInt_t index) const;
    Parameters GetDefaultParameters() const;

    void SetGenerator(TRandom3 rand);
    void SetDefaultParameter(FitParameter par);
    void SetDefaultParameters(Parameters par);

    FitParameter GenerateFitParameter(UInt_t index);
    Parameters GenerateParameters();
};

#endif // RANDFITPARAMGENERATOR_H
