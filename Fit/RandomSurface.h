#ifndef RANDOMSURFACE_H
#define RANDOMSURFACE_H

#include "Math/IFunction.h"
#include "TRandom3.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TRandom3>;
    struct shared_ptr<ROOT::Math::IMultiGenFunction>;
}
#endif

class TGraph2DErrors;


//! Randomly select points from a surface to create a TGraph2DErrors
/*!
  * This class effectively generates pseudo data to fit for testing purposes. It is
  * somewhat specific for the generation of 2D surfaces with axes of theta and phi.
  * Given a generic multidimensional function, this will randomly sample point
  * to evaluate the function at. The results are then returned to the user as a
  * a TGraph2DErrors object. Further, it is possible to specify whether to represent
  * the data in degrees or radians.
  */
class RandomSurface
{
public:
    //! type specification for degree or radian units
    enum UNIT { Deg, Rad };

private:
    //! No default constructor for minimum definition of class
    /*!
      * No method for setting the function after instantiation
      * so this would render the class completely useless.
      */
    RandomSurface();

    //! No copy constructor
    /*!
      * If it would be useful in the future, to instantiate this
      * functionality, then it would be fine.
      */
    RandomSurface(const RandomSurface&);

    //! No assignment operator
    /*!
      * If it would be useful in the future, to instantiate this
      * functionality, then it would be fine.
      */
    RandomSurface& operator=(const RandomSurface&) { return *this;};

public:
    //! Construct from a generic multidim function
    /*!
      * The function is cloned and controlled by a std::shared_ptr.
      * Futher, a TRandom3 object is generated for the purpose of generating
      * the points to evaluate the function at.
      *
      * @param func is an implentation of the ROOT::Math::IBaseFunctionMultiDim abstract base class
      */
    RandomSurface(ROOT::Math::IMultiGenFunction const& func);

    //! Generate a a data set from the function
    /*!
      * Use the random number generator TRandom3 object to generate data points
      * uniformly in both axes. The ranges sampled are [0, pi] and [-pi, pi] for the x-
      * and y-axes, respectively. For each set of data points generated, evaluate the function
      * and then add this to a TGraph2DErrors object. The axes of the return object are
      * given in the units specified in the argument. The x-axis and y-axis errors are
      * always assigned a value of 0.1 radian.
      *
      * @param npoints the number of data points to generate
      * @param type is the unit to specify the axes on the returned graph
      * @param rel_err is the relative error to assign to z for each data point generated
      *
      * @returns a TGraph2DErrors pointer allocated on the heap. Ownership is transferred to the user.
      */
    TGraph2DErrors* GenerateSurface(UInt_t npoints, UNIT type=Deg, Double_t rel_err=0.05) const;

private:
    std::shared_ptr<TRandom3> fRand;
    std::shared_ptr<ROOT::Math::IMultiGenFunction> fFunc;
};
#endif // RANDOMSURFACE_H
