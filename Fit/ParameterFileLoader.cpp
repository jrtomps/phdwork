#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Parameter.h"
#include "Parameters.h"
#include "ParameterFileLoader.h"


ParameterFileLoader::~ParameterFileLoader() {}

Bool_t ParameterFileLoader::LineShouldBeSkipped(const std::string& line)
{
    if ( (line.length()==0) || (line.find_first_not_of(" \t") == std::string::npos) )
        return true;
    else
        return false;
}

Bool_t ParameterFileLoader::FileIsNotOkay(std::ifstream& f)
{
    Bool_t notokay = false;
    std::ios::iostate state = f.rdstate();

    if (f.eof())
    {
        notokay = true;
    }
    else if (f.fail())
    {
        std::cerr << std::hex << std::showbase
                << "fstream is corrupted (badbit=" << (state&std::ios_base::badbit)
                << ",failbit=" << (state&std::ios_base::failbit) << ")"
                << std::dec << std::noshowbase
                << std::endl;
        notokay = true;
    }

    return notokay;
}

Parameters ParameterFileLoader::CreateParametersFromFile(std::ifstream& parfile)
{
    // expects the format of the input file to be
    // index    name    value   verr    vlow    vhigh

    Parameters params;

    if (parfile.fail())
    {
        std::cerr << "Parameter file failure" << std::endl;
        return params;
    }

    UInt_t i;
    std::string name;
    Double_t v, verr, vlow, vhigh, vstep;
    Char_t vconstr;

    Bool_t is_parameter = false;
    std::string aline;
    std::stringstream ss;
    while(1)
    {
        getline(parfile,aline);
//        std::cout << aline << std::endl;
        if ( FileIsNotOkay(parfile) )
            break;
        else if ( LineShouldBeSkipped(aline) )
            continue;
        else
        {
            is_parameter = false;
            ss.clear(); ss.str("");

            ss << aline;
            ss >> i >> name >> v >> verr;
            if (!ss.fail())
                is_parameter = true;

            ss >> vlow >> vhigh >> vstep >> vconstr;

            if (!ss.fail())
            {
                FitParameter::CONSTRAINT constr = ConvertToConstraint(vconstr);
                FitParameter p(i,v,name,verr,vstep,vlow,vhigh, constr);
                params.insert( std::make_pair(i,std::shared_ptr<Parameter>(p.Clone())) );
            }
            else if (is_parameter)
            {
                Parameter p(i,v,name,verr);
                params.insert( std::make_pair(i,std::shared_ptr<Parameter>(p.Clone())) );
            }
        }
    }

    if (! params.KeysAreSensible())
    {
        std::cerr << "Keys are not sensible!" << std::endl;
    }

    return params;
}

FitParameter::CONSTRAINT ParameterFileLoader::ConvertToConstraint(Char_t id)
{
    FitParameter::CONSTRAINT constr;
    if      (id=='n' || id=='N' || id=='0')  constr = FitParameter::NONE;
    else if (id=='l' || id=='L' || id=='1')  constr = FitParameter::LOWER;
    else if (id=='u' || id=='U' || id=='2')  constr = FitParameter::UPPER;
    else if (id=='b' || id=='B' || id=='3')  constr = FitParameter::BOTH;
    else if (id=='f' || id=='F' || id=='4')  constr = FitParameter::FIXED;
    else                                     constr = FitParameter::NONE;

    return constr;
}


