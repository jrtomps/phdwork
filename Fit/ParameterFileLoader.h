#ifndef ParameterFileLoader_H
#define ParameterFileLoader_H

#include <fstream>
#include <string>
#include "Parameters.h"

//! Load parameters from properly formatted files into Parameters objects
class ParameterFileLoader
{

public:
    ParameterFileLoader() {}
    ParameterFileLoader(const ParameterFileLoader& obj) {}
    ParameterFileLoader& operator=(const ParameterFileLoader& obj) { return *this;}
    ~ParameterFileLoader();

    Parameters CreateParametersFromFile(std::ifstream& parfile);

private:
    Bool_t LineShouldBeSkipped(const std::string& aline);
    Bool_t FileIsNotOkay(std::ifstream& file);
    FitParameter::CONSTRAINT ConvertToConstraint(Char_t id);

};
#endif // ParameterFileLoader_H
