
#include <string>
#include <sstream>
#include "Rtypes.h"
#include "Parameter.h"

Parameter::Parameter()
    : fIndex(0),
    fName(),
    fValue(0),
    fError(0)
{
    SetNameFromIndex();
}

Parameter::Parameter(UInt_t index, Double_t val, std::string name, Double_t err)
    : fIndex(index),
    fName(name),
    fValue(val),
    fError(err)
{
    if (fName.length()==0) SetNameFromIndex();
}

Parameter::Parameter(const Parameter &obj)
    : fIndex(obj.fIndex),
    fName(obj.fName),
    fValue(obj.fValue),
    fError(obj.fError)
{}

Parameter& Parameter::operator=(Parameter obj)
{
    swap(obj);
    return *this;
}

void Parameter::swap(Parameter& obj) throw()
{
    Double_t temp;
    temp = obj.GetIndex();
    obj.SetIndex(fIndex);
    fIndex = temp;

    std::string tstr = obj.GetName();
    obj.SetName(fName);
    fName = tstr;

    temp = obj.GetValue();
    obj.SetValue(fValue);
    fValue = temp;

    temp = obj.GetError();
    obj.SetError(fError);
    fError = temp;
}

Parameter* Parameter::Clone() const
{
    return new Parameter(*this);
}

Parameter::~Parameter() {}

Parameter& Parameter::operator*=(const Double_t val)
{
   fValue *= val;
   fError *= val;
   return *this;
}


UInt_t Parameter::GetIndex() const { return fIndex;}
std::string Parameter::GetName() const {return fName;}
Double_t Parameter::GetValue() const { return fValue;}
Double_t Parameter::GetError() const { return fError;}

void Parameter::SetIndex(const UInt_t i) { fIndex = i;}
void Parameter::SetName(const std::string name) { fName = name;}
void Parameter::SetValue(const Double_t val) {fValue = val;}
void Parameter::SetError(const Double_t val) {fError = val;}

void Parameter::SetNameFromIndex()
{
    std::ostringstream oss("p",std::ios::app| std::ios::out);
    oss << fIndex;
    fName = oss.str();
}



//___________________________________________________
//
//  FitParameter
//
//___________________________________________________


FitParameter::FitParameter()
    : Parameter(),
    fLowerBound(0),
    fUpperBound(0),
    fStepSize(0.01),
    fConstraint(NONE)
{
}

FitParameter::FitParameter(UInt_t index, Double_t val, std::string name, Double_t err, Double_t step_size, Double_t lower_bound, Double_t upper_bound, CONSTRAINT constraint)
    : Parameter(index, val, name, err),
    fLowerBound(lower_bound),
    fUpperBound(upper_bound),
    fStepSize(step_size),
    fConstraint(constraint)
{
}

FitParameter::FitParameter(const FitParameter &obj)
    : Parameter(obj),
    fLowerBound(obj.fLowerBound),
    fUpperBound(obj.fUpperBound),
    fStepSize(obj.fStepSize),
    fConstraint(obj.fConstraint)
{}

FitParameter& FitParameter::operator=(FitParameter obj)
{
    swap(obj);
    Parameter::swap(obj);

    return *this;
}


FitParameter* FitParameter::Clone() const
{
    return new FitParameter(*this);
}

FitParameter& FitParameter::operator*=(const Double_t val)
{
    fLowerBound *= val;
    fUpperBound *= val;
    fStepSize *= val;
    Parameter::operator*=(val);

    return *this;
}

FitParameter::~FitParameter() {}

Double_t FitParameter::GetLowerBound() const {return fLowerBound;}
Double_t FitParameter::GetUpperBound() const {return fUpperBound;}
Double_t FitParameter::GetStepSize() const {return fStepSize;}
FitParameter::CONSTRAINT FitParameter::GetConstraint() const {return fConstraint;}

void FitParameter::SetLowerBound(const Double_t val) {fLowerBound = val;}
void FitParameter::SetUpperBound(const Double_t val) {fUpperBound = val;}
void FitParameter::SetStepSize(const Double_t val) {fStepSize = val;}
void FitParameter::SetConstraint(CONSTRAINT constraint) {fConstraint = constraint;}
