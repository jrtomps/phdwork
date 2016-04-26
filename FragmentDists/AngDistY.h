#ifndef AngDistY_H
#define AngDistY_H

#include "Rtypes.h"
#include "Parameters.h"
#include <vector>
#include <string>
#include "MDataRange.h"
#include "Polarity.h"

#include "Math/IParamFunction.h"
#include "TFoamIntegrand.h"

//! The fragment angular distribution dependent on cos(theta)
/*!
  * \f[
  *     W(\theta, \phi) = a + b \alpha + c \alpha x^2 + \omega P_{\gamma} cos (2 \phi) ( d \alpha - 4 c \alpha^2 )
  *  \f]
  * where \f$\alpha = 1-x^2 \f$, \f$x =cos(\theta)\f$, and the \f$\omega\f$ corresponds to the multipolarity (+1 for electric, -1 for magnetic) and \f$P_{\gamma}\f$ for
  * the polarization. I assume in this class that the polarization is 100% and also that the transition is electric in nature.
  *
  * To protect the ability to properly evaluate the function, a range constraint is enforced. Note that
  * this class doesn't give the option for relaxation of the constraint b/c cos(theta) is undefined
  * outside of the range [-1,1]. Upon evaluation, the values are first check to see if they are
  * within the allowed range. If not, an exception is thrown, otherwise, the function evaluates as normal.
  */
class AngDistY : public ROOT::Math::IParametricFunctionMultiDim, public TFoamIntegrand
{

private:
    std::vector<Double_t> fPars;
    std::vector<std::string> fParNames;
    ROOT::Fit::MDataRange fRanges;
    Double_t fP;
    Double_t fOmega;
    Double_t fO22;
    Double_t fO42;
    Double_t fSqrt10;
    Double_t fThOffset;

public:
    //! Default constructor
    /*! The parameters are initialized to 1 and the range is set to be x[0] = [-1,1]
      * and x[1]= [-pi, pi] (note that these are actually stored as radians)
      */
    AngDistY(const Double_t polarization=1.0,  POLARITY polarity=ELECTRIC, const Double_t th_offset=0);
    //! Construct with specified parameters
    /*! The same initializations occur as in the default constructor but the
      * parameters are set according to the argument values.
      * @param a is p[0]
      * @param b is p[1]
      * @param c is p[2]
      * @param d is p[3]
      */
    AngDistY(const Double_t a, const Double_t b, const Double_t c, const Double_t d, const Double_t polarization=1.0,  POLARITY polarity=ELECTRIC, const Double_t th_offset=0);

    //! Construct from a Parameters object
    /*!
      * Pass a Parameters object storing the values for a, b, c, and d
      * values to construct the object. If the Parameters object passed into this function has more or less
      * parameters than 4, the number of parameters in this parametric function,
      * the operation is considered undefined and a std::runtime_error exception is
      * thrown.
      * @param params is a Parameters object storing the parameters to be set
      */
    AngDistY(const ::Parameters& params, Double_t polarization=1.0,  POLARITY polarity=ELECTRIC, const Double_t th_offset=0);
    //! Copy constructor
    AngDistY(const AngDistY& rhs);
    //! Assignment operator
    AngDistY& operator=(const AngDistY& rhs);

    virtual ~AngDistY();

    virtual AngDistY* Clone() const;
    virtual UInt_t NDim() const { return 2;}

    virtual UInt_t NPar() const { return 4;}
//    virtual UInt_t NPar() const { return 5;}
    virtual const Double_t* Parameters() const;

    //! Get the name of a parameter
    /*! Returns the name of the parameter at index.
      * index = 0 ---> "a"
      * index = 1 ---> "b"
      * index = 2 ---> "c"
      * index = 3 ---> "d"
      *
      * @param index the index of the parameter
      * @returns the name of the indexed parameter as a std::string
      */
    virtual std::string ParameterName(UInt_t index) const;
    virtual void SetParameters(const Double_t* pars);

    void SetStandardParameters(const Double_t a, const Double_t b, const Double_t c, const Double_t d);
    void SetParameters(const Double_t c00, const Double_t c20, const Double_t c22, const Double_t c40);


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

    static std::vector<Double_t> ConvertToCCoeffs(const Double_t a, const Double_t b, const Double_t c, const Double_t d);
    std::vector<Double_t> ConvertToStandardCoeffs() const;

    static ::Parameters ConvertToStandardParameters(::Parameters& cpars);

private:
    //! Evaluate the function
    /*! This is the functionality of the inherited functions
      * operator()(const Double_t*) and operator()(const Double_t*, const Double_t*)
      *
      * @param x_rad are the values to evaluate the function at (in radians)
      * @param pars are the parameters to evaluate the function with
      */
    Double_t DoEvalPar(const Double_t *x_rad, const Double_t *pars) const;


    ClassDef(AngDistY,0);
};

#endif // AngDistY_H
