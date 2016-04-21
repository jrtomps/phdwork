#include <algorithm>
#include <iostream>
#include <vector>
#include "Fit/DataRange.h"
#include "MDataRange.h"

ROOT::Fit::MDataRange::MDataRange(unsigned int dim)
    : ROOT::Fit::DataRange(dim)
{}

ROOT::Fit::MDataRange::MDataRange(double xmin, double xmax)
    : ROOT::Fit::DataRange(xmin,xmax)
{}

ROOT::Fit::MDataRange::MDataRange(double xmin, double xmax,
                       double ymin, double ymax)
                           : ROOT::Fit::DataRange(xmin,xmax, ymin,ymax)
{}

ROOT::Fit::MDataRange::MDataRange(double xmin, double xmax,
                                  double ymin, double ymax,
                                  double zmin, double zmax)
                                      : ROOT::Fit::DataRange(xmin,xmax, ymin,ymax, zmin, zmax)
{}

bool ROOT::Fit::MDataRange::AllAreInside(const std::vector<double> &x) const
{
    bool flag = true;
    if (x.size()!=NDim())
    {
        std::cerr << "Warning in <MDataRange::AllAreInside> dimension mismatch" << std::endl;
    }

    for (unsigned int i=0; i<std::max(x.size(), NDim()); i++)
    {
        flag = (flag && IsInside(x[i],i));
    }
    return flag;
}

void ROOT::Fit::MDataRange::EnsureInRange(double& x, const unsigned int icoord) const
{
    unsigned int ic = icoord;
    if (IsInside(x, ic)) return;
    else
    {
        double low,high;
        GetRange(ic,low,high);

        if (x<low)
        {
#ifdef DEBUG
            std::cerr << "x=" << x
                    << " not in range (" << low << "," << high  << ")"
                    << std::endl;
#endif
            x=low;
        }
        else // x>high
        {
            x=high;
        }

    }
}


