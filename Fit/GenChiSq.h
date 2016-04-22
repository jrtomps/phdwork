#ifndef GENCHISQ_H
#define GENCHISQ_H

#include <vector>
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"

#include "Rtypes.h"
#include "TGraph2DErrors.h"
#include "ConverterMultiDim.h"
#include <memory>
//! Compute the chi-squared value between a data set and a generic function
/*!
  * This object shares a parametrized generic function that it uses
  * to compute a chi-squared value from. It also stores a data set that is
  * passed into it as a TGraph2DErrors pointer. The operator()(const Double_t*)
  * sets the parameters of the owned function and then computes the chi-square.
  */
class GenChiSq : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IParametricFunctionMultiDim> fFunc;
    std::shared_ptr<ConverterMultiDim> fConversion;

    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    std::vector<Double_t> fZ;

    std::vector<Double_t> fEX;
    std::vector<Double_t> fEY;
    std::vector<Double_t> fEZ;

private:
    //! No default constructor will be implemented
    GenChiSq();

public:
    //! Constructor
    /*! Construct the object from a data set stored as a TGraph2DErrors object and
      * a parametric function. The parametric function is Cloned and the new object of
      * it is owned by this object.
      *
      * @param gr is the data set
      * @param func is the parametric function
      */
    GenChiSq(TGraph2DErrors* gr, const ROOT::Math::IParametricFunctionMultiDim& func);

    //! Copy constructor
    GenChiSq(const GenChiSq& obj);
    //! Assignment operator
    /*! Uses the copy-and-swap idiom */
    GenChiSq& operator=(GenChiSq obj);
    //! the swap function for the copy-swap
    friend void swap(GenChiSq& lhs, GenChiSq& rhs);

    virtual ~GenChiSq();

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
    void SetConversion(const std::shared_ptr<ConverterMultiDim>& f);

    //! Convert data of the internal buffers using the specified conversions
    /*! Rather than setting the conversion, these convert the data once and store
      * the results. The conversion function is not set as it is when the
      * SetConversion methods are called.
      */
    void ConvertData(const std::shared_ptr<ConverterMultiDim>& f);

    virtual GenChiSq* Clone() const;
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

inline void swap(GenChiSq &lhs, GenChiSq &rhs)
{
    lhs.fFunc.swap(rhs.fFunc);
    lhs.fConversion.swap(rhs.fConversion);

    lhs.fX.swap(rhs.fX);
    lhs.fY.swap(rhs.fY);
    lhs.fZ.swap(rhs.fZ);

    lhs.fEX.swap(rhs.fEX);
    lhs.fEY.swap(rhs.fEY);
    lhs.fEZ.swap(rhs.fEZ);

}

#endif // GENCHISQ_H
