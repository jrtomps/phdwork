#ifndef DATASETBUILDER_HPP
#define DATASETBUILDER_HPP

#include "Math/DistSampler.h"

class DataSetBuilder : public ROOT::Math::DistSampler
{
public:
    DataSetBuilder(Double_t rel_err=0.05);

    TGraph2DErrors* GenerateSampleGraph(UInt_t nsize);

private:
    Double_t fRelErr;

};

#endif // DATASETBUILDER_HPP
