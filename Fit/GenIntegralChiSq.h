#ifndef GenIntegralChiSq_H
#define GenIntegralChiSq_H

#include <vector>
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#include "ConverterMultiDim.h"
#else
namespace boost {
    struct shared_ptr<ROOT::Math::IParametricFunctionMultiDim>;
    struct shared_ptr<ROOT::Math::IBaseFunctionOneDim>;
    struct shared_ptr<ROOT::Math::IntegratorMultiDim>;
    struct shared_ptr<ConverterMultiDim>;
}
#endif
#include "Rtypes.h"
#include "TGraph2DErrors.h"


//! A semi-generic chi square computer using a comparison of integrations
/*!
  *
  * This object shares a parametrized generic function that it uses
  * to compute a chi-squared value from. However, the function is not
  * simply evaluated, it is integrated over a simple rectangular integration region
  * that. The integration region is defined by the errors on the data set passed
  * into as a TGraph2DErrors pointer. The operator()(const Double_t*)
  * sets the parameters of the owned function and then computes the chi-square for
  * that parameterization of the function.
  */
class GenIntegralChiSq : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IParametricFunctionMultiDim> fFunc;
    std::shared_ptr<ConverterMultiDim> fConversion;
    std::shared_ptr<ROOT::Math::IntegratorMultiDim> fIntegrator;

    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    std::vector<Double_t> fZ;

    std::vector<Double_t> fEX;
    std::vector<Double_t> fEY;
    std::vector<Double_t> fEZ;

private:
    //! No default constructor
    /*!
     * To make usage of the class simple and reduce the risk of misusage,
     * the class requires the data set and the function to be passed to it
     * on initialization.
     */
    GenIntegralChiSq();

public:
    //! Construct from data set and parameterized function
    /*! Construct the object from a data set stored as a TGraph2DErrors object and
      * a parametric function. The parametric function is Cloned and the new object of
      * it is owned by this object.
      *
      * @param gr is the data set
      * @param func is the parametric function
      */
    GenIntegralChiSq(TGraph2DErrors* gr, const ROOT::Math::IParametricFunctionMultiDim& func);
    //! Copy constructor
    GenIntegralChiSq(const GenIntegralChiSq& obj);
    //! Assignment operator based on copy-and-swap idiom
    GenIntegralChiSq& operator=(GenIntegralChiSq obj);
    //! Swap function needed for copy-and-swap idiom
    friend void swap(GenIntegralChiSq& lhs, GenIntegralChiSq& rhs);

    virtual ~GenIntegralChiSq();

    //! Specify a conversion function to apply to the regions before integration.
    /*! A conversion may be necessary when the region is defined with a unit that
      * is incompatible with the function being integrated. For example, if the regions
      * are defined in degrees but the function requires radians. Becuase this is a
      * function rather than a scale factor, the conversion is very flexible and can
      * involve complex operations if desired. This specifies a conversion to be
      * used for all of the dimensions of the problem.
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

    //! Access the integrator by a smart_ptr
    std::shared_ptr<ROOT::Math::IntegratorMultiDim>& GetIntegrator();
    //! Replace the current integrator with an external integrator
    void SetIntegrator(const std::shared_ptr<ROOT::Math::IntegratorMultiDim>& integrator);

    virtual GenIntegralChiSq* Clone() const;
    //! Get dimensionality of this function
    /*!
      * This effectively returns the fFunc->NPar() because
      * the operator()(const Double_t*) actually sets the parameters
      * of the function and thus needed NDim() == fFunc->NPar()
      *
      * @returns the dimension of this object, same as fFunc->NPar()
      */
    virtual UInt_t NDim() const;

private:
    //! Called by the public operator()(const Double_t*)  virtual function
    /*! The parameter array passed by pointer are used to set the parameters
      * of the parametric function owned by this object.
      *@param pars the parameter array
      */
    virtual Double_t DoEval(const double *pars) const;
    //! Copy the data from the graph
    /*! Rather than store the TGraph2DErrors object, the object
      * copies its data into internal buffers.
      */
    void CopyDataFromGraph(TGraph2DErrors* gr);
    /*!
     * Get rid of the data that corresponds to z=0 since it is only occupying space
     */
    void RemoveZeroesFromData(void);

};

inline void swap(GenIntegralChiSq &lhs, GenIntegralChiSq &rhs)
{
    lhs.fFunc.swap(rhs.fFunc);
    lhs.fConversion.swap(rhs.fConversion);
    lhs.fIntegrator.swap(rhs.fIntegrator);

    lhs.fX.swap(rhs.fX);
    lhs.fY.swap(rhs.fY);
    lhs.fZ.swap(rhs.fZ);

    lhs.fEX.swap(rhs.fEX);
    lhs.fEY.swap(rhs.fEY);
    lhs.fEZ.swap(rhs.fEZ);

}


#endif // GenIntegralChiSq_H
