#ifndef DATASETBUILDER_H
#define DATASETBUILDER_H

#include <iostream>
#include <iomanip>
#include <functional>
#include "Rtypes.h"
#include "Math/IFunction.h"
#include "TRandom3.h"
#include "Math/DistSampler.h"
#include "TFoamIntegrandWrapper.h"
#include "TFoam.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    class shared_ptr<TFoam>;
    class shared_ptr<TFoamIntegrandWrapper>;
}
#endif

class TGraph2DErrors;
class TH2D;


//! A class to sample a function using TFoam
/*!
  * This class makes the TFoam object a bit more accessible
  * by bundling together the probability density function,
  * the random number generator (TRandom3), and the TFoam.
  * The class also provides the ability to set the function using
  * a ROOT::Math::IBaseFunctionMultiDim rather than a TFoamIntegrand,
  * since the former are much more readily available.
  *
  * In addition, this extends the ROOT::Math::DistSampler class
  * so it is legitimately a data set builder. It can generate a
  * data set using the generic MathCore/MathMore library features.
  */
class DataSetBuilder : public ROOT::Math::DistSampler
{

public:
    //! Default constructor
    /*!
      * Generates the TFoam, TRandom3, and initializes the smart ptr
      * for the probability density function to null.
      */
    DataSetBuilder();

    //! Destructor
    ~DataSetBuilder();

    //! Concrete instantiation of the ROOT::Math::DistSampler::Init()
    /*!
      * Sets the TFoam object to have 2000 cells and its strategy to Varedu
      * It also initializes the TFoam object.
      *
      * The parameter is not used.
      */
    Bool_t Init(const char *);

    //! Concrete instantiation of the ROOT::Math::DistSampler::Sample(const Double_t*)
    /*!
      * Calls TFoam::MakeEvent and then fills the parameter array with the results
      *
      * @param x is a pointer to a double array to be filled
      */
    Bool_t Sample(double *x);

    //! Set the probability distribution
    /*!
      * The parameter function is cloned and the clone is passed to the TFoamIntegrandWrapper,
      * which is ultimately passed into the TFoam object. The TFoamIntegrandWrapper is referenced
      * by a std::shared_ptr and thus can be shared with other objects outside this object.
      * The function must be 2 dimensional!!!
      *
      * @param func is a 2-dimensional function to be used as the pdf
      */
    void SetFunction(ROOT::Math::IBaseFunctionMultiDim* func);

    //! A generic implementation of the SetFunction method
    template<class Function> void SetGenFunction(Function* fun, UInt_t ndim);

    TH2D* GenerateSampleHist(UInt_t nsize);

    TGraph2DErrors* GenerateSmallSampleGraph (UInt_t nsize, Double_t rel_err=0.05);
    TGraph2DErrors* GenerateSampleGraph (UInt_t nsize, Double_t rel_err=0.05);

private:
    std::shared_ptr<TFoam> fFoam;
    TRandom3 fRndmEng;
    std::shared_ptr<TFoamIntegrandWrapper> fIntWrap;

    DataSetBuilder(const DataSetBuilder& b);
    DataSetBuilder& operator=(DataSetBuilder const& b) { return *this;};

};

template <class Function> void DataSetBuilder::SetGenFunction(Function* fun, UInt_t ndim)
{

    ROOT::Math::DistSampler::SetFunction(*fun,ndim);

    std::cout << "Set the TFoamIntegrandWrapper" << std::endl;
    fFoam->SetRho(fun);
    std::cout << "fFoam::GetRho() = "
            << std::hex << std::showbase << fFoam->GetRho()
            << std::dec << std::endl;

}
#endif // DATASETBUILDER_H
