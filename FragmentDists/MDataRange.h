#ifndef MDATARANGE_H
#define MDATARANGE_H

#include <exception>
#include <vector>
#include "Fit/DataRange.h"

namespace ROOT {
namespace Fit {

    class DataOutOfRange : public std::exception {};

    class MDataRange : public DataRange
    {
    public:
        MDataRange(unsigned int dim=1);
        MDataRange(double xmin, double xmax);
        MDataRange(double xmin, double xmax, double ymin, double ymax);
        MDataRange(double xmin, double xmax,
                   double ymin, double ymax,
                   double zmin, double zmax);
        bool AllAreInside(const std::vector<double>& x) const;

        void EnsureInRange(double& x, const unsigned int index) const;

    };

} // end Fit namespace
} // end ROOT namespace
#endif // MDATARANGE_H
