#ifndef REDUCEDDIMFUNCTIONCONTROLLER_H
#define REDUCEDDIMFUNCTIONCONTROLLER_H

#include "Rtypes.h"
#include <map>
#include <vector>
#include <algorithm>
#include <exception>
#include <ostream>
#include <sstream>
#include <iomanip>
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/ParamFunctor.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif

class ReducedDimFunctionController : public ROOT::Math::IParametricFunctionMultiDim, public ROOT::Math::ParamFunctionBase
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;
    UInt_t fNDim;
    std::map<UInt_t,UInt_t> fIndexMap;
    std::vector<Double_t> fIntVals;
    std::vector<Double_t> fControlPars;

private:
    ReducedDimFunctionController();

public:
    ReducedDimFunctionController(const ROOT::Math::IBaseFunctionMultiDim& func, UInt_t ndim);
    ReducedDimFunctionController(const ReducedDimFunctionController& obj);
    ReducedDimFunctionController& operator=(ReducedDimFunctionController obj);
    virtual ~ReducedDimFunctionController();

    friend void swap(ReducedDimFunctionController& lhs, ReducedDimFunctionController& rhs) throw();
    friend std::ostream& operator<<(std::ostream& stream, ReducedDimFunctionController& rdf);

    virtual ReducedDimFunctionController* Clone() const;
    virtual UInt_t NDim() const;

    virtual UInt_t NPar() const;
    virtual const Double_t* Parameters() const;
    virtual void SetParameters(const Double_t* pars);

    template <typename Iter> void SetInternalValues(Iter beg, Iter end);
    void SetInternalValues(std::vector<Double_t> values);
    void SetIndexMap(std::map<UInt_t, UInt_t> amap);

    virtual Double_t operator()(Double_t* x, Double_t *p);

    Double_t EvalPar(Double_t* x, Double_t* p);

private:
    virtual Double_t DoEvalPar(const Double_t* x, const Double_t* p) const;
    void InitializeDefaultIndexMap();
};

template <typename Iter>
void ReducedDimFunctionController::SetInternalValues(Iter beg, Iter end)
{
    UInt_t i=0;
    while (beg<end && i<fIntVals.size())
    {
        fIntVals[i] = *beg;

        i++;
        beg++;
    }
}

inline void swap(ReducedDimFunctionController &lhs, ReducedDimFunctionController &rhs) throw()
{
    std::swap(lhs.fNDim,rhs.fNDim);
    lhs.fFunc.swap(rhs.fFunc);
    lhs.fIndexMap.swap(rhs.fIndexMap);
    lhs.fIntVals.swap(rhs.fIntVals);
    lhs.fControlPars.swap(rhs.fControlPars);
}

inline
std::ostream& operator<<(std::ostream& stream, ReducedDimFunctionController& rdf)
{
    std::vector<Double_t>::iterator v_it;
    std::streamsize w = 5;

    stream << std::setw(10) << "values";
    for (v_it=rdf.fIntVals.begin(); v_it!= rdf.fIntVals.end(); v_it++)
    {
        stream << std::setw(w) << *v_it << ":";
    }

    stream << "\n" << std::setw(10) << "params";
    for (v_it=rdf.fControlPars.begin(); v_it!=rdf.fControlPars.end(); v_it++)
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

#endif // REDUCEDDIMFUNCTIONCONTROLLER_H
