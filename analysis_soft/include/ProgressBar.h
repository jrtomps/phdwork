#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>
#include <string>
#include "Rtypes.h"

class ProgressBar
{
    std::ostream& fStream;
    UInt_t fMax;
    UInt_t fCurr;
    UInt_t fWidth;
    std::string fPreString;

public:
    ProgressBar(UInt_t nmax, std::ostream& stream=std::cout , std::string prestr="");
    ProgressBar(UInt_t nmax, UInt_t width, std::ostream& stream=std::cout, std::string prestr="");

    void Reset();
    void AdvanceProgress();
    void Print();
};

#endif // PROGRESSBAR_H
