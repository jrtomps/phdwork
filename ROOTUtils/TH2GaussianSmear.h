#ifndef TH2GAUSSIANSMEAR_H
#define TH2GAUSSIANSMEAR_H

#include "Rtypes.h"
#include "TH2.h"
#include <iostream>

namespace ROOTUtils
{

class TH2GaussianSmear
{
private:
    Double_t fWidth;
    std::vector<std::vector<Double_t> > fFilter;

public:
    TH2GaussianSmear(const Double_t width);

   /*! Smear function
    *  Does not modify the original histogram but returns
    * a smeared clone of the original back
    */
    TH2* operator()(const TH2* h);

    void SetWidth(const Double_t width) {fWidth = width;}
    Double_t GetWidth() const {return fWidth;}

    void PrintFilter(std::ostream& stream=std::cout) const;

private:
    Double_t SmearedPointValue(const TH2* h, const Int_t bx, const Int_t by) const;
    void SetUpFilter(const TH2* h);
};

} // end namespace

#endif // TH2GAUSSIANSMEAR_H
