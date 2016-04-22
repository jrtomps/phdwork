#ifndef MULTIBOUNDARYGENERATOR_H
#define MULTIBOUNDARYGENERATOR_H

#include <vector>
#include "TGraph2DErrors.h"
#include "RegionIntegratorMultiDim.h"
#include "BoundaryGenerator.h"

class MultiBoundaryGenerator : public BoundaryGenerator
{
public:
    MultiBoundaryGenerator(Int_t nbins_per_axis=200);
    MultiBoundaryGenerator(const MultiBoundaryGenerator& obj);
    MultiBoundaryGenerator& operator=(MultiBoundaryGenerator obj);

    virtual ~MultiBoundaryGenerator();

    std::vector<Regions> GenerateCircularBoundaries(TGraph2DErrors* gr, Double_t radius);
};

#endif // MULTIBOUNDARYGENERATOR_H
