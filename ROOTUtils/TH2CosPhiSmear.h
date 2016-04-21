#ifndef TH2CosPhiSMEAR_H
#define TH2CosPhiSMEAR_H

#include "Rtypes.h"
#include "TH2.h"
#include <iostream>
#include <vector>
#include "Math/IFunction.h"
#include "Math/Integrator.h"

namespace ROOTUtils
{

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

class GaussSine : public ROOT::Math::IGenFunction
{
private:
    Double_t fCentroid;
    Double_t fSigma;

public:
    GaussSine(const Double_t th_center=0, const Double_t sigma=1.0);
    GaussSine* Clone() const { return new GaussSine(fCentroid, fSigma);}

    Double_t GetCentroid(void) const { return fCentroid;}
    Double_t GetSigma(void) const { return fSigma;}

    void SetCentroid(const Double_t th_centroid) { fCentroid = th_centroid;}
    void SetSigma(const Double_t sigma) { fSigma = sigma;}

private:
    Double_t DoEval(double x) const;
};

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

class GaussSineIntegral
{
private:
    ROOT::Math::IntegratorOneDim fIntegrator;
    GaussSine fFunc;

public:
    GaussSineIntegral();

    GaussSine& GetFunction() { return fFunc;}
    ROOT::Math::IntegratorOneDim& GetIntegrator() { return fIntegrator; }

    Double_t operator()(const Double_t centroid, const std::vector<Double_t>& x);
};

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

class Gauss : public ROOT::Math::IGenFunction
{
private:
    Double_t fCentroid;
    Double_t fSigma;

public:
    Gauss(const Double_t th_center=0, const Double_t sigma=1.0);
    Gauss* Clone() const { return new Gauss(fCentroid, fSigma);}

    Double_t GetCentroid(void) const { return fCentroid;}
    Double_t GetSigma(void) const { return fSigma;}

    void SetCentroid(const Double_t th_centroid) { fCentroid = th_centroid;}
    void SetSigma(const Double_t sigma) { fSigma = sigma;}

private:
    Double_t DoEval(double x) const;
};

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

class GaussIntegral
{
private:
    ROOT::Math::IntegratorOneDim fIntegrator;
    Gauss fFunc;

public:
    GaussIntegral();

    Gauss& GetFunction() { return fFunc;}
    ROOT::Math::IntegratorOneDim& GetIntegrator() { return fIntegrator; }

    Double_t operator()(const Double_t centroid, const std::vector<Double_t>& x);
};

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

class TH2CosPhiSmear
{
private:
    Double_t fWidth;
    std::vector<std::vector<std::vector<Double_t> > > fFilters;
    GaussSineIntegral fThetaProb;
    GaussIntegral fPhiProb;

public:
    TH2CosPhiSmear(const Double_t width);

   /*! Smear function
    *  Does not modify the original histogram but returns
    * a smeared clone of the original back
    */
    TH2* operator()(const TH2* h);

    void SetWidth(const Double_t width) {fWidth = width;}
    Double_t GetWidth() const {return fWidth;}

//    void PrintFilter(std::ostream& stream=std::cout) const;
    TH2* FilterToHist(const Int_t xbin) const;

private:
    Double_t SmearedPointValue(const TH2* h, const Int_t bx, const Int_t by) const;
    void SetUpFilters(const TH2* h);
    std::vector<std::vector<Double_t> > SetUpFilter(const TH2* h, const Int_t bin);
};

} // end namespace

#endif // TH2CosPhiSMEAR_H
