#ifndef REGIONINTEGRALCHISQ_H
#define REGIONINTEGRALCHISQ_H

#include "Rtypes.h"
#include <vector>
#include <map>
#include <ostream>
#include <utility>
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "RegionIntegratorMultiDim.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#include "ConverterMultiDim.h"
#else
namespace boost {
    struct shared_ptr<ROOT::Math::IParametricFunctionMultiDim>;
    struct shared_ptr<ROOT::Math::IBaseFunctionOneDim>;
    struct shared_ptr<RegionIntegratorMultiDim>;
    struct shared_ptr<ConverterMultiDim>;
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
  * where \f$ Y_i = \int_{V} W(\theta,\phi) \sin \theta d\phi d\theta \f$ with V denoting an
  * integration region.
  */

class RegionIntegralChiSq : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IParametricFunctionMultiDim> fFunc;
    std::shared_ptr<ConverterMultiDim> fConversion;
    std::shared_ptr<RegionIntegratorMultiDim> fIntegrator;

    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    std::vector<Double_t> fZ;

    std::vector<Double_t> fEX;
    std::vector<Double_t> fEY;
    std::vector<Double_t> fEZ;

    std::vector<Regions> fTheRegions;
    std::map<UInt_t, Int_t> fPointToRegionMap;

private:
    //! No default constructor
    RegionIntegralChiSq();

public:
    //! Constructor
    /*!
      * Construct the RegionIntegralChiSq with a TGraph2DErrors pointer,
      * a generic parametric function, and a list of the regions that
      * correspond to the data points. The TGraph2DErrors is not remembered by
      * this object. However, the data that it contained is copied into internal
      * buffers owned by this object. All points with z=0 are discarded.
      * @param gr is the data set to compare
      * @param func is a parameterized function whose integral will be compared to the data set
      * @param regions is the set of regions to integrate over for each data point
      */
    RegionIntegralChiSq(TGraph2DErrors* gr,
                        const ROOT::Math::IParametricFunctionMultiDim& func,
                        const std::vector<Regions>& regions);

    //! Copy constructor
    RegionIntegralChiSq(const RegionIntegralChiSq& obj);

    //! Assignment operator
    /*! uses the copy-and-swap idiom */
    RegionIntegralChiSq& operator=(RegionIntegralChiSq obj);

    //! swap function to implement the copy-and-swap idiom
    friend void swap(RegionIntegralChiSq& lhs, RegionIntegralChiSq& rhs);

    //! Destructor
    virtual ~RegionIntegralChiSq();

    //! Specify a conversion function to apply to the regions before integration.
    /*! A conversion may be necessary when the region is defined with a unit that
      * is incompatible with the function being integrated. For example, if the regions
      * are defined in degrees but the function requires radians. Becuase this is a
      * function rather than a scale factor, the conversion is very flexible and can
      * involve complex operations if desired.
      */
    void SetConversion(const ROOT::Math::IBaseFunctionOneDim& f);

    //! Specify a conversion function to apply to the regions before integration.
    /*! A conversion may be necessary when the region is defined with a unit that
      * is incompatible with the function being integrated. For example, if the regions
      * are defined in degrees but the function requires radians. Becuase this is a
      * function rather than a scale factor, the conversion is very flexible and can
      * involve complex operations if desired. This specifies all conversions to be
      * used for all of the dimensions of the problem.
      */

    void SetConversion(const std::shared_ptr<ConverterMultiDim> &f);
    //! Convert data of the internal buffers using the specified conversions
    /*! Rather than setting the conversion, these convert the data once and store
      * the results. The conversion function is not set as it is when the
      * SetConversion methods are called.
      */
    void ConvertData(const std::shared_ptr<ConverterMultiDim>& f);

    //! Access the Integrator
    /*!
      * Returns a std::shared_ptr to the RegionIntegratorMultiDim object
      * held by this class. As a shared_ptr, it passes a shared_ptr to ensure that
      * the reference counting scheme is broken.
      */
    std::shared_ptr<RegionIntegratorMultiDim>& GetIntegrator();

    //! Set the integrator
    /*!
      * Replace the default integrator with an externally defined integrator
      * @param integrator is the std::shared_ptr to new integrator
      */
    void SetIntegrator(const std::shared_ptr<RegionIntegratorMultiDim>& integrator);

    virtual RegionIntegralChiSq* Clone() const;
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
    typedef std::pair<Double_t, Double_t> Point;
    typedef std::vector<Point> Centers;

    virtual Double_t DoEval(const double *pars) const;
    void CopyDataFromGraph(TGraph2DErrors* gr);
    void RemoveZeroesFromData(void);
    void InitializeMap();
    Centers ComputeCenters();
    Point ComputeCenter(Regions& regions);
    Int_t ClosestRegionIndex(const Double_t x, const Double_t y, Centers& cntrs);
    Bool_t RegionEncloses(Int_t i, const Double_t x, const Double_t y);

};


/*! Overloaded extraction operator to work with the std::map type  */
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::map<T1,T2>& m)
{
    typename std::map<T1,T2>::const_iterator it = m.begin();
    while (it != m.end())
    {
        stream << "\n" << it->first << "\t" << it->second;
        it++;
    }
    return stream;
}

inline void swap(RegionIntegralChiSq &lhs, RegionIntegralChiSq &rhs)
{
    lhs.fFunc.swap(rhs.fFunc);
    lhs.fConversion.swap(rhs.fConversion);
    lhs.fIntegrator.swap(rhs.fIntegrator);

    using std::swap;
    swap(lhs.fX,rhs.fX);
    swap(lhs.fY,rhs.fY);
    swap(lhs.fZ,rhs.fZ);

    swap(lhs.fEX,rhs.fEX);
    swap(lhs.fEY,rhs.fEY);
    swap(lhs.fEZ,rhs.fEZ);

    swap(lhs.fTheRegions, rhs.fTheRegions);
    swap(lhs.fPointToRegionMap, rhs.fPointToRegionMap);

}

#endif // REGIONINTEGRALCHISQ_H
