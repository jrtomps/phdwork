// FitAngularDistribution.cpp
//
// Jeromy Tompkins
// 2/22/2012
//

#include "TFile.h"
#include "TString.h"
#include "TF2.h"

// @(#)root/minuit2:$Id$
// Author: L. Moneta    12/2005
/**
   test of a pure minimization passing a user FCN class directly to the
   TFitterMinuit

*/
#include "TH1.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TVirtualFitter.h"
#include "TStyle.h"
#include "Minuit2/FCNBase.h"
#include "TFitterMinuit.h"
#include "TSystem.h"
#include "TStopwatch.h"
#include "TMath.h"
#include <vector>
#include <iostream>


class AngDist
{
public:
    AngDist (Double_t a, Double_t b, Double_t c, Double_t d)
        : fA(a), fB(b), fC(c), fD(d)
    {}

    AngDist(const AngDist& rhs)
        : fA(rhs.fA), fB(rhs.fB), fC(rhs.fC), fD(rhs.fD)
    {}

    AngDist& operator=(const AngDist& rhs)
        : fA(rhs.fA), fB(rhs.fB), fC(rhs.fC), fD(rhs.fD)
    {}

    ~AngDist() {}

    double operator()(double* x)
    {
        using TMath::Power;
        using TMath::Sin;
        using TMath::Cos;

        //   Fission channel formalism
        return  fA
                +fB*Power(Sin(x[0]), 2.0)
                +fC*Power(Sin(2.0*x[0]), 2.0)
                +Cos(2.0*x[1])*( fD*Power(Sin(x[0]), 2.0) + 4.0*fC*Power(Sin(x[0]),4.0) );
    }

private:
    Double_t fA;
    Double_t fB;
    Double_t fC;
    Double_t fD;
};

class FCN_ChiSq : public ROOT::Minuit2::FCNBase {

public:
    FCN_ChiSq(TGraph2DErrors* gr)
        : fIntegrator(),
        fX(), fY(),
        fZ(), fEX(),
        fEY(), fEZ(),
        fIntLimitsLow(NULL),
        fIntLimitsHigh(NULL)
    {
        CopyDataFromGraph(gr);
        RemoveZeroesFromData();

        fIntLimitsLow = new Double_t[2];
        fIntLimitsHigh = new Double_t[2];

        std::cout << "Constructed FCN_ChiSq" << std::endl;
    }

public:

    ~FCN_ChiSq()
    {
        delete [] IntLimitsLow;
        delete [] IntLimitsHigh;
    }

    Double_t operator() (const std::vector<Double_t> & x) const
    {
        if (x.size()!=4) throw -1;
        AngDist W(x[0],x[1],x[2],x[3]);
        fIntegrator.SetFunction(W,2);
        Double_t sq_sum = 0;

        Double_t v = 0;
        for (Int_t i=0; i<fZ.size(); i++)
        {
            fIntLimitsLow[0] = fX[i]-fEX[i];
            fIntLimitsLow[1] = fY[i]-fEY[i];

            fIntLimitsHigh[0] = fX[i]+fEX[i];
            fIntLimitsHigh[1] = fY[i]+fEY[i];

             v = fIntegrator.Integral(fIntLimitsLow, fIntLimitsHigh);
             sq_sum += Power(fZ[i]-v,2.0);
        }

        return sq_sum/(fZ.size()-1);
    }


    double Up() const { return 1.; // standard for chi2
        // return 0.5; // standard for log-likelihood
  }

    void CopyDataFromGraph(TGraph2DErrors* gr)
    {
        if (gr==NULL) throw -1;

        fX.assign(gr->GetX(),gr->GetX()+gr->GetN());
        fY.assign(gr->GetY(),gr->GetY()+gr->GetN());
        fZ.assign(gr->GetZ(),gr->GetZ()+gr->GetN());

        fEX.assign(gr->GetEX(),gr->GetEX()+gr->GetN());
        fEY.assign(gr->GetEY(),gr->GetEY()+gr->GetN());
        fEZ.assign(gr->GetEZ(),gr->GetEZ()+gr->GetN());
    }

    void RemoveZeroesFromData(void)
    {

        std::cout << "Erasing elements : "
                << std::setw(4) << fZ.size();

        for (UInt_t i=0; i<fZ.size(); i++)
        {
            if (fZ[i] == 0)
            {
                fX.erase(i);
                fY.erase(i);
                fZ.erase(i);

                fEX.erase(i);
                fEY.erase(i);
                fEZ.erase(i);
                i--;
            }
        }

        std::cout << " --> "
                << std::setw(4) << fZ.size()
                << std::endl;

    }

private:
    ROOT::Math::IntegratorMultiDim::IntegratorMultiDim 	fIntegrator;
    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    std::vector<Double_t> fZ;
    std::vector<Double_t> fEX;
    std::vector<Double_t> fEY;
    std::vector<Double_t> fEZ;
    Double_t* fIntLimitsLow;
    Double_t* fIntLimitsHigh;

private:
    FCN_ChiSq(const FCN_ChiSq& rhs);
    FCN_ChiSq& operator=(const FCN_ChiSq& rhs) {}
};

class Parameters
{
public:
    Parameters(TString fname);

    Double_t GetParam(TString key);
private:
    std::map<TString, Double_t> fParams;
};

Int_t RunMinuit (Parameters params)
{

}

int FitAngularDistribution() {

  gSystem->Load("libMinuit2");

  TFitterMinuit * minuit = new TFitterMinuit();

  FCN_ChiSq fcn;
  minuit->SetMinuitFCN(&fcn);
  // starting values
  Double_t startA = -1.2;
  Double_t startB = 1.0;
  Double_t startC = 1.0;
  Double_t startD = 1.0;


  // if not limited (vhigh <= vlow)
  minuit->SetParameter(0,"a",startA,0.1,0,0);
  minuit->SetParameter(1,"b",startB,0.1,0,0);
  minuit->SetParameter(2,"c",startC,0.1,0,0);
  minuit->SetParameter(3,"d",startD,0.1,0,0);

  minuit->SetPrintLevel(3);
  // create Minimizer (default is Migrad)
  minuit->CreateMinimizer();
  int iret = minuit->Minimize();
  if (iret != 0) {
     return iret;
  }

  std::cout << "\nTest performances........\n\n";

  // test performances
  int nMin = 10000;
  TStopwatch w;
  w.Start();
  for (int i = 0; i < nMin; ++i) {
     minuit->Clear();
     // reset -everything
     //minuit->SetMinuitFCN(&fcn);
     minuit->SetParameter(0,"a",startA,0.1,0,0);
     minuit->SetParameter(1,"b",startB,0.1,0,0);
     minuit->SetParameter(2,"c",startC,0.1,0,0);
     minuit->SetParameter(3,"d",startD,0.1,0,0);

     minuit->SetPrintLevel(0);
     // create Minimizer (default is Migrad)
     minuit->CreateMinimizer();
     iret = minuit->Minimize();
     if (iret != 0) {
        std::cout << "Minimization failed - exit " ;
        return iret;
     }
  }
  w.Stop();
  std::cout << "\nTime: \t" << w.RealTime() << " , " << w.CpuTime() << std::endl;
  return 0;
}

#ifndef __CINT__
int main() {
  int iret = testMinimize();
  if (iret != 0) {
    std::cerr << "ERROR: Minimize test failed !" << std::endl;
    return iret;
  }
  return 0;
}
#endif


