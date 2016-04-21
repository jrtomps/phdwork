#ifndef NUCLEARCHART_H
#define NUCLEARCHART_H

#include <Rtypes.h>
#include "TObject.h"
#include "Isotope.h"
#include <iostream>
#include <string>
#include <map>

class NuclearChart
{
private:
    static NuclearChart* theInstance;
    std::map<std::string, UInt_t> fNameMap;
    std::map<UInt_t, std::string> fZMap;

public:
    static NuclearChart* GetInstance();

    Isotope ParseName(const std::string& name);
    std::string GetName(UInt_t Z);

private:
    NuclearChart();
    void SetUpNameMap();
    void SetUpZMap();

	ClassDef(NuclearChart,0);
};

#endif // NUCLEARCHART_H
