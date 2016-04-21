#ifndef BOUNDARYFINDER_H
#define BOUNDARYFINDER_H

#include "Rtypes.h"
#include <map>
#include <utility>
#include <algorithm>
#include "TCutG.h"
#include "TGraph.h"
#include "GridSorter.h"

struct point_t
{
    Double_t radius;
    Double_t index;
};

class BoundaryFinder
{
private:
    TCutG* fBoundary;
    std::multimap<Double_t, point_t> fSortedVals;
    std::pair<Double_t, Double_t> fCenter;

private:
    BoundaryFinder ();

public:
    BoundaryFinder(const TGraph& gr);
    BoundaryFinder(const BoundaryFinder& obj);
    BoundaryFinder& operator=(BoundaryFinder obj);
    friend void swap(BoundaryFinder& lhs, BoundaryFinder& rhs);

    ~BoundaryFinder();

    const TCutG& GetBoundary();

private:
    void FindBoundary();
    void StoreDataFromGraph(const TGraph& gr);
    void ComputeCenter(const TGraph& gr);
};

inline void swap(BoundaryFinder& lhs, BoundaryFinder& rhs)
{
    std::swap(lhs.fBoundary, rhs.fBoundary);
    std::swap(lhs.fSortedVals,rhs.fSortedVals);
    std::swap(lhs.fCenter, rhs.fCenter);
}

#endif // BOUNDARYFINDER_H
