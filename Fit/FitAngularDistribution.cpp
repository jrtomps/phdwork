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
#include "TVirtualFitter.h"
#include "Minuit2/FCNBase.h"
#include "TFitterMinuit.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "FCN_ChiSq.h"

#include <iostream>


Int_t RunMinuit (TFitterMinuit *minuit, Parameters params)
{
    minuit->Clear();

//    minuit->SetParameter(0,"a",startA,0.1,0,0);
    params.CopyIntoMinuit(minuit);

    minuit->SetPrintLevel(0);
    // create Minimizer (default is Migrad)
    minuit->CreateMinimizer();
    iret = minuit->Minimize();
    if (iret != 0) {
       std::cout << "Minimization failed - exit " ;
       return iret;
    }
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


