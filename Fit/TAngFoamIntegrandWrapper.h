#ifndef TAngFoamINTEGRANDWRAPPER_H
#define TAngFoamINTEGRANDWRAPPER_H

#include "Math/IFunction.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#include <CLHEP/Units/PhysicalConstants.h>
#else
namespace boost
{
    class shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif
#include "TFoamIntegrand.h"

//! Adapts ROOT::Math::IBaseFunctionMultiDim objects for usage with TAngFoam
/*!
  * Inherits from TAngFoamIntegrand !
  * These hold a point to a function passed in at construction.
  * The class provides the necessary density function requires for the
  * TAngFoam distribution sampler that is just a little odd to add into a
  * generic class. This is the simplest solution to make any MathCore-like
  * generic function act as a TAngFoamIntegrand object.
  *
  */
class TAngFoamIntegrandWrapper : public TFoamIntegrand
{
private:
    //! No default constructor
    /*!
      * For simplicity of implementation and to avoid problematic
      * instantiations due to incomplete definitions, this is
      * kept private.
      */
    TAngFoamIntegrandWrapper();

public:
    //! Construct from a generic multi-dim function
    /*!
      * The class doesn't own any object. It merely shares a pointer to
      * a multi-dim function object through the std::shared_ptr.
      */
    TAngFoamIntegrandWrapper(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& f);

    //! Destructor
    ~TAngFoamIntegrandWrapper();

    //! Called by TAngFoam
    /*!
      * This simply calls the operator()(const Double_t*) for the generic
      * function referenced by this class
      */
    Double_t Density(Int_t ndim, Double_t* x);

private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;

};

template <class T>
class TGenAngFoamIntegrand
{
public:
    TGenAngFoamIntegrand(const T& f) : obj(f) {}

    Double_t Density(Int_t ndim, Double_t* x)
    {
        x[0] = 2.0*x[0]-1.0;
        x[1] *= CLHEP::twopi;
        return obj.Density(ndim,x);
    }

private:
        T obj;
};


#endif // TAngFoamINTEGRANDWRAPPER_H
