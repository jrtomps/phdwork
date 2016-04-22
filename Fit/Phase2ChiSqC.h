#ifndef Phase2ChiSqC_H
#define Phase2ChiSqC_H

#include "Rtypes.h"
#include <vector>
#include <map>
#include <ostream>
#include <utility>
#include "TH2.h"
#include "TGraphErrors.h"
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

class Phase2ChiSqC : public ROOT::Math::IBaseFunctionMultiDim
{
public:
    // the first is the index of the detector in the plane of polarization
    // and the second is the index of the det. perp to the plane of pol.
    typedef std::pair<UInt_t, UInt_t> SSDPair;

private:
    std::shared_ptr<ROOT::Math::IParametricFunctionMultiDim> fUnpolAngDist;
    std::shared_ptr<ROOT::Math::IParametricFunctionMultiDim> fPolPart;
    std::shared_ptr<ROOT::Math::IntegratorMultiDim> fIntegrator;
    std::vector<std::shared_ptr<ProductIntegrand> > fNumIntegrands;
    std::vector<std::shared_ptr<ProductIntegrand> > fDenomIntegrands;

    std::vector<Double_t> fX;
    std::vector<Double_t> fY;

    std::vector<Double_t> fEX;
    std::vector<Double_t> fEY;

    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fEta;
    std::vector<TH2*> fLambdas;
    std::map<UInt_t, SSDPair> fPointToRegionMap;
    std::vector<UInt_t> fIndices;
    std::vector<Double_t>* fComputedRatios;

private:
    //! No default constructor
    Phase2ChiSqC();

public:
    //! Constructor
    /*!
      * Construct the Phase2ChiSqC with a TGraph2DErrors pointer,
      * a generic parametric function, and a list of the regions that
      * correspond to the data points. The TGraph2DErrors is not remembered by
      * this object. However, the data that it contained is copied into internal
      * buffers owned by this object. All points with z=0 are discarded.
      * @param gr is the data set to compare
      * @param weighted_angdist is a parameterized function whose integral will be compared to the data set
      * @param regions is the set of regions to integrate over for each data point
      */
    Phase2ChiSqC(TGraphErrors* gr,
                const ROOT::Math::IParametricFunctionMultiDim& angdistc,
                const ROOT::Math::IParametricFunctionMultiDim& polpart,
                const ROOT::Math::IBaseFunctionMultiDim& marg_eta,
                const std::vector<TH2*>& hfunc,
                const std::vector<UInt_t>& indices);

    //! Copy constructor
    Phase2ChiSqC(const Phase2ChiSqC& obj);

    //! Assignment operator
    /*! uses the copy-and-swap idiom */
    Phase2ChiSqC& operator=(Phase2ChiSqC obj);

    //! swap function to implement the copy-and-swap idiom
    friend void swap(Phase2ChiSqC& lhs, Phase2ChiSqC& rhs);

    //! Destructor
    virtual ~Phase2ChiSqC();

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

    virtual Phase2ChiSqC* Clone() const;
    virtual UInt_t NDim() const;

    //! Access the mapping between the data set points and the regions
    /*! By default, the mapping is treated simply. It is assumed that the data set
      * stored in the TGraph2DErrors is sorted identically to the regions. However,
      * because the zeroes are removed from the internal data buffers, the mapping doesn't
      * remain as simple.
      */
    const std::map<UInt_t, Phase2ChiSqC::SSDPair >& GetPointToRegionMap() const;

    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> GetNumerator(UInt_t i);
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> GetDenominator(UInt_t i);

    void SetAllIntegrandParameters(const Double_t* pars) const;

    std::vector<Double_t> GetComputedRatios() const;
//    /*! Set the mapping. */
//    void SetPointToRegionMap(const std::map<UInt_t, SSDPair>& p2rmap);

private:
    virtual Double_t DoEval(const double *pars) const;
    void ConstructTheIntegrands();
    void CopyDataFromGraph(TGraphErrors* gr);
    void RemoveZeroesFromData(void);
    void SetUpMap();    
};


inline void swap(Phase2ChiSqC &lhs, Phase2ChiSqC &rhs)
{
    std::swap(lhs.fUnpolAngDist,rhs.fUnpolAngDist);
    std::swap(lhs.fPolPart,rhs.fPolPart);
    std::swap(lhs.fIntegrator,rhs.fIntegrator);
    std::swap(lhs.fNumIntegrands, rhs.fNumIntegrands);
    std::swap(lhs.fDenomIntegrands, rhs.fDenomIntegrands);

    using std::swap;
    swap(lhs.fX,rhs.fX);
    swap(lhs.fY,rhs.fY);

    swap(lhs.fEX,rhs.fEX);
    swap(lhs.fEY,rhs.fEY);

    swap(lhs.fEta, rhs.fEta);
    swap(lhs.fLambdas, rhs.fLambdas);
    swap(lhs.fPointToRegionMap, rhs.fPointToRegionMap);
    swap(lhs.fIndices, rhs.fIndices);
    swap(lhs.fComputedRatios, rhs.fComputedRatios);

}

#endif // Phase2ChiSqC_H
