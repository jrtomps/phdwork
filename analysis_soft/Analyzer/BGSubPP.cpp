// BGSubPP.cpp
//
// Author : Jeromy Tompkins
// Date   : 11/13/2010
//
// Purpose: An extension of the ProofPlot class that substracts the bg from the en spectrum.

#include "TObject.h"
#include "TObjArray.h"
#include "TFile.h"
#include "ProofPlot.h"
#include "ConfigEntry.h"
#include "BGSubPP.h"

ClassImp(BGSubPP)

BGSubPP::BGSubPP(ConfigEntry* ce, Double_t intx1, Double_t intx2, bool save)
: ProofPlot(ce,save), arr_tof(), arr_necut()
{
  fIntx1 = intx1;
  fIntx2 = intx2;
}

BGSubPP::BGSubPP(BGSubPP const& obj)
  : ProofPlot(obj)
{
  if (this != &obj)
    {
      fIntx1    = obj.fIntx1;
      fIntx2    = obj.fIntx2;
      arr_tof   = obj.arr_tof;
      arr_necut = obj.arr_necut;
    }
}

BGSubPP::~BGSubPP(void)
{
  arr_tof.Delete();
  arr_necut.Delete();
}

Double_t
BGSubPP::GetIntX1(void)
{return fIntx1; }

Double_t
BGSubPP::GetIntX2(void)
{return fIntx2; }

void 
BGSubPP::SetIntX1(Double_t x)
{fIntx1 = x; }

void 
BGSubPP::SetIntX2(Double_t x)
{fIntx2 = x; }

void
BGSubPP::SubtractEnBG(void)
{
  Double_t scalefactor;
  TH1D *hbg_tof;
  TH1F *hbg_necut;
  
  Int_t lobin, hibin;
  Int_t lobin1, hibin1;
  Int_t en_bin;
  Float_t content;

  Double_t tdcval;
  Double_t bpmval = nt_bpm*bpm_calib;

  for (Int_t i=0; i<tofnsvspsd.size(); i++)
    {
  
      tdcval = nt_tdc[i]*tdc_calib[i]+tdc_offset[i];

      lobin = tofnsvspsd[i]->GetXaxis()->FindBin(fIntx1);
      hibin = tofnsvspsd[i]->GetXaxis()->FindBin(fIntx2);
      
      hbg_tof = tofnsvspsd[i]->ProjectionY(Form("hbg_tof%i",i),lobin, hibin);
      hbg_tof->SetDirectory(0);
      arr_tof.Add(hbg_tof);
  
      // too general. Correct treatment should be binwise subtraction
      scalefactor = static_cast<Float_t>(tofnsvspsd[i]->GetNbinsX())/(fIntx2-fIntx1);
      hbg_tof->Scale(scalefactor);
      
      hbg_necut = dynamic_cast<TH1F*>(necut[i]->Clone(Form("hbg_necut%i",i)));
      
      for (Int_t ii=0; ii<=hbg_necut->GetNbinsX(); ii++)
        hbg_necut->SetBinContent(ii,0);
      
      hbg_necut->SetDirectory(0);
      arr_necut.Add(hbg_necut);
      
      for (Int_t j=1; j<hbg_necut->GetNbinsX(); j++) // Does not include under/overflow
        {
	lobin1 = hbg_tof->GetXaxis()->FindBin(ne2tof(hbg_necut->GetXaxis()->GetBinUpEdge(j), det_dist[i]));
	hibin1 = hbg_tof->GetXaxis()->FindBin(ne2tof(hbg_necut->GetXaxis()->GetBinLowEdge(j), det_dist[i]));
	
	content = hbg_tof->Integral(lobin1,hibin1);
	// Correct for the finite width of the bins
	content -= hbg_tof->GetBinContent(lobin1)*(hbg_tof->GetXaxis()->GetBinCenter(lobin1)-hbg_tof->GetXaxis()->GetBinLowEdge(lobin1))/hbg_tof->GetXaxis()->GetBinWidth(lobin1);
	content -= hbg_tof->GetBinContent(hibin1)*(hbg_tof->GetXaxis()->GetBinUpEdge(hibin1)-hbg_tof->GetXaxis()->GetBinCenter(hibin1))/hbg_tof->GetXaxis()->GetBinWidth(hibin1);
	
	hbg_necut->SetBinContent(j,content);
	
        }
      necut[i]->Add(hbg_necut,-1.0);
    }
  
}

void 
BGSubPP::Terminate(void)
{
  std::cout << std::endl;
  CreateLines();
  SubtractEnBG();

  if (willSave==1)
    {
      TFile *histfile =new TFile(Form("pfjuly10_hist_run%i.root",runnumber[runcount-1]),"RECREATE");
      
      for(Int_t i=0;i<ndet;i++)
        {
	adc[i]->Write();
	ph[i]->Write();
	tac[i]->Write();
	tdc[i]->Write();
	tdccut[i]->Write();
	tdcns[i]->Write();
	tdcnscut[i]->Write();
	necut[i]->Write();
	phvsEn[i]->Write();
	psd[i]->Write();
	tofnsvspsd[i]->Write();
	cpsd[i]->Write();
	nevsT[i]->Write();
	adccutvsT[i]->Write();
	l_gflash[i]->Write();
	l_ntof[i]->Write();
	l_deut_en[i]->Write();
	arr_tof[i]->Write();
	arr_necut[i]->Write();
      }
      bpm->Write();
      histfile->Close();
             
    }

}
