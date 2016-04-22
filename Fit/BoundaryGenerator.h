#ifndef BOUNDARYGENERATOR_H
#define BOUNDARYGENERATOR_H


#include "RegionIntegratorMultiDim.h"
#include "TAxis.h"

struct Point2D
{
    Float_t x;
    Float_t y;
};

class BoundaryGenerator
{
    TAxis* fXaxis;
    TAxis* fYaxis;

public:
    BoundaryGenerator(UInt_t nbins_per_axis=100);

    BoundaryGenerator(const BoundaryGenerator& obj);
    BoundaryGenerator& operator=(BoundaryGenerator obj);
    friend void swap(BoundaryGenerator& lhs, BoundaryGenerator& rhs);

    virtual ~BoundaryGenerator();

    TAxis& GetXaxis();
    TAxis& GetYaxis();

    Regions GenerateCircularBoundary(const Point2D& cntr, Double_t radius);

};

inline
void swap(BoundaryGenerator &lhs, BoundaryGenerator &rhs)
{
    std::swap(lhs.fXaxis, rhs.fXaxis);
    std::swap(lhs.fYaxis, rhs.fYaxis);
}

#endif // BOUNDARYGENERATOR_H
