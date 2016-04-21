#ifndef WeightedAngDist_H
#define WeightedAngDist_H

#include "Rtypes.h"
#include "Parameters.h"
#include <vector>
#include <string>
#include "MDataRange.h"

#include "Math/IParamFunction.h"
#include "TFoamIntegrand.h"
#include "AngMarginalEta2.h"
#include "Polarity.h"

//! The parameterized fragment angular distribution
/*!
  * \f[
  *     W(\theta, \phi) = a + b sin^2 (\theta) + c sin^2 (2 \theta) + \omega P_{\gamma} cos (2 \phi) ( d sin^2(\theta) - 4 c sin^4(\theta) )
  *  \f]
  * Here the \f$\omega\f$ corresponds to the multipolarity (+1 for electric, -1 for magnetic) and \f$P_{\gamma}\f$ for
  * the polarization. I assume in this class that the polarization is 100% and also that the transition is electric in nature. However,
  * the user can specify at construction the polarity and polarization. It is considered constant and unchangable.
  *
  * To protect the ability to properly evaluate the function, a range constraint can be specified.
  * When the constraint is applied, any evaluation of the function checks first if the values are
  * within the allowed range. If not, an exception is thrown, otherwise, the function evaluates as normal.
  */
class WeightedAngDist : public ROOT::Math::IParametricFunctionMultiDim, public TFoamIntegrand
{

private:
    std::vector<Double_t> fPars;
    std::vector<std::string> fParNames;
    ROOT::Fit::MDataRange fRanges;
    Bool_t fRangeIsConstrained;
    Double_t fP;
    Double_t fOmega;
    AngMarginalEta2* fEta;

public:

    //! Parametrized constructor
    /*! Set the values of the a, b, c, and d coefficients upon
      * construction. These are then stored internally as default
      * parameters. When operator()(const Double_t*) is called as
      * opposed to operator()(const Double_t*, const Double_t*), the
      * stored parameters are used.
      */
    WeightedAngDist(Double_t a, Double_t b, Double_t c, Double_t d,
                    const AngMarginalEta2& eta,
                    Double_t polarization=1.0,  POLARITY polarity=ELECTRIC);

    //! Constructor from a Parameters object
    /*!
      * The parameters passed in are used to initialize the internally
      * stored default parameters.
      */
    WeightedAngDist(const ::Parameters& params, const AngMarginalEta2& eta,
                    Double_t polarization=1.0, POLARITY polarity=ELECTRIC);

    //! Copy Constructor
    WeightedAngDist(const WeightedAngDist& rhs);

    //! Assignment operator
    WeightedAngDist& operator=(const WeightedAngDist& rhs);

    virtual ~WeightedAngDist();

    virtual WeightedAngDist* Clone() const;
    virtual UInt_t NDim() const { return 2;}

    virtual UInt_t NPar() const { return 4;}
    virtual const Double_t* Parameters() const;
    virtual std::string ParameterName(UInt_t index) const;
    virtual void SetParameters(const Double_t* pars);

    void SetParameters(Double_t a, Double_t b, Double_t c, Double_t d);

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

    ClassDef(WeightedAngDist,0);
};

#endif // WeightedAngDist_H
