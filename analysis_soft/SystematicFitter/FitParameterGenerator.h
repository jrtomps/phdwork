#ifndef FITPARAMETERGENERATOR_H
#define FITPARAMETERGENERATOR_H

#include "Rtypes.h"
#include "Parameter.h"
#include "Parameters.h"


class FitParameterGenerator
{
public:
    virtual FitParameter GenerateFitParameter(UInt_t index) = 0;
    virtual Parameters GenerateParameters() = 0;
};

#endif // FITPARAMETERGENERATOR_H
