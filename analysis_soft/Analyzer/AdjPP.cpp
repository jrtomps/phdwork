// ProofPlot.cpp
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <fstream>

#define AdjPP_cpp
#include "TLine.h"
#include "TFile.h"
#include "ProofPlot.h"
#include "AdjPP.h"

ClassImp(AdjPP)

//___________________________________________________________________________
AdjPP::AdjPP(ConfigEntry *ce, bool save) 
  : ProofPlot(ce,save), adjadc(), cadc(), xshift(), xslope()
{}

AdjPP::AdjPP(AdjPP const& obj) : ProofPlot(obj)
{

  if (this != &obj)
    {
      adjadc = obj.adjadc;
    }
}

//_________________________________________________________________
AdjPP::~AdjPP() 
{
#ifdef DEBUG
  std::cout << "destructing" << std::endl;
#endif

  if (! willSave) 
    {
      std::cout << "deleting all hists AdjPP" << std::endl;

      for (Int_t i=0; i<adc.size(); i++)
        {
	  delete adjadc[i];
	  delete cadc[i];
        }
    }
}

//_________________________________________________________________
void
AdjPP::Begin(TTree* /*tree*/) 
{
#ifdef DEBUG
  std::cout << "begin" << std::endl;
#endif

  TString option = GetOption();

}

//_________________________________________________________________
void
AdjPP::SlaveBegin(TTree* /*tree*/) 
{
#ifdef DEBUG
  std::cout << "slave begin" << std::endl;
#endif

  TString option = GetOption();
#ifdef DEBUG
  for (Int_t i=0; i<ndet; i++)
    {
      //      std::cout << desired_max_ph << " ";
      std::cout << ped[i] << " ";
    }
  
#endif

  bpm = new TH1F("bpm",Form("Run %d bpm",runnumber[runcount]),4096,0,4095);
  latch = new TH1F("latch","latch",4096,-0.5,4095.5);
  
  for (Int_t i=0; i<ndet; i++ ) 
    {

     adc.push_back(new TH1F(Form("adc%i",i),Form("Run %d %s adc%i",runnumber[runcount],label[i].Data(),i),4096,0,4095));
     adjadc.push_back(new TH1F(Form("adjadc%i",i),Form("Run %d %s adjadc%i",runnumber[runcount],label[i].Data(),i),4096,0,4095));
     tdc.push_back(new TH1F(Form("tdc%i",i),Form("Run %d %s tdc%i",runnumber[runcount],label[i].Data(),i),4096,0,4095));
     cadc.push_back(new TH1F(Form("cadc%i",i),Form("Run %d %s cadc%i",runnumber[runcount],label[i].Data(),i),4096,0,4095));
     //       tdccut.push_back(new TH1F(Form("tdccut%i",i),Form("Run %d %s tdccut%i",runnumber[runcount],label[i].Data(),i),4096,0,4095));
     //       tdcns.push_back(new TH1F(Form("tdcns%i",i),Form("Run %d %s tdc (ns)%i",runnumber[runcount],label[i].Data(),i),4096,0,4095*tdc_calib[i]));
     //       tdcnscut.push_back(new TH1F(Form("tdcnscut%i",i),Form("Run %d %s tdc (ns)%i Cut by PSD",runnumber[runcount],label[i].Data(),i),4096,0,4095*tdc_calib[i]));
     
      adc[i]->SetXTitle("ADC Channel");
      adc[i]->SetYTitle("Counts");

      adjadc[i]->SetXTitle("ADC Channel");
      adjadc[i]->SetYTitle("Counts");

      tdc[i]->SetXTitle("TDC Channel");
      tdc[i]->SetYTitle("Counts");

      cadc[i]->SetXTitle("ADC Channel");
      cadc[i]->SetYTitle("Counts");

//       tdccut[i]->SetXTitle("TDC Channel");
//       tdccut[i]->SetYTitle("Counts");
//       tdccut[i]->SetLineColor(2);

//       tdcns[i]->SetXTitle("Timing (ns)");
//       tdcns[i]->SetYTitle("Counts");
//       tdcnscut[i]->SetXTitle("Timing (ns)");
//       tdcnscut[i]->SetYTitle("Counts");
//       tdcnscut[i]->SetLineColor(2);
//      adc[i]->SetLineColor(2);

    }

}

//_________________________________________________________________
void
AdjPP::Init(TTree* tree) 
{

  GenerateShiftFactors(500);

#ifdef DEBUG
  std::cout << "init" << std::endl;
#endif

  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);

  nentries = fChain->GetEntries();

//   //D2O, 11 MeV, Linear pol
//   //  TFile *f2 = new TFile("2dcuts_Run73.root");
//   //Cd 15.5 MeV, Circ pol
//   // TFile f2 ("2dcuts_Run157.root");
//   TFile f2 (pce->GetPSDCutFile());
  
//   for (Int_t i=0;i<ndet;i++)
//     {
//       cut.push_back((TCutG*)f2.Get(Form("cut%i",i+1)));
//       //      std::cout << i+1 << std::endl;
//       cut[i]->SetName(Form("cut%i",i));
//       cut[i]->SetLineColor(2);
//       cut[i]->SetLineWidth(2);
//     } 
//   f2.Close();

//   //D2O, 11 MeV, Linear pol
//   //  TFile *f3 = new TFile("2dcuts_tofnsvspsd_Run75.root");
//   //9Be, Circ, 15.5 MeV
//   // TFile f3 ("2dcuts_tofnsvspsd_Run187.root");
//   TFile f3 (pce->GetTOFvsPSDCutFile());
//   for (Int_t i=0;i<ndet;i++)
//     {
//       cuttof.push_back((TCutG*)f3.Get(Form("cut%i",i+1)));
//       cuttof[i]->SetName(Form("cut%i",i));
//       cuttof[i]->SetLineColor(2);
//       cuttof[i]->SetLineWidth(2);
//     }
//   f3.Close();
//   //end
//   //--------------------------------

  for (Int_t i= 0; i<ndet; i++)
    {
      nt_tdc.push_back(0);
      nt_adc.push_back(0);

      b_tdc.push_back(0);
      b_adc.push_back(0);

    }

  // std::cout << "Setting addresses" << std::endl;
  //  fChain->SetBranchAddress("BPM",        &nt_bpm,     &b_bpm);
  fChain->SetBranchAddress("EventNumber",&nt_evnum,   &b_evnum);
  fChain->SetBranchAddress("RunNumber",  &nt_run,     &b_run);
  fChain->SetBranchAddress("trigger",    &nt_latch,   &b_trigger);
  //   fChain->SetBranchAddress("Clock",      &nt_clock ,  &b_clock);
  //   fChain->SetBranchAddress("VetoClock",  &VetoClock,  &b_VetoClock);
  //   fChain->SetBranchAddress("FinalClock", &FinalClock, &b_FinalClock);
  //   fChain->SetBranchAddress("FinalVetoClock", &FinalVetoClock, &b_FinalVetoClock);
  
  // std::cout << "Setting tdc addresses" << std::endl;  

  for (Int_t i=0; i<ndet; i++)
    {
      fChain->SetBranchAddress(Form("TDC-%i%i",i/32,i%32), &nt_tdc[i], &b_tdc[i]);
      fChain->SetBranchAddress(Form("ADC-%i%i",i/32,i%32), &nt_adc[i], &b_adc[i]);
    }  
  
  b_run->GetEntry(0);
  runnumber[runcount] = nt_run;
  // LoadSettings(pce);

  for (Int_t i=0; i<ndet; i++) 
    {

      adc[i]->SetTitle(Form("Run %d %s adc%i",runnumber[runcount], label[i].Data(),i));
      adjadc[i]->SetTitle(Form("Run %d %s adjadc%i",runnumber[runcount], label[i].Data(),i));

      tdc[i]->SetTitle(Form("Run %d %s tdc%i",runnumber[runcount], label[i].Data(),i));
      cadc[i]->SetTitle(Form("Run %d %s cadc%i",runnumber[runcount], label[i].Data(),i));
      //      tdccut[i]->SetTitle(Form("Run %d %s tdccut%i",runnumber[runcount], label[i].Data(),i));
      //       tdcns[i]->SetTitle(Form("Run %d %s tdc (ns)%i",runnumber[runcount], label[i].Data(),i));
      //       tdcnscut[i]->SetTitle(Form("Run %d %s tdc (ns)%i Cut by PSD",runnumber[runcount], label[i].Data(),i));
    }

  runcount++;

}


//_________________________________________________________________
Bool_t
AdjPP::Process(Long64_t entry)
{
  if (entry % 10000 == 0)
    {
      std::cout.precision(2);
      std::cout << "\r\t" << entry/ (Float_t) nentries*100. << "% completed  ";
      std::cout.flush();
    }
  
  b_evnum->GetEntry(entry);
  b_run->GetEntry(entry);
  b_trigger->GetEntry(entry);
//   b_clock->GetEntry(entry);
//   b_VetoClock->GetEntry(entry);
//   b_FinalClock->GetEntry(entry);
//   b_FinalVetoClock->GetEntry(entry);
 //  b_bpm->GetEntry(entry);

  for (Int_t i=0; i<ndet; i++)
    {
      b_tdc[i]->GetEntry(entry);
      b_adc[i]->GetEntry(entry);
     }    
    
  latch->Fill(nt_latch);
 
  bpmval = 0;
  tdcval = 0;

  for (Int_t j=0;j<ndet;j++)
      {
        if (nt_adc[j]>0)
          {
	    adc[j]->Fill(nt_adc[j]);	
	    adjadc[j]->Fill(nt_adc[j]+xshift[j]);
	    if (nt_tdc[j]>1000) cadc[j]->Fill(nt_adc[j]);
	  }

        //--------------------------------------------
        // Begin conditional+more fancy histos
        //--------------------------------------------
       if (nt_tdc[j]>0)
          {
	  tdc[j]->Fill(nt_tdc[j]);

// 	  bpmval = nt_bpm*bpm_calib;
// #ifdef DEBUG
// 	  if (entry%10000 ==0) std::cout << nt_tdc[j] << "*"<< tdc_calib[j] << "+"<<tdc_offset[j] << std::endl;
// #endif
// 	  tdcval = nt_tdc[j]*tdc_calib[j]+tdc_offset[j];
// 	  tdcns[j]->Fill(tdcval-bpmval);	

// 	  tofnsvspsd[j]->Fill(nt_tac[j],tdcval-bpmval);
      
// 	  // 2D cuts
// 	  if (cuttof[j]->IsInside(nt_tac[j],tdcval-bpmval) && (nt_adc[j]>=thresh[j]))
// 	    {
// 	      tdccut[j]->Fill(nt_tdc[j]);
// 	      tdcnscut[j]->Fill(tdcval-bpmval);
// 	      necut[j]->Fill(tof2ne((tdcval-bpmval-gflash[j]),
// 			        det_dist[j]));
// 	      adccutvsT[j]->Fill(nt_clock/10.,nt_adc[j]);
// 	    }
	  	

	}
        
      }


  
  return kTRUE;
}

//_________________________________________________________________
void
AdjPP::SlaveTerminate() 
{
  

}

//_________________________________________________________________
void
AdjPP::Terminate() 
{
  std::cout << std::endl;
  //  CreateLines();
  if (willSave==1)
    {
      TFile *histfile =new TFile(Form("photofrag_hist_run%i.root",runnumber[runcount-1]),"RECREATE");
      
      for(Int_t i=0;i<ndet;i++)
        {
	adc[i]->Write();
	adjadc[i]->Write();
	tdc[i]->Write();
	cadc[i]->Write();
	//	tdccut[i]->Write();
	// 	tdcns[i]->Write();
	// 	tdcnscut[i]->Write();
	
	// 	l_gflash[i]->Write();
	// 	l_ntof[i]->Write();
	// 	l_deut_en[i]->Write();
        }
      //       bpm->Write();
      histfile->Close();
             
    }

}


void
AdjPP::GenerateShiftFactors(Int_t ch)
{

  Float_t refbinc, maxbinc, val;
  Float_t maxpermissible = 5000;
  Float_t min_peak_content = 6;
  Int_t max1, max2;
  Int_t minbin;
  Char_t fgsfname[200];
  TH1F *h1;
  std::strcpy(fgsfname, Form("photofrag_hist_run%i.root",runnumber[runcount]));
  TFile fgsf(fgsfname,"READ");

  std::ofstream out("logfile.txt",std::ofstream::trunc);
  if (out)
    std::clog.rdbuf(out.rdbuf());
  else
    std::cerr << "Error while opening the file" << std::endl;

  if (fgsf.IsZombie())
    {
      std::cout 
	<< fgsfname
	<< " does not exist"
	<< std::endl;
  
      // Set all shifts to 0 and slopes to 1 
      // so that the adjadc hists will be identical
      // to the adc hists
      for (Int_t i=0; i<ndet; i++) 
	{
	  xshift.push_back(0);
	  xslope.push_back(1);
	}
      return;
    }
  
  std::clog << "Run " << runnumber[runcount] << std::endl;
  std::clog
	<< std::setw(10) << "index"
	<< std::setw(8)  << "max1"
	<< std::setw(8)  << "max2"
	<< std::setw(10) << "refbinc"
	<< std::setw(10) << "maxbinc"
	<< std::setw(10) << "diff"
	<< std::endl;

  // Loop through hists finding the necessary shift to put 
  // the 2nd maximum at a consistent location for all hists 
  for (Int_t i=0; i<ndet; i++)
    {
      // Initialize the maximum bins to zero;
      max1 = 0;
      max2 = 0;

      h1 = dynamic_cast<TH1F*>(fgsf.Get(Form("adc%i",i)));

      // Set the range so that the GetMaximumBin returns the largest
      // bin in the entire histogram.
      h1->GetXaxis()->SetRange(1,h1->GetNbinsX());
      max1 = h1->GetMaximumBin();
      refbinc = h1->GetXaxis()->GetBinCenter(ch);  
     
      // Find min bin
      minbin = FindMinBin(h1,max1); 
      // Check to see if there are two maximum by restricting the
      // search to higher bins than the bin with the global maximum
      h1->GetXaxis()->SetRange(minbin,h1->GetNbinsX());
      max2 = h1->GetMaximumBin();

      // Check to see if the size of the second maximum is of "comparable"
      // size to the first peak (it should be within a factor)
      // If there is a second peak, set the second peak to a specific channel.
      // If not, consider the first peak found to be the higher energy peak.
      if (h1->GetBinContent(max1)/h1->GetBinContent(max2) < maxpermissible
	  && h1->GetBinContent(max2) > min_peak_content)
	{
	  maxbinc = h1->GetXaxis()->GetBinCenter(max2);
	}
      else
	{
	  std::cout
	    << "Only one peak found in detector titled \""
	    << h1->GetTitle()
	    << "\""
	    << std::endl;
	  maxbinc = h1->GetXaxis()->GetBinCenter(max1);
	}
 
      // Reset the range of the xaxis
      h1->GetXaxis()->SetRange(1,h1->GetNbinsX());

      val = refbinc - maxbinc;
      
      std::clog.precision(4);
      std::clog 
	<< std::setw(10) << i
	<< std::setw(8)  << max1
	<< std::setw(8)  << max2
	<< std::setw(10) << refbinc
	<< std::setw(10) << maxbinc
	<< std::setw(10) << val
	<< std::endl;
	
      xshift.push_back(val);
      
    }
  
  fgsf.Close();
  out.close();
  
}


Int_t 
AdjPP::FindMinBin(TH1* h, Int_t start_bin)
{
  Float_t thresh = 10.;
  Int_t firstbin = start_bin;
  Int_t lastbin  = h->GetXaxis()->GetLast();

#ifdef DB_FINDMINBIN
  std::cout << firstbin << " " << lastbin << std::endl;
#endif
  Int_t   minbin = firstbin;
  Float_t bc_min = h->GetBinContent(firstbin);
  
  // Note that this does not include the last bin
  // b/c the algorithm would go out of bounds
  Int_t i=firstbin+1;
  while ((h->GetBinContent(i)/bc_min)<thresh && i<lastbin)
    {
      
      // Curr bin is less than previously found min
      if (h->GetBinContent(i) < bc_min) 
	{
	  bc_min = h->GetBinContent(i);
	  minbin = i;
	}
      
      i++;
    }
  
  return minbin;

}
