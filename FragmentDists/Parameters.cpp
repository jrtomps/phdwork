#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "Parameter.h"
#include "ParameterFileLoader.h"
#include "Parameters.h"


Parameters::Parameters() : std::map<UInt_t, std::shared_ptr<Parameter> >() {}


Parameters::Parameters(const Parameter& par)
    : std::map<UInt_t, std::shared_ptr<Parameter> >()
{
    insert(std::make_pair(0,std::shared_ptr<Parameter>(par.Clone())));
}

Parameters::Parameters(std::ifstream &parfile)
    : std::map<UInt_t, std::shared_ptr<Parameter> >()
{
    *this = ParameterFileLoader().CreateParametersFromFile(parfile);
}

Parameters::Parameters(const Parameters &obj)
    : std::map<UInt_t, std::shared_ptr<Parameter> >(obj)
{}

Parameters& Parameters::operator=(Parameters obj)
{
    std::map<UInt_t, std::shared_ptr<Parameter> >::swap(obj);
    return *this;
}

void Parameters::swap(Parameters& params) throw()
{
//    swap(params);
    std::swap(params, *this);
}

Parameters::~Parameters() {}

Parameters& Parameters::operator*=(const Double_t val)
{
    for(iterator it=begin(); it!=end(); it++)
    {
        (*it->second) *= val;
    }
    return *this;
}

void Parameters::AddParameter(UInt_t index, const Parameter& par)
{
    UInt_t prev_size = size();
    if (index != par.GetIndex())
    {
        std::cout << "Index mismatch, not acceptabled" << std::endl;
        //par.SetIndex(index);
        return ;
    }

    insert(std::make_pair(index,std::shared_ptr<Parameter>(par.Clone())));

    if (size()==prev_size)
    {
        std::cerr << "Parameter insertion failed, nonunique parameter" << std::endl;
    }
}

void Parameters::AddParameter(const Parameter& par)
{
    UInt_t index = par.GetIndex();
    AddParameter(index, par);
}


Bool_t Parameters::KeysAreSensible(void) const
{
    Bool_t are_sensible=true;

    UInt_t index=0;
    for (const_iterator it=begin(); it!=end(); it++, index++)
    {
        if (index!=(*it).first)
        {
            are_sensible=false;
            break;
        }
    }

    return are_sensible;
}

