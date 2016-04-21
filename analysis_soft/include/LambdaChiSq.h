#ifndef LambdaChiSq_H
#define LambdaChiSq_H

#include "Rtypes.h"
#include <vector>
#include <map>
#include <ostream>
#include <utility>
#include "TH2.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#include "ConverterMultiDim.h"
#include "ProductIntegrand.h"
#else
namespace boost {
    struct shared_ptr<ROOT::Math::IParametricFunctionMultiDim>;
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
    struct shared_ptr<ROOT::Math::IBaseFunctionOneDim>;
    struct shared_ptr<ROOT::Math::IntegratorMultiDim>;
    struct shared_ptr<ConverterMultiDim>;
    struct shared_ptr<ProductIntegrand>;
}

namespace std
{
    struct vector<std::shared_ptr<ProductIntegrand> >;
}
#endif
#include "TGraph2DErrors.h"


//! A class to compute the ChiSq between a dataset and an integrated surface
/*!
  * Gives the ability to compare an integrated region with a data point. This
  * makes it possible to incorporate the finite geometry effects when fitting
  * the experimental yield.
  *
  * It calculates the following
  *
  * \f[
  *      \chi^2 = \frac{1}{\nu} \sum_i \frac{ Y_i - y_i } { \sigma_{Y_i} }
  * \f]
  * where \f$ Y_i = \int_{4\pi} W(\theta,\phi) h(\theta,\phi) \eta(\theta, \phi) \sin \theta d\phi d\theta \f$ with V denoting an
  * integration region.
  */

class LambdaChiSq : public ROOT::Math::IBaseFunctionMultiDim
{

private:
    std::shared_ptr<ROOT::Math::IParametricFunctionMultiDim> fFunc;
    std::shared_ptr<ConverterMultiDim> fConversion;
    std::shared_ptr<ROOT::Math::IntegratorMultiDim> fIntegrator;
    std::vector<std::shared_ptr<ProductIntegrand> > fIntegrands;

    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    std::vector<Double_t> fZ;

    std::vector<Double_t> fEX;
    std::vector<Double_t> fEY;
    std::vector<Double_t> fEZ;

    std::vector<TH2*> fLambdas;
    std::map<UInt_t, Int_t> fPointToRegionMap;

private:
    //! No default constructor
    LambdaChiSq();

public:
    //! Constructor
    /*!
      * Construct the LambdaChiSq with a TGraph2DErrors pointer,
      * a generic parametric function, and a list of the regions that
      * correspond to the data points. The TGraph2DErrors is not remembered by
      * this object. However, the data that it contained is copied into internal
      * buffers owned by this object. All points with z=0 are discarded.
      * @param gr is the data set to compare
      * @param weighted_angdist is a parameterized function whose integral will be compared to the data set
      * @param regions is the set of regions to integrate over for each data point
      */
    LambdaChiSq(TGraph2DErrors* gr,
                const ROOT::Math::IParametricFunctionMultiDim& angdist,
                const std::vector<TH2*>& solang_pdfs);

    //! Copy constructor
    LambdaChiSq(const LambdaChiSq& obj);

    //! Assignment operator
    /*! uses the copy-and-swap idiom */
    LambdaChiSq& operator=(LambdaChiSq obj);

    //! swap function to implement the copy-and-swap idiom
    friend void swap(LambdaChiSq& lhs, LambdaChiSq& rhs);

    //! Destructor
    virtual ~LambdaChiSq();

    //! Access the Integrator
    /*!
      * Returns a std::shared_ptr to the RegionIntegratorMultiDim object
      * held by this class. As a shared_ptr, it passes a shared_ptr to ensure that
      * the reference counting scheme is broken.
      */
    std::shared_ptr<ROOT::Math::IntegratorMultiDim>& GetIntegrator();

    //! Set the integrator
    /*!
      * Replace the default integrator with an externally defined integrator
      * @param integrator is the std::shared_ptr to new integrator
      */
    void SetIntegrator(const std::shared_ptr<ROOT::Math::IntegratorMultiDim>& integrator);

    virtual LambdaChiSq* Clone() const;
    virtual UInt_t NDim() const;

    //! Access the mapping between the data set points and the regions
    /*! By default, the mapping is treated simply. It is assumed that the data set
      * stored in the TGraph2DErrors is sorted identically to the regions. However,
      * because the zeroes are removed from the internal data buffers, the mapping doesn't
      * remain as simple.
      */
    const std::map<UInt_t, Int_t>& GetPointToRegionMap() const;

    /*! Set the mapping. */
    void SetPointToRegionMap(const std::map<UInt_t, Int_t>& p2rmap);

private:
    virtual Double_t DoEval(const double *pars) const;
    void ConstructTheIntegrands();
    void ConvertAxesOnSolAngHistsToRadians();
    void CopyDataFromGraph(TGraph2DErrors* gr);
    void RemoveZeroesFromData(void);
};


inline void swap(LambdaChiSq &lhs, LambdaChiSq &rhs)
{
    std::swap(lhs.fFunc,rhs.fFunc);
    std::swap(lhs.fConversion,rhs.fConversion);
    std::swap(lhs.fIntegrator,rhs.fIntegrator);
    std::swap(lhs.fIntegrands, rhs.fIntegrands);

    using std::swap;
    swap(lhs.fX,rhs.fX);
    swap(lhs.fY,rhs.fY);
    swap(lhs.fZ,rhs.fZ);

    swap(lhs.fEX,rhs.fEX);
    swap(lhs.fEY,rhs.fEY);
    swap(lhs.fEZ,rhs.fEZ);

    swap(lhs.fLambdas, rhs.fLambdas);
    swap(lhs.fPointToRegionMap, rhs.fPointToRegionMap);

}

#endif // LambdaChiSq_H
