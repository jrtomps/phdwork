#ifndef GSBOUNDARYFINDER_H
#define GSBOUNDARYFINDER_H

#include "TCutG.h"
#include "GridSorter.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<GridSorter>;
}
#endif
#include <vector>
#include <ostream>
#include <utility>


class GSBoundaryFinder
{
private:
    TCutG* fBoundary;
    boost::shared_ptr<GridSorter> fGS;

public:
    GSBoundaryFinder(const boost::shared_ptr<GridSorter>& gsp);
    GSBoundaryFinder(const GSBoundaryFinder& obj);
    GSBoundaryFinder& operator=(GSBoundaryFinder obj);
    friend void swap(GSBoundaryFinder& lhs, GSBoundaryFinder& rhs) throw();

    ~GSBoundaryFinder() throw();

    TCutG* GetBoundary();
    std::pair<std::vector<UInt_t>, std::vector<UInt_t> > GetBounds();
    void WriteBoundaryTo(std::ostream& stream, std::string header);

private:
    void FindBoundary();
    void FormBoundary(std::vector<UInt_t>& lows, std::vector<UInt_t>& highs);
};

inline
void swap(GSBoundaryFinder& lhs, GSBoundaryFinder& rhs) throw()
{
    std::swap(lhs.fBoundary, rhs.fBoundary);
    boost::swap(lhs.fGS, rhs.fGS);
}


#endif // GSBOUNDARYFINDER_H
