#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
#include "Rtypes.h"


// Parameter
//_______________________________________

class Parameter
{
public:
    UInt_t fIndex;
    std::string fName;
    Double_t fValue;
    Double_t fError;

public:
    Parameter();
    Parameter(UInt_t index,
              Double_t val=0,
              std::string name="",
              Double_t err=0);
    Parameter(const Parameter& obj);
    Parameter& operator=(Parameter obj);
    virtual ~Parameter();

    void swap(Parameter& obj) throw();

    virtual Parameter& operator*=(const Double_t val);

    virtual Parameter* Clone() const;

    UInt_t GetIndex() const;
    std::string GetName() const;
    Double_t GetValue() const;
    Double_t GetError() const;

    void SetIndex(const UInt_t i);
    void SetName(const std::string name);
    void SetValue(const Double_t val);
    void SetError(const Double_t val);

private:
    void SetNameFromIndex();

};

inline void swap(Parameter &lhs, Parameter &rhs)
{
    lhs.swap(rhs);
}

inline
std::ostream& operator<<(std::ostream& stream , Parameter& par)
{
    stream << std::setiosflags(std::ios::scientific)
            << std::setprecision(4)
            << std::right;

    stream << std::setw(3) << par.GetIndex()
            << std::setw(12) << par.GetName().substr(0,9)
            << std::setw(12) << par.GetValue()
            << std::setw(12) << par.GetError();

    stream << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);

    return stream;

}

inline std::ostream& operator<<(std::ostream& stream , std::pair<std::streamsize, Parameter> par)
{
    stream << std::setiosflags(std::ios::scientific)
            << std::setprecision(4)
            << std::right;

    stream << std::setw(par.first) << ' '
            << std::setw(3) << par.second.GetIndex()
            << std::setw(12) << par.second.GetName().substr(0,9)
            << std::setw(12) << par.second.GetValue()
            << std::setw(12) << par.second.GetError();

    stream << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);

    return stream;

}


//000000000000000000000000000000000000000000000000
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//
//     FitParameter
//________________________________________________

class FitParameter : public Parameter
{
public:
    enum CONSTRAINT { NONE=0, LOWER=1, UPPER=2, BOTH=3, FIXED=4};

private:
    Double_t fLowerBound;
    Double_t fUpperBound;
    Double_t fStepSize;
    CONSTRAINT fConstraint;

public:
    FitParameter();
    FitParameter(UInt_t index,
                 Double_t val=0,
                 std::string name="",
                 Double_t err=0,
                 Double_t step_size=0,
                 Double_t lower_bound=0,
                 Double_t upper_bound=0,
                 CONSTRAINT constraint=NONE);

    FitParameter(const FitParameter& obj);
    FitParameter& operator=(FitParameter obj);
    ~FitParameter();
    friend void swap(FitParameter& lhs, FitParameter& rhs) throw();

    virtual FitParameter& operator*=(const Double_t val);

    virtual FitParameter* Clone() const;

    Double_t GetLowerBound() const;
    Double_t GetUpperBound() const;
    Double_t GetStepSize() const;
    CONSTRAINT GetConstraint() const;

    void SetLowerBound(const Double_t val);
    void SetUpperBound(const Double_t val);
    void SetStepSize(const Double_t val);
    void SetConstraint(CONSTRAINT constraint);

};

inline void swap(FitParameter& lhs, FitParameter& rhs) throw()
{
    std::swap(lhs.fLowerBound, rhs.fLowerBound);
    std::swap(lhs.fUpperBound, rhs.fUpperBound);
    std::swap(lhs.fStepSize, rhs.fStepSize);
    std::swap(lhs.fConstraint, rhs.fConstraint);

    swap(static_cast<Parameter&>(lhs), static_cast<Parameter&>(rhs));
}

inline
std::ostream& operator<<(std::ostream& stream, FitParameter& par)
{
    // -1.00e+10
    // 012345678

    stream << std::setiosflags(std::ios::scientific)
            << std::setprecision(4)
            << std::right;

    stream << std::setw(3) << par.GetIndex()
            << std::setw(12) << par.GetName().substr(0,9)
            << std::setw(12) << par.GetValue()
            << std::setw(12) << par.GetError();

    stream  << std::setw(12) << par.GetLowerBound()
            << std::setw(12) << par.GetUpperBound()
            << std::setw(12) << par.GetStepSize()
            << std::setw(4) << par.GetConstraint();

    stream << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, std::pair<std::streamsize, FitParameter> par)
{
    // -1.00e+10
    // 012345678

    stream << std::setiosflags(std::ios::scientific)
            << std::setprecision(4)
            << std::right;

    stream << std::setw(par.first)
            << std::setw(3) << par.second.GetIndex()
            << std::setw(12) << par.second.GetName().substr(0,9)
            << std::setw(12) << par.second.GetValue()
            << std::setw(12) << par.second.GetError();

    stream  << std::setw(12) << par.second.GetLowerBound()
            << std::setw(12) << par.second.GetUpperBound()
            << std::setw(12) << par.second.GetStepSize()
            << std::setw(12) << par.second.GetConstraint();

    stream << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);

    return stream;
}


#endif // PARAMETER_H
