#ifndef TFOAMINTEGRANDWRAPPER_H
#define TFOAMINTEGRANDWRAPPER_H

#include "Math/IFunction.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    class shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif
#include "TFoamIntegrand.h"

//! Adapts ROOT::Math::IBaseFunctionMultiDim objects for usage with TFoam
/*!
  * Inherits from TFoamIntegrand !
  * These hold a point to a function passed in at construction.
  * The class provides the necessary density function requires for the
  * TFoam distribution sampler that is just a little odd to add into a
  * generic class. This is the simplest solution to make any MathCore-like
  * generic function act as a TFoamIntegrand object.
  *
  */
class TFoamIntegrandWrapper : public TFoamIntegrand
{
private:
    //! No default constructor
    /*!
      * For simplicity of implementation and to avoid problematic
      * instantiations due to incomplete definitions, this is
      * kept private.
      */
    TFoamIntegrandWrapper();

public:
    //! Construct from a generic multi-dim function
    /*!
      * The class doesn't own any object. It merely shares a pointer to
      * a multi-dim function object through the std::shared_ptr.
      */
    TFoamIntegrandWrapper(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& f);

    //! Destructor
    ~TFoamIntegrandWrapper();

    //! Called by TFoam
    /*!
      * This simply calls the operator()(const Double_t*) for the generic
      * function referenced by this class
      */
    Double_t Density(Int_t ndim, Double_t* x);

private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;

    ClassDef(TFoamIntegrandWrapper,0);

};

template <class T>
class TGenFoamIntegrand
{
public:
    TGenFoamIntegrand(const T& f) : obj(f) {}

    Double_t Density(Int_t ndim, Double_t* x)
    { return obj.Density(ndim,x);  }

private:
        T obj;
};


#endif // TFOAMINTEGRANDWRAPPER_H
