#ifndef REDUCEDDIMFUNCTION_H
#define REDUCEDDIMFUNCTION_H

#include "Rtypes.h"
#include <map>
#include <vector>
#include <algorithm>
#include <exception>
#include <ostream>
#include <sstream>
#include <iomanip>
#include "Math/IFunction.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif

struct DimensionException : public std::exception {};


//! Reduced dimensionality function
/*!
  * A ReducedDimFunction binds some dimensions of a function
  * of higher dimension to specified values. It is therefore a
  * function of reduced dimension. A ReducedDimFunction object is
  * constructed from a ROOT::Math::IBaseFunctionMultiDim const reference.
  *
  */
class ReducedDimFunction : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;
    UInt_t fNDim;
    std::map<UInt_t,UInt_t> fIndexMap;
    std::vector<Double_t> fIntVals;


private:
    /*! Prevent a default constructor*/
    ReducedDimFunction();

public:
    /*! Construct from a const reference of type ROOT::Math::IBaseFunctionMultiDim
      * and specify the desired dimensionality of the ReducedDimFunction. A clone of
      * the function is owned by the object instantiated from this class. The dimension
      * must be less than or equal to the dimensionality of the function passed and by
      * default, the first ndim dimensions are treated as variables. Any remaining dimensions
      * are set to zero.
      * @param func is a function to be controlled by this class
      * @param ndim is the reduced dimension of the function
      */
    ReducedDimFunction(const ROOT::Math::IBaseFunctionMultiDim& func, UInt_t ndim);

    //! Copy constructor
    ReducedDimFunction(const ReducedDimFunction& obj);

    //! Assignment operator
    /*!
      * Utilizes the copy-swap idiom.
      */
    ReducedDimFunction& operator=(ReducedDimFunction obj);

    //! swap function
    friend void swap(ReducedDimFunction& lhs, ReducedDimFunction& rhs) throw();

    //! Overloaded extraction operator
    /*!
      * Writes the control state to the output stream.
      * The control state is specified by the internal mapping of
      * which dimensions are fixed and which are variable.
      *
      */
    friend std::ostream& operator<<(std::ostream& stream, ReducedDimFunction& rdf);

    virtual ReducedDimFunction* Clone() const;
    virtual UInt_t NDim() const;

    /*! Set the values that the fixed dimensions are evaluated at.
      * By default these are set to zero.
      */
    template <typename Iter>
    void SetInternalValues(Iter beg, Iter end);

    /*! Set the values that the fixed dimensions are evaluated at.
      * By default these are set to zero.
      */
    void SetInternalValues(std::vector<Double_t> values);

    /*!
      * Set the control state of this object, i.e., the dimensions
      * that are held fixed and which are evaluated. In addition,
      * one can specify permutations of the dimensions
      * for example: f(x,y,z) --> f(z,y,x)
      */
    void SetIndexMap(std::map<UInt_t, UInt_t> amap);

private:
    virtual Double_t DoEval(const Double_t* x) const;
    void InitializeDefaultIndexMap();
};

template <typename Iter>
void ReducedDimFunction::SetInternalValues(Iter beg, Iter end)
{
    UInt_t i=0;
    while (beg<end && i<fIntVals.size())
    {
        fIntVals[i] = *beg;

        i++;
        beg++;
    }
}

inline void swap(ReducedDimFunction &lhs, ReducedDimFunction &rhs) throw()
{
    std::swap(lhs.fNDim,rhs.fNDim);
    lhs.fFunc.swap(rhs.fFunc);
    lhs.fIndexMap.swap(rhs.fIndexMap);
    lhs.fIntVals.swap(rhs.fIntVals);
}

inline
std::ostream& operator<<(std::ostream& stream, ReducedDimFunction& rdf)
{
    std::vector<Double_t>::iterator v_it;
    std::streamsize w = 5;

    stream << std::setw(10) << "values";
    for (v_it=rdf.fIntVals.begin(); v_it!= rdf.fIntVals.end(); v_it++)
    {
        stream << std::setw(w) << *v_it << ":";
    }

    std::ostringstream sstream;
    sstream << std::setw(10) << "index";
    std::streampos p0 = sstream.tellp();
    std::streamsize wtot = (w+1)*rdf.NDim();
    sstream << std::setw(wtot) << ' ';

    std::map<UInt_t,UInt_t>::iterator m_it;
    for (m_it=rdf.fIndexMap.begin(); m_it!=rdf.fIndexMap.end(); m_it++)
    {
        std::streampos pos = p0;
        pos += (m_it->second)*(w+1);

        sstream.seekp(pos);
        sstream << std::setw(w) << m_it->first;
    }

    stream << "\n" << sstream.str() << std::endl;

    return stream;
}



#endif // REDUCEDDIMFUNCTION_H
