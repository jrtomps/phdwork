// ProofPlot.cpp
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TSelector.h"
#include "UtilTemplates.h"
#include "TROOT.h"
#include "Visualizer.h"
#include "FNameManager.h"
#include "ProofPlot.h"

ClassImp(ProofPlot);

std::string ProofPlot::outfname_base("pffrag2_hist_run");

//___________________________________________________________________________
ProofPlot::ProofPlot(ConfigEntry *ce, bool save) 
    : TSelector(),
    fPed(),
    fThresh(),
    fLowCut(),
    fHighCut(),
    fLowCut2(),
    fHighCut2(),
    fLowCut3(),
    fHighCut3(),
    fLowTOFCut(),
    fHighTOFCut(),
    fChain(nullptr),
    runcount(0),
    nentries(0),
    cm(),
    pce(nullptr),

    //    tdc_gate(),
    adc(),   
    adc_cut(), 
    inv_adc_cut(),
    adc_cut_tofcut(),
    adc_gt_thresh(),
    adc_gt_thresh_tofcut(),
    tdc(),
    tdc_cut(),
    inv_tdc_cut(),
    tdc_cut_tofcut(),
    tdc_gt_thresh(),
    tdc_gt_thresh_tofcut(),

    tof(),
    tof_cut(),
    inv_tof_cut(),
    tof_cut_tofcut(),
    tof_gt_thresh(),
    tof_gt_thresh_tofcut(),
    //    tdcnscut(),
    //    tdccut(),
    runnumber(),
    starttime(), 
    endtime(), 
    n_phys_events(),
    ndet(ce->GetNDets()),
    nadc_ch(ce->GetNADCChannels()),
    ntdc_ch(ce->GetNTDCChannels())
    //   max_clock(),
{
    willSave = save;
    for (Int_t i=0; i<nadc_ch; i++)
    {
        adc_label.push_back(TString::Format("ADC%i%02i",i/32,i%32));
    }

    for (Int_t i=0; i<ntdc_ch; i++)
    {
        tdc_label.push_back(TString::Format("TDC%i%02i",i/32,i%32));
        tdc_cut_label.push_back(TString::Format("Cut TDC%i%02i",i/32,i%32));
        inv_tdc_cut_label.push_back(TString::Format("Cut TDC%i%02i",i/32,i%32));
        tof_label.push_back(TString::Format("TOF%i%02i",i/32,i%32));
        tof_cut_label.push_back(TString::Format("TDC-Cut TOF%i%02i",i/32,i%32));
        inv_tof_cut_label.push_back(TString::Format("TDC-Cut Complement TOF%i%02i",i/32,i%32));

        // There are ntdc_ch of these b/c they were combined in the ADC but will
        // be separated by the tdc cut.
        adc_cut_label.push_back(TString::Format("TDC-Cut ADC%i%02i",i/32, i%32));
        inv_adc_cut_label.push_back(TString::Format("TDC-Cut Complement ADC%i%02i",i/32, i%32));

    }
    
    runcount=0;
    if (ce)
    {
        pce = ce;
        LoadSettings(pce);
        runnumber.push_back(pce->GetRun());
    }
    else {
        std::cout << "Error in <ProofPlot::ProofPlot> : Invalid ConfigEntry" << std::endl;
        TSelector::Abort("Invalid config entry",TSelector::kAbortProcess);
        return;
    }
}

ProofPlot::ProofPlot(ProofPlot const& obj)  
    : TSelector(), fPed(),
    fThresh(),
    fLowCut(),
    fHighCut(),
    fLowCut2(),
    fHighCut2(),
    fLowCut3(),
    fHighCut3()
{

    if (this != &obj)
    {
        runcount = obj.runcount;
        nentries = obj.nentries;
        //  cm(obj.cm);
        pce = obj.pce;   // Do not copy the object
        LoadSettings (pce);

        adc_label = obj.adc_label;
        tdc_label = obj.tdc_label;
        adc_cut_label = obj.adc_cut_label;
        inv_adc_cut_label = obj.inv_adc_cut_label;
        tdc_cut_label = obj.tdc_cut_label;
        inv_tdc_cut_label = obj.inv_tdc_cut_label;
        tof_label = obj.tof_label;
        tof_cut_label = obj.tof_cut_label;
        inv_tof_cut_label = obj.inv_tof_cut_label;

        adc = obj.adc;
        adc_cut = obj.adc_cut;
        inv_adc_cut = obj.inv_adc_cut;
        adc_cut_tofcut = obj.adc_cut_tofcut;
        adc_gt_thresh = obj.adc_gt_thresh;
        adc_gt_thresh_tofcut = obj.adc_gt_thresh_tofcut;

        tdc = obj.tdc;
        tdc_cut = obj.tdc_cut;
        inv_tdc_cut = obj.inv_tdc_cut;
        tdc_cut_tofcut = obj.tdc_cut_tofcut;
        tdc_gt_thresh = obj.tdc_gt_thresh;
        tdc_gt_thresh_tofcut = obj.tdc_gt_thresh_tofcut;

        tof  = obj.tof;
        tof_cut  = obj.tof_cut;
        inv_tof_cut  = obj.inv_tof_cut;
        tof_cut_tofcut = obj.tof_cut_tofcut;
        tof_gt_thresh = obj.tof_gt_thresh;
        tof_gt_thresh_tofcut = obj.tof_gt_thresh_tofcut;

        runnumber = obj.runnumber;
        starttime = obj.starttime;
        endtime = obj.endtime;
        n_phys_events = obj.n_phys_events;
        //      max_clock = obj.max_clock;

        nt_tdc = obj.nt_tdc;
        nt_adc = obj.nt_adc;

        b_evnum = obj.b_evnum;
        b_run = obj.b_run;
        b_start = obj.b_start;
        b_end = obj.b_end;
        b_nphyev = obj.b_nphyev;
        b_evtype = obj.b_evtype;
        b_latch = obj.b_latch;
        b_trigger = obj.b_trigger;

        b_tdc = obj.b_tdc;
        b_adc = obj.b_adc;

        ndet = obj.ndet;
        nadc_ch = obj.nadc_ch;
        ntdc_ch = obj.ntdc_ch;

        willSave = obj.willSave;
    }
}

void ProofPlot::SafeDeleteTH1F(TH1F* hist)
{
    if (hist!=0) delete hist;
    hist=0;
}

//_________________________________________________________________
ProofPlot::~ProofPlot() 
{
#ifdef DEBUG
    std::cout << "destructing" << std::endl;
#endif

    for_each(adc.begin(), adc.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(adc_cut.begin(), adc_cut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(inv_adc_cut.begin(), inv_adc_cut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(adc_cut_tofcut.begin(), adc_cut_tofcut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(adc_gt_thresh.begin(), adc_gt_thresh.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(adc_gt_thresh_tofcut.begin(), adc_gt_thresh_tofcut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));

    for_each(tdc.begin(), tdc.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tdc_cut.begin(), tdc_cut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(inv_tdc_cut.begin(), inv_tdc_cut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tdc_cut_tofcut.begin(), tdc_cut_tofcut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tdc_gt_thresh.begin(), tdc_gt_thresh.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tdc_gt_thresh_tofcut.begin(), tdc_gt_thresh_tofcut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));

    for_each(tof.begin(), tof.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tof_cut.begin(), tof_cut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(inv_tof_cut.begin(), inv_tof_cut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tof_cut_tofcut.begin(), tof_cut_tofcut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tof_gt_thresh.begin(), tof_gt_thresh.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));
    for_each(tof_gt_thresh_tofcut.begin(), tof_gt_thresh_tofcut.end(),
             std::bind1st(std::mem_fun(&ProofPlot::SafeDeleteTH1F),this));

}

//_________________________________________________________________
void ProofPlot::Begin(TTree* /*tree*/)
{
#ifdef DEBUG
    std::cout << "begin" << std::endl;
#endif

    TString option = GetOption();

}

//_________________________________________________________________
void ProofPlot::SlaveBegin(TTree* /*tree*/)
{
#ifdef DEBUG
    std::cout << "slave begin" << std::endl;
#endif

    TH1::SetDefaultSumw2(true);

    TString option = GetOption();

    bpm = new TH1F("bpm",Form("Run %d bpm",runnumber[runcount]),4096,0,4096);
    trigger = new TH1F("trigger","trigger",32,0,32);

    TString temp_label;

    for (Int_t i=0; i<nadc_ch; i++ )
    {
        adc.push_back(new TH1F(Form("adc%i",i),
                               Form("Run %d %s; ADC Channel; Counts",
                                    runnumber[runcount],
                                    adc_label[i].Data()),
                               4096,0,4096));
        adc.at(i)->SetDirectory(0);

        temp_label = TString::Format("ADC%i%02i > Thresh",i/32,i%32);
        adc_gt_thresh.push_back(new TH1F(Form("adc_gt_thresh%i",i),
                               Form("Run %d %s; ADC Channel; Counts",
                                    runnumber[runcount],
                                    temp_label.Data()),
                               4096,0,4096));
        adc_gt_thresh.at(i)->SetDirectory(0);

        temp_label = TString::Format("TOF-Cut ADC%i%02i > Thresh",i/32,i%32);
        adc_gt_thresh_tofcut.push_back(new TH1F(Form("adc_gt_thresh_tofcut%i",i),
                               Form("Run %d %s; ADC Channel; Counts",
                                    runnumber[runcount],
                                    temp_label.Data()),
                               4096,0,4096));
        adc_gt_thresh_tofcut.at(i)->SetDirectory(0);

        temp_label = TString::Format("TDC%i%02i > Thresh",i/32,i%32);
        tdc_gt_thresh.push_back(new TH1F(Form("tdc_gt_thresh%i",i),
                               Form("Run %d %s; TDC Channel; Counts",
                                    runnumber[runcount],
                                    temp_label.Data()),
                               4096,0,4096));
        tdc_gt_thresh.at(i)->SetDirectory(0);

        temp_label = TString::Format("TOF-Cut TDC%i%02i > Thresh",i/32,i%32);
        tdc_gt_thresh_tofcut.push_back(new TH1F(Form("tdc_gt_thresh_tofcut%i",i),
                               Form("Run %d %s; TDC Channel; Counts",
                                    runnumber[runcount],
                                    temp_label.Data()),
                               4096,0,4096));
        tdc_gt_thresh_tofcut.at(i)->SetDirectory(0);

        temp_label = TString::Format("TOF%i%02i > Thresh",i/32,i%32);
        tof_gt_thresh.push_back(new TH1F(Form("tof_gt_thresh%i",i),
                               Form("Run %d %s; Time (a.u.); Counts",
                                    runnumber[runcount],
                                    temp_label.Data()),
                               4096,-4096,4096));
        tof_gt_thresh.at(i)->SetDirectory(0);

        temp_label = TString::Format("TOF-Cut TOF%i%02i > Thresh",i/32,i%32);
        tof_gt_thresh_tofcut.push_back(new TH1F(Form("tof_gt_thresh_tofcut%i",i),
                               Form("Run %d %s; ADC Channel; Counts",
                                    runnumber[runcount],
                                    temp_label.Data()),
                               4096,-4096,4096));
        tof_gt_thresh_tofcut.at(i)->SetDirectory(0);

    }

    for (Int_t i=0; i<ntdc_ch; i++)
    {

        tdc.push_back(new TH1F(Form("tdc%i",i),
                               Form("Run %d %s; TDC Channel; Counts",
                                    runnumber[runcount],
                                    tdc_label[i].Data()),
                               4096,0,4096));
        tdc.at(i)->SetDirectory(0);

        tdc_cut.push_back(new TH1F(Form("tdc_cut%i",i),
                                   Form("Run %d %s; TDC Channel; Counts",
                                        runnumber[runcount],
                                        tdc_cut_label[i].Data()),
                                   4096,0,4096));
        tdc_cut.at(i)->SetDirectory(0);

        inv_tdc_cut.push_back(new TH1F(Form("inv_tdc_cut%i",i),
                                       Form("Run %d %s; TDC Channel; Counts",
                                            runnumber[runcount],
                                            inv_tdc_cut_label[i].Data()),
                                       4096,0,4096));
        inv_tdc_cut.at(i)->SetDirectory(0);

        temp_label = TString::Format("TOF-Cut, TDC-CUT TDC%i%02i",i/32,i%32);
        tdc_cut_tofcut.push_back(new TH1F(Form("tdc_cut_tofcut%i",i),
                                          Form("Run %d %s; TDC Channel; Counts",
                                               runnumber[runcount],
                                               temp_label.Data()),
                                          4096,0,4096));
        tdc_cut_tofcut.at(i)->SetDirectory(0);

        tof.push_back(new TH1F(Form("tof%i",i),
                               Form("Run %d %s; Time (a.u.); Counts",
                                    runnumber[runcount],
                                    tof_label[i].Data()),
                               4096,-4096,4096));
        tof.at(i)->SetDirectory(0);

        tof_cut.push_back(new TH1F(Form("tof_cut%i",i),
                                   Form("Run %d %s; Time (a.u.); Counts",
                                        runnumber[runcount],
                                        tof_cut_label[i].Data()),
                                   4096,-4096,4096));
        tof_cut.at(i)->SetDirectory(0);

        inv_tof_cut.push_back(new TH1F(Form("inv_tof_cut%i",i),
                                       Form("Run %d %s; Time (a.u.); Counts",
                                            runnumber[runcount],
                                            inv_tof_cut_label[i].Data()),
                                       4096,-4096,4096));
        inv_tof_cut.at(i)->SetDirectory(0);

        temp_label = TString::Format("TOF-Cut, TDC-CUT TOF%i%02i",i/32,i%32);
        tof_cut_tofcut.push_back(new TH1F(Form("tof_cut_tofcut%i",i),
                                          Form("Run %d %s; Time (a.u.); Counts",
                                               runnumber[runcount],
                                               temp_label.Data()),
                                          4096,-4096,4096));
        tof_cut_tofcut.at(i)->SetDirectory(0);

        adc_cut.push_back(new TH1F(Form("adc_cut%i",i),
                                   Form("Run %d %s; ADC Channel; Counts",
                                        runnumber[runcount],
                                        adc_cut_label[i].Data()),
                                   4096,0,4096));
        adc_cut.at(i)->SetDirectory(0);

        inv_adc_cut.push_back(new TH1F(Form("inv_adc_cut%i",i),
                                       Form("Run %d %s; ADC Channel; Counts",
                                            runnumber[runcount],
                                            inv_adc_cut_label[i].Data()),
                                       4096,0,4096));
        inv_adc_cut.at(i)->SetDirectory(0);

        temp_label = TString::Format("TOF-Cut, TDC-CUT ADC%i%02i",i/32,i%32);
        adc_cut_tofcut.push_back(new TH1F(Form("adc_cut_tofcut%i",i),
                                          Form("Run %d %s; ADC Channel; Counts",
                                               runnumber[runcount],
                                               temp_label.Data()),
                                          4096,0,4096));
        adc_cut_tofcut.at(i)->SetDirectory(0);

    }  
}

//_________________________________________________________________
void ProofPlot::Init(TTree* tree)
{
#ifdef DEBUG
    std::cout << "init" << std::endl;
#endif

    if (!tree) return;
    fChain = tree;
    fChain->SetMakeClass(1);

    nentries = fChain->GetEntries();

    // Set size of the vectors holding all branch
    // address pointers to ndets using pointers
    // to 0 as placeholders
    b_evnum=0;
    b_run=0;
    b_trigger=0;
    for (Int_t i=0; i<nadc_ch; i++)
    {
        nt_adc.push_back(0);
        b_adc.push_back(0);
    }
    for (Int_t i=0; i<ntdc_ch; i++)
    {
        nt_tdc.push_back(0);
        b_tdc.push_back(0);
    }

    fChain->SetBranchAddress("EventNumber",&nt_evnum,   &b_evnum);
    fChain->SetBranchAddress("RunNumber",  &nt_run,     &b_run);
    fChain->SetBranchAddress("trigger",    &nt_trigger,   &b_trigger);

    for (Int_t i=0; i<nadc_ch; i++)
    {
        fChain->SetBranchAddress(Form("ADC%i%02i",i/32,i%32), &nt_adc[i], &b_adc[i]);
    }  

    for (Int_t i=0; i<ntdc_ch; i++)
    {
        fChain->SetBranchAddress(Form("TDC%i%02i",i/32,i%32), &nt_tdc[i], &b_tdc[i]);
    }

    b_run->GetEntry(0);
    runnumber[runcount] = nt_run;

    runcount++;

}


//_________________________________________________________________
Bool_t ProofPlot::Process(Long64_t entry)
{
    if (entry % (nentries/1000)== 0)
    {
        std::cout << std::setiosflags( std::ios::fixed );
        std::cout.precision(1);
        std::cout << "\r"
  		<< std::setw(8) << std::right <<(100.0*entry)/nentries 
  		<< "% completed";
        std::cout.flush();
        std::cout << std::resetiosflags( std::ios::fixed );
    }
    else if (entry == (nentries - 1 ))
    {
        std::cout << std::setiosflags( std::ios::fixed );
        std::cout.precision(1);
        std::cout << "\r" << std::setw(8) << std::right << 100 << "% completed  ";
        std::cout.flush();
        std::cout << std::resetiosflags( std::ios::fixed );
    }

    b_evnum->GetEntry(entry);
    b_run->GetEntry(entry);
    b_trigger->GetEntry(entry);

    // for (incr=0; incr<nadc_ch; incr++)
    //   b_adc[incr]->GetEntry(entry);
    // for (incr=0; incr<ntdc_ch; incr++)
    //   b_tdc[incr]->GetEntry(entry);

    trigger->Fill(nt_trigger);

    bpmval = 0;
    tdcval = 0;
    Double_t adc_val=0, tdc_val=0;
    Float_t tofval=0;
    for (incr=0;incr<ntdc_ch;incr++)
    {
      if (incr<nadc_ch)
      {
        b_adc[incr]->GetEntry(entry);
        adc_val = nt_adc[incr];

        b_tdc[incr]->GetEntry(entry);
        tdc_val = nt_tdc[incr];

        // TOF corresponds to appropriate BPM channel
        tofval = nt_tdc[32*(incr/32+1)-1]-tdc_val;

        if (adc_val>0)
        {
          adc[incr]->Fill(adc_val);
          if (adc_val>4095)
            std::cout << "Overflow detected in adc"
              << incr
              << std::endl;
          if (adc_val>fThresh[incr])
          {
            adc_gt_thresh[incr]->Fill(adc_val);
            tdc_gt_thresh[incr]->Fill(tdc_val);
            tof_gt_thresh[incr]->Fill(tofval);

            if (tofval>fLowTOFCut[incr] && tofval<fHighTOFCut[incr])
            {
              adc_gt_thresh_tofcut[incr]->Fill(adc_val);
              tdc_gt_thresh_tofcut[incr]->Fill(tdc_val);
              tof_gt_thresh_tofcut[incr]->Fill(tofval);
            }
          }
        }
      }

      if (tdc_val>0)
      {
        tdc[incr]->Fill(tdc_val);
        tof[incr]->Fill(tofval);
      }

      // Fill tdc-cut histograms
      if ( (tdc_val>fLowCut[incr] && tdc_val<fHighCut[incr]) || (tdc_val>fLowCut2[incr] && tdc_val<fHighCut2[incr]) || (tdc_val>fLowCut3[incr] && tdc_val<fHighCut3[incr]) )
      {
        tdc_cut[incr]->Fill(tdc_val);
        tof_cut[incr]->Fill(tofval);

        if (tofval>fLowTOFCut[incr] && tofval<fHighTOFCut[incr])
        {
          tdc_cut_tofcut[incr]->Fill(tdc_val);
          tof_cut_tofcut[incr]->Fill(tofval);
        }

        if (incr<nadc_ch && adc_val>0)
        {
          adc_cut[incr]->Fill(adc_val);

          if (tofval>fLowTOFCut[incr] && tofval<fHighTOFCut[incr])
          {
            adc_cut_tofcut[incr]->Fill(adc_val);
          }
        }
      }
      else // Fill tdc-cut complement histograms
      {
        inv_tdc_cut[incr]->Fill(tdc_val);
        inv_tof_cut[incr]->Fill(tofval);

        if (incr<nadc_ch && adc_val>0)
        {
          inv_adc_cut[incr]->Fill(adc_val);
        }
      } // end if-else construct for tdc-cuts
    }

    return kTRUE;
}

//_________________________________________________________________
void ProofPlot::SlaveTerminate()
{


}

//_________________________________________________________________
void ProofPlot::Terminate()
{
  std::cout << std::endl;
  //  CreateLines();

  SetCutHistogramColors();
  FNameManager fnm;
  std::string base = fnm.GetHistFileBase(true);
  if (willSave==kTRUE)
  {
    TFile *histfile = new TFile(Form("%s%i.root",base.data(),runnumber[runcount-1]),"UPDATE");

    WriteToFile(histfile,"adc",adc);
    WriteToFile(histfile,"adc_cut",adc_cut);
    WriteToFile(histfile,"inv_adc_cut",inv_adc_cut);
    WriteToFile(histfile,"adc_cut_tofcut",adc_cut_tofcut);
    WriteToFile(histfile,"adc_gt_thresh",adc_gt_thresh);
    WriteToFile(histfile,"adc_gt_thresh_tofcut",adc_gt_thresh_tofcut);

    WriteToFile(histfile,"tdc",tdc);
    WriteToFile(histfile,"tdc_cut",tdc_cut);
    WriteToFile(histfile,"inv_tdc_cut",inv_tdc_cut);
    WriteToFile(histfile,"tdc_cut_tofcut",tdc_cut_tofcut);
    WriteToFile(histfile,"tdc_gt_thresh",tdc_gt_thresh);
    WriteToFile(histfile,"tdc_gt_thresh_tofcut",tdc_gt_thresh_tofcut);

    WriteToFile(histfile,"tof",tof);
    WriteToFile(histfile,"tof_cut",tof_cut);
    WriteToFile(histfile,"inv_tof_cut",inv_tof_cut);
    WriteToFile(histfile,"tof_cut_tofcut",tof_cut_tofcut);
    WriteToFile(histfile,"tof_gt_thresh",tof_gt_thresh);
    WriteToFile(histfile,"tof_gt_thresh_tofcut",tof_gt_thresh_tofcut);


    SaveCanvases(histfile,"canvases");
    trigger->Write("",TObject::kOverwrite);
    //       bpm->Write();
    histfile->Close();

  }

}

void ProofPlot::LoadSettings(ConfigEntry *dbentry)
{
  ExtensibleDb *db = dbentry->GetExtensibleDb();

  if (db==NULL) throw -1;

  db->GetValues("Pedestal",fPed);
  if (fPed.size()==0) throw -1;

  db->GetValues("Threshold",fThresh);
  if (fThresh.size()==0) throw -1;

  db->GetValues("LowCut",fLowCut);
  if (fLowCut.size()==0) throw -1;

  db->GetValues("HighCut",fHighCut);
  if (fHighCut.size()==0) throw -1;

  db->GetValues("LowCut2",fLowCut2);
  if (fLowCut2.size()==0) throw -1;

  db->GetValues("HighCut2",fHighCut2);
  if (fHighCut2.size()==0) throw -1;

  db->GetValues("LowCut3",fLowCut3);
  if (fLowCut3.size()==0) throw -1;

  db->GetValues("HighCut3",fHighCut3);
  if (fHighCut3.size()==0) throw -1;

  db->GetValues("LowTOFCut",fLowTOFCut);
  if (fLowTOFCut.size()==0) throw -1;

  db->GetValues("HighTOFCut",fHighTOFCut);
  if (fHighTOFCut.size()==0) throw -1;


#ifdef VERBOSE_PROOFPLOT
  std::cout << std::setw(15) << "Pedestal" << ":"; PrintVector(fPed);
  std::cout << std::setw(15) << "Threshold" << ":"; PrintVector(fThresh);
  std::cout << std::setw(15) << "LowCut" << ":"; PrintVector(fLowCut);
  std::cout << std::setw(15) << "HighCut" << ":"; PrintVector(fHighCut);
#endif
}

void ProofPlot::SetWarningColors(TH1* h)
{
  if (h)
  {
    h->SetLineColor(kRed);
    h->SetLineWidth(2);
    h->SetFillColor(kYellow);
    h->SetMarkerColor(kRed);
  }
  else
  {
    std::cerr << "SetWarningColors:: histogram doesn't exist" 
      << std::endl;
  }
}

void ProofPlot::SetCutHistogramColors(void)
{
  for (Int_t i=0; i<ntdc_ch; i++)
  {
    if (fLowCut[i] > fHighCut[i])
    {
      SetWarningColors(adc_cut[i]);
      SetWarningColors(tdc_cut[i]);
      SetWarningColors(tof_cut[i]);
      SetWarningColors(inv_adc_cut[i]);
      SetWarningColors(inv_tdc_cut[i]);
      SetWarningColors(inv_tof_cut[i]);
      SetWarningColors(adc_cut_tofcut[i]);
      SetWarningColors(tdc_cut_tofcut[i]);
      SetWarningColors(tof_cut_tofcut[i]);
    }
  }
}

void ProofPlot::WriteToFile(TFile* f, const Char_t* dir_name, std::vector<TH1F*>& vec)
{
  if (f==0)
  {
    std::cout << "ProofPlot::WriteToFile:::> NULL file argument" << std::endl;
    return;
  }
  else if (!f->IsOpen())
  {
    std::cout << "ProofPlot::WriteToFile:::> File is not open" << std::endl;
    return;
  }

  TObject *obj=0;
  TDirectory* dir;
  obj = f->Get(dir_name);
  if (obj!=0)
    dir = static_cast<TDirectory*>(obj);
  else
    dir = f->mkdir(dir_name);

  dir->cd();

  for (UInt_t i=0; i<vec.size(); i++)
    vec[i]->Write("",TObject::kOverwrite);

  f->cd();

}

void ProofPlot::SaveCanvases(TFile *f, const TString& dir_name)
{
  if (f==0)
  {
    std::cerr << "ProofPlot::SaveCanvases:::> NULL file argument" << std::endl;
    return;
  }
  else if (!f->IsOpen())
  {
    std::cerr << "ProofPlot::SaveCanvases:::> File is not open or doesn't exist" << std::endl;
    return;
  }

  TDirectory* dir;
  dir = dynamic_cast<TDirectory*>(f->GetDirectory(dir_name));
  if (dir==NULL)
    dir = f->mkdir(dir_name);
  f->cd();
  //    dir->cd();

  gROOT->SetBatch(true);

  Visualizer vis;
  std::vector<TLine*> vlow, vhi;

  std::cout << "Begin drawing ADC canvases" << std::endl;
  vlow = GenerateTLines(fThresh);
  vis.Draw("adc%i","logy");
  vis.Draw("inv_adc_cut%i",Form("canvas=adc_run%i color=3 same",runnumber[runcount-1]));
  vis.Draw("adc_cut%i",Form("canvas=adc_run%i color=2 same",runnumber[runcount-1]));
  vis.Draw("adc_cut_tofcut%i",Form("canvas=adc_run%i color=6 same",runnumber[runcount-1]));
  vis.Draw("adc_gt_thresh",Form("canvas=adc_run%i color=7 same",runnumber[runcount-1]));
  vis.Draw("adc_gt_thresh_tofcut",Form("canvas=adc_run%i color=14 same",runnumber[runcount-1]));
  vis.DrawLines(vlow,Form("canvas=adc_run%i color=4 same",runnumber[runcount-1]));

  std::cout << "Begin drawing TDC canvases" << std::endl;
  vlow = GenerateTLines(fLowCut);
  vhi = GenerateTLines(fHighCut);
  vis.Draw("tdc%i","logy");
  vis.Draw("inv_tdc_cut%i",Form("canvas=tdc_run%i color=3 same",runnumber[runcount-1]));
  vis.Draw("tdc_cut%i",Form("canvas=tdc_run%i color=2 same",runnumber[runcount-1]));
  vis.Draw("tdc_cut_tofcut%i",Form("canvas=tdc_run%i color=6 same",runnumber[runcount-1]));
  vis.Draw("tdc_gt_thresh",Form("canvas=tdc_run%i color=7 same",runnumber[runcount-1]));
  vis.Draw("tdc_gt_thresh_tofcut",Form("canvas=tdc_run%i color=14 same",runnumber[runcount-1]));
  vis.DrawLines(vlow,Form("canvas=tdc_run%i color=4 same",runnumber[runcount-1]));
  vis.DrawLines(vhi,Form("canvas=tdc_run%i color=4 same",runnumber[runcount-1]));


  std::cout << "Begin drawing TOF canvases" << std::endl;
  vlow = GenerateTLines(fLowTOFCut);
  vhi = GenerateTLines(fHighTOFCut);

  vis.Draw("tof%i","logy");
  vis.Draw("inv_tof_cut%i",Form("canvas=tof_run%i color=3 same",runnumber[runcount-1]));
  vis.Draw("tof_cut%i",Form("canvas=tof_run%i color=2 same",runnumber[runcount-1]));
  vis.Draw("tof_cut_tofcut%i",Form("canvas=tof_run%i color=6 same",runnumber[runcount-1]));
  vis.Draw("tof_gt_thresh",Form("canvas=tof_run%i color=7 same",runnumber[runcount-1]));
  vis.Draw("tof_gt_thresh_tofcut",Form("canvas=tof_run%i color=14 same",runnumber[runcount-1]));
  vis.DrawLines(vlow,Form("canvas=tof_run%i color=4 same",runnumber[runcount-1]));
  vis.DrawLines(vhi,Form("canvas=tof_run%i color=4 same",runnumber[runcount-1]));

  //    for (UInt_t i=0; i<vec.size(); i++)
  //        vec[i]->Write("",TObject::kOverwrite);
  vis.SetAxisLabelSizeOnAllCanvases(0.05,'x');
  vis.SaveAllCanvases(dir);

  gROOT->SetBatch(false);
  f->cd();

}


std::vector<TLine*> ProofPlot::GenerateTLines(std::vector<Float_t> &xvals)
{
  std::vector<TLine*> lines(xvals.size(),static_cast<TLine*>(NULL));

  for (UInt_t i=0; i<xvals.size(); i++)
  {
    lines[i] = new TLine(xvals[i],0,xvals[i],10000);
  }

  return lines;
}
