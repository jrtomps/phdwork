#ifndef MULTIDIMFUNCTIONCONTROLLER_H
#define MULTIDIMFUNCTIONCONTROLLER_H

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

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif

class MultiDimFunctionController : public ROOT::Math::IParametricFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;
    std::vector<Double_t> fControlVals;

private:
    MultiDimFunctionController();

public:
    MultiDimFunctionController(const ROOT::Math::IBaseFunctionMultiDim& func);
    MultiDimFunctionController(const MultiDimFunctionController& obj);
    MultiDimFunctionController& operator=(MultiDimFunctionController obj);
    friend void swap(MultiDimFunctionController& lhs, MultiDimFunctionController& rhs) throw();

    virtual ~MultiDimFunctionController() throw();

    virtual MultiDimFunctionController* Clone() const;
    virtual UInt_t NDim() const;

    virtual const Double_t* Parameters() const;
    virtual void SetParameters(const Double_t* pars);
    virtual UInt_t NPar() const;


    template <typename Iter> void SetControlValues(Iter beg, Iter end);
    void SetControlValues(std::vector<Double_t> values);
    void SetControlValue(UInt_t index, UInt_t val);
    void SetParameter(UInt_t index, UInt_t val);

private:
    virtual Double_t DoEvalPar(const Double_t* x, const Double_t* p) const;

};

template <typename Iter>
void MultiDimFunctionController::SetControlValues(Iter beg, Iter end)
{
    UInt_t i=0;
    while (beg<end && i<fControlVals.size())
    {
        fControlVals[i] = *beg;

        i++;
        beg++;
    }
}

inline void swap(MultiDimFunctionController &lhs, MultiDimFunctionController &rhs) throw()
{
    lhs.fFunc.swap(rhs.fFunc);
    lhs.fControlVals.swap(rhs.fControlVals);
}

//inline
//std::ostream& operator<<(std::ostream& stream, MultiDimFunctionController& rdf)
//{
//    std::vector<Double_t>::iterator v_it;
//    std::streamsize w = 5;
//
//    stream << std::setw(10) << "values";
//    for (v_it=rdf.fIntVals.begin(); v_it!= rdf.fIntVals.end(); v_it++)
//    {
//        stream << std::setw(w) << *v_it << ":";
//    }
//
//    std::ostringstream sstream;
//    sstream << std::setw(10) << "index";
//    std::streampos p0 = sstream.tellp();
//    std::streamsize wtot = (w+1)*rdf.NDim();
//    sstream << std::setw(wtot) << ' ';
//
//    std::map<UInt_t,UInt_t>::iterator m_it;
//    for (m_it=rdf.fIndexMap.begin(); m_it!=rdf.fIndexMap.end(); m_it++)
//    {
//        std::streampos pos = p0;
//        pos += (m_it->second)*(w+1);
//
//        sstream.seekp(pos);
//        sstream << std::setw(w) << m_it->first;
//    }
//
//    stream << "\n" << sstream.str() << std::endl;
//
//    return stream;
//}



#endif // MULTIDIMFUNCTIONCONTROLLER_H
