
#include <iostream>
#include <cstring>
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TObjArray.h"

static const Int_t ndets = 32;
static const Char_t* adc_base = "ADC";
static const Char_t* tdc_base = "TDC";
TObjArray *histarr;
TCanvas *c1, *c2;

void
quickinspect(Int_t runnumber, Int_t mode=0,Int_t thresh=200) // 0 = ADC , 1 = TDC
{
  // TH1F *hist[ndets];
  Char_t pchar[100];
  histarr = new TObjArray(ndets);

  if (mode == 0)
    strcpy(pchar,adc_base);
  else // if (mode == 1)
    strcpy(pchar,tdc_base);

  //  for (Int_t k=0; k<ndets; k++)
  //    {
  //      histarr->Add(new TH1F(Form("hist[%i]",k+1),Form("hist[%i]",k+1),4096,0,4095));
      //      hist[k] = new TH1F(Form("hist[%i]",k+1),Form("hist%i",k+1),4096,0,4095);
  //    }

  c1 = new TCanvas("c1","c1");
  c1->SetLogy(1);
  c1->Divide(4,4,0.00001, 0.00001);


  c2 = new TCanvas("c2","c2");
  c2->SetLogy(1);
  c2->Divide(4,4,0.00001, 0.00001);

  
  TFile *f = new TFile(Form("run_tree_dir/run%i_tree.root",runnumber),"READ");
  TTree *nt = dynamic_cast<TTree*>(f->Get("flat_tree"));
  
  for (Int_t i=0; i<ndets/2; i++)
    {
      std::cout << i+1 << std::endl;
      c1->cd(i+1);
      //      nt->Draw(Form("ADC%i",i+1,i+1),Form("ADC%i>100",i+1,i+1));
     
      nt->Draw(Form("%s%1i%02i",pchar,i/32,i%32),Form("%s%1i%02i>%i",pchar,i/32,i%32,thresh));
      //      nt->Draw(Form("ADC%i>>hist[%i]",i+1,i+1));
      //      ((TH1F*)histarr->At(i))->Draw();
      //      hist[i]->Draw();
      c1->Modified();
      c1->Update();
    }

  for (Int_t i=ndets/2; i<ndets; i++)
    {
      std::cout << i-ndets/2+1 << std::endl;
      c2->cd(i-ndets/2+1);
      //      nt->Draw(Form("ADC%i",i+1,i+1),Form("ADC%i>100",i+1,i+1));
      nt->Draw(Form("%s%1i%02i",pchar,i/32,i%32),Form("%s%1i%02i>%i",pchar,i/32,i%32,thresh));    
      //      nt->Draw(Form("ADC%i>>h[%i](4096,0,4095)",i+1,i+1));
      //
      //      (TH1F*)gDirectory->GetPrimitive("htemp");
      //      ((TH1F*)histarr->At(i))->Draw();
      //      hist[i]->Draw();

    }

  

}
