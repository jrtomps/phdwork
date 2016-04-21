// CompareScalerWithADC.cpp
//
// Jeromy Tompkins
// 6/21/2011
//
//

#include <iostream>
#include "Analyzer/SScalerTool.h"
#include "TObject.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"

enum { ADC0=0, ADC1=1 };
static const Int_t nch_per_adc = 32;

TString base("pffrag1_hist_run");
const TString  sclrfile("/home/coda/experiments/tompkins/pffrag1/scalers/pffrag1_scalers");
const TString sclrcfile("/home/coda/experiments/tompkins/pffrag1/scalers/scaler.config"  );

Double_t
CompareScalerWithADC(const Int_t run, const Int_t adc_id)
{
  Double_t   sum = 0;
  Int_t sclr_val = 0;
  TString fname = base;
  fname += run;
  fname += ".root";

  TFile f(fname.Data());

  if (!f.IsOpen()) {cout << fname.Data() << " doesn't exist" << endl; return sum;}
  
  
  TObject *obj;
  TH1       *h;

  Int_t curr = adc_id*nch_per_adc;


  for (Int_t i=1; i<=nch_per_adc; i++)
    {
      obj = f.Get(Form("adc%i",curr));

      if (obj) h = dynamic_cast<TH1*>(obj);
      else     continue;

      sum += h->Integral();
    }

  SScalerTool st(sclrfile.Data(), sclrcfile.Data());
  sclr_val = st.GetValue(run,Form("VetoTrigs#%i",adc_id));
  
  cout << "ADC/scaler value = " << endl;

  if ( sclr_val != 0)
    return sum / sclr_val;
  else
    return 0;
}
