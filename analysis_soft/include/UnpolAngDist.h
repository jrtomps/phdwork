#ifndef UnpolAngDist_H
#define UnpolAngDist_H

#include "Rtypes.h"
#include "Parameters.h"
#include <vector>
#include <string>
#include "MDataRange.h"

#include "Math/IParamFunction.h"
#include "TFoamIntegrand.h"

//! The parameterized fragment angular distribution
/*!
  * \f[
  *     W(\theta, \phi) = a + b sin^2 (\theta) + c sin^2 (2 \theta)
  *  \f]
  *
  * To protect the ability to properly evaluate the function, a range constraint can be specified.
  * When the constraint is applied, any evaluation of the function checks first if the values are
  * within the allowed range. If not, an exception is thrown, otherwise, the function evaluates as normal.
  */
class UnpolAngDist : public ROOT::Math::IParametricFunctionMultiDim, public TFoamIntegrand
{
private:
    std::vector<Double_t> fPars;
    std::vector<std::string> fParNames;
    ROOT::Fit::MDataRange fRanges;
    Bool_t fRangeIsConstrained;

public:
    //! Default constructor
    /*!
      * The a, b, and c coefficients are intialized to 1.
      * The datarange is initialized to theta = (0,180 deg)
      * and phi=(-180 deg, 180 deg).
      * By default, the range constraint is specified.
      *
      */
    UnpolAngDist();

    //! Parametrized constructor
    /*! Set the values of the a, b, and c coefficients upon
      * construction. These are then stored internally as default
      * parameters. When operator()(const Double_t*) is called as
      * opposed to operator()(const Double_t*, const Double_t*), the
      * stored parameters are used.
      */
    UnpolAngDist(Double_t a, Double_t b, Double_t c);

    //! Constructor from a Parameters object
    /*!
      * The parameters passed in are used to initialize the internally
      * stored default parameters.
      */
    UnpolAngDist(const ::Parameters& params);

    //! Copy Constructor
    UnpolAngDist(const UnpolAngDist& rhs);

    //! Assignment operator
    UnpolAngDist& operator=(const UnpolAngDist& rhs);

    virtual ~UnpolAngDist();

    virtual UnpolAngDist* Clone() const;
    virtual UInt_t NDim() const { return 2;}

    virtual UInt_t NPar() const { return 3;}
    virtual const Double_t* Parameters() const;
    virtual std::string ParameterName(UInt_t index) const;
    virtual void SetParameters(const Double_t* pars);

    void SetParameters(Double_t a, Double_t b, Double_t c);

    //! Get flag specifying whether to implement a range constraint
    Bool_t GetConstrainedRange(void) const;

    //! Set whether the range should be constrained.
    void SetConstrainedRange(Bool_t yn=true);

    //! Compatibility with the TFoamIntegrand
    /*! This allows this multidimensional function to be used easily
      * with the TFoam multidimensional monte-carlo sampling algorithm
      * as a probability distribution.
      */
    Double_t Density(Int_t ndim, Double_t* x);

    //! Access the data range stored by this object
    /*!
      * @returns the data range that is used when the range is constrained.
      */
    const ROOT::Fit::MDataRange* GetDataRange() const  {return &fRanges;}

private:
    Double_t DoEvalPar(const Double_t *x_deg, const Double_t *pars) const;

};

#endif // UnpolAngDist_H
