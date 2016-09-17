// PPCoincMap.cpp
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
#include "PPCoincMap.h"

ClassImp(PPCoincMap);

const Char_t*  PPCoincMap::outfname_base = "pffrag2_hist_run";

//___________________________________________________________________________
PPCoincMap::PPCoincMap(ConfigEntry *ce, bool save)
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
    fPedThresh(),
    ndet(ce->GetNDets()),
    nadc_ch(ce->GetNADCChannels()),
    ntdc_ch(ce->GetNTDCChannels()), 

    coinc_map_aa(NULL),
    coinc_map_ff(NULL),
    coinc_map_fa(NULL),
    coinc_map_mult_a(NULL),
    coinc_map_mult_f(NULL),

    coinc_map_anota(NULL),
    coinc_map_fnota(NULL),
    coinc_mult_a(NULL),
    coinc_mult_f(NULL),

    adc_vals_a(),
    adc_vals_f(),

    runnumber(),
    starttime(),
    endtime(),
    n_phys_events(),
    mult_mismatch(0)
    //   max_clock(),
{
#ifdef DEBUG
    std::cout << "Constructing PPCoinMap" << std::endl; std::cout.flush();
#endif
    willSave = save;

    adc_vals_a.resize(nadc_ch,0);
    adc_vals_f.resize(nadc_ch,0);

    runcount=0;
    if (ce)
    {
        pce = ce;
        LoadSettings(pce);
        runnumber.push_back(pce->GetRun());
    }
    else {
        std::cout << "Error in <PPCoincMap::PPCoincMap> : Invalid ConfigEntry" << std::endl;
        TSelector::Abort("Invalid config entry",TSelector::kAbortProcess);
        return;
    }
}

PPCoincMap::PPCoincMap(PPCoincMap const& obj)
    : fPed(),
    fThresh(),
    fLowCut(),
    fHighCut(),
    fLowCut2(),
    fHighCut2(),
    fLowCut3(),
    fHighCut3(),
    fPedThresh(),
    coinc_map_aa(NULL),
    coinc_map_ff(NULL),
    coinc_map_fa(NULL),
    coinc_map_mult_a(NULL),
    coinc_map_mult_f(NULL),

    coinc_map_anota(NULL),
    coinc_map_fnota(NULL),
    coinc_mult_a(NULL),
    coinc_mult_f(NULL),

    adc_vals_a(),
    adc_vals_f(),

    mult_mismatch(0)
{
#ifdef DEBUG
    std::cout << "Copy constructing PPCoinMap" << std::endl; std::cout.flush();
#endif

    if (this != &obj)
    {
        runcount = obj.runcount;
        nentries = obj.nentries;
        //  cm(obj.cm);
        pce = obj.pce;   // Do not copy the object
        LoadSettings (pce);

        TH2D* tmp = static_cast<TH2D*>(obj.coinc_map_aa->Clone(TString(obj.coinc_map_aa->GetName()).Append("_").Data()));
        tmp->SetDirectory(0);

        coinc_map_aa = tmp;
        coinc_map_aa->SetName("coinc_map_aa");

        tmp = static_cast<TH2D*>(obj.coinc_map_ff->Clone(TString(obj.coinc_map_ff->GetName()).Append("_").Data()));
        tmp->SetDirectory(0);

        coinc_map_ff = tmp;
        coinc_map_ff->SetName("coinc_map_ff");

        tmp = static_cast<TH2D*>(obj.coinc_map_fa->Clone(TString(obj.coinc_map_fa->GetName()).Append("_").Data()));
        tmp->SetDirectory(0);

        coinc_map_fa = tmp;
        coinc_map_fa->SetName("coinc_map_fa");

        tmp = static_cast<TH2D*>(obj.coinc_map_mult_a->Clone(TString(obj.coinc_map_mult_a->GetName()).Append("_").Data()));
        tmp->SetDirectory(0);

        coinc_map_mult_a = tmp;
        coinc_map_mult_a->SetName("coinc_map_mult_a");

        tmp = static_cast<TH2D*>(obj.coinc_map_mult_f->Clone(TString(obj.coinc_map_mult_f->GetName()).Append("_").Data()));
        tmp->SetDirectory(0);

        coinc_map_mult_f = tmp;
        coinc_map_mult_f->SetName("coinc_map_mult_f");

        TH1D* tmp1 = static_cast<TH1D*>(obj.coinc_map_anota->Clone(TString(obj.coinc_map_anota->GetName()).Append("_").Data()));
        tmp1->SetDirectory(0);

        coinc_map_anota = tmp1;
        coinc_map_anota->SetName("coinc_map_anota");

        tmp1 = static_cast<TH1D*>(obj.coinc_map_fnota->Clone(TString(obj.coinc_map_fnota->GetName()).Append("_").Data()));
        tmp1->SetDirectory(0);

        coinc_map_fnota = tmp1;
        coinc_map_fnota->SetName("coinc_map_fnota");

        tmp1 = static_cast<TH1D*>(obj.coinc_mult_a->Clone(TString(obj.coinc_mult_a->GetName()).Append("_").Data()));
        tmp1->SetDirectory(0);

        coinc_mult_a = tmp1;
        coinc_mult_a->SetName("coinc_mult_a");

        tmp1 = static_cast<TH1D*>(obj.coinc_mult_f->Clone(TString(obj.coinc_mult_f->GetName()).Append("_").Data()));
        tmp1->SetDirectory(0);

        coinc_mult_f = tmp1;
        coinc_mult_f->SetName("coinc_mult_f");

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

        adc_vals_a = obj.adc_vals_a;
        adc_vals_f = obj.adc_vals_f;

        willSave = obj.willSave;

        mult_mismatch = obj.mult_mismatch;
    }
}

void PPCoincMap::SafeDeleteTH2D(TH2D* hist)
{
    if (hist!=NULL) delete hist;
    hist=0;
}

void PPCoincMap::SafeDeleteTH1D(TH1D* hist)
{
    if (hist!=NULL) delete hist;
    hist=0;
}

//_________________________________________________________________
PPCoincMap::~PPCoincMap()
{
#ifdef DEBUG
    std::cout << "destructing" << std::endl;
#endif

    SafeDeleteTH2D(coinc_map_aa);
    SafeDeleteTH2D(coinc_map_ff);
    SafeDeleteTH2D(coinc_map_fa);
    SafeDeleteTH2D(coinc_map_mult_a);
    SafeDeleteTH2D(coinc_map_mult_f);
    SafeDeleteTH1D(coinc_map_anota);
    SafeDeleteTH1D(coinc_map_fnota);
    SafeDeleteTH1D(coinc_mult_a);
    SafeDeleteTH1D(coinc_mult_f);

}

//_________________________________________________________________
void PPCoincMap::Begin(TTree* /*tree*/)
{
#ifdef DEBUG
    std::cout << "begin" << std::endl;
#endif

    TString option = GetOption();

}

//_________________________________________________________________
void PPCoincMap::SlaveBegin(TTree* /*tree*/)
{
#ifdef DEBUG
    std::cout << "slave begin" << std::endl;
#endif

    TH1::SetDefaultSumw2(true);

    TString option = GetOption();

    TString temp_label;

    coinc_map_aa = new TH2D("coinc_map_aa",
                         "Coincidence Map Anything-Anything; ADC channel; ADC channel;Total Events (counts)",
                         nadc_ch,-0.5,nadc_ch-0.5,
                         nadc_ch,-0.5,nadc_ch-0.5);
    coinc_map_aa->SetDirectory(0);

    coinc_map_ff = new TH2D("coinc_map_ff",
                         "Coincidence Map Fragment-Fragment; ADC channel; ADC channel;Total Events (counts)",
                         nadc_ch,-0.5,nadc_ch-0.5,
                         nadc_ch,-0.5,nadc_ch-0.5);
    coinc_map_ff->SetDirectory(0);

    coinc_map_fa = new TH2D("coinc_map_fa",
                         "Coincidence Map Fragment-Anything; ADC channel; ADC channel;Total Events (counts)",
                         nadc_ch,-0.5,nadc_ch-0.5,
                         nadc_ch,-0.5,nadc_ch-0.5);
    coinc_map_fa->SetDirectory(0);

    coinc_map_mult_a = new TH2D("coinc_map_mult_a",
                         "Coincidence Multiplicities For Anything; ADC channel;Multiplicity;Total Events (counts)",
                         nadc_ch,-0.5,nadc_ch-0.5,
                         nadc_ch,-0.5,nadc_ch-0.5);
    coinc_map_mult_a->SetDirectory(0);

    coinc_map_mult_f = new TH2D("coinc_map_mult_f",
                         "Coincidence Multiplicities For Fragments; ADC channel;Multiplicity;Total Events (counts)",
                         nadc_ch,-0.5,nadc_ch-0.5,
                         nadc_ch,-0.5,nadc_ch-0.5);
    coinc_map_mult_f->SetDirectory(0);

    coinc_map_anota = new TH1D("coinc_map_anota",
                               "Coincidence Map Anything-Nothing;ADC channel;Total Events(counts)",
                               nadc_ch,-0.5,nadc_ch-0.5);
    coinc_map_anota->SetDirectory(0);

    coinc_map_fnota = new TH1D("coinc_map_fnota",
                               "Coincidence Map Fragment-Nothing;ADC channel;Total Events (counts)",
                               nadc_ch,-0.5,nadc_ch-0.5);
    coinc_map_fnota->SetDirectory(0);

    coinc_mult_a = new TH1D("coinc_mult_a",
                               "Coincidence Multiplicities For Anything;Multiplicity; Total Events (counts)",
                               nadc_ch,-0.5,nadc_ch-0.5);
    coinc_mult_a->SetDirectory(0);

    coinc_mult_f = new TH1D("coinc_mult_f",
                               "Coincidence Multiplicities For Fragments;Mulitiplicity; Total Events (counts)",
                               nadc_ch,-0.5,nadc_ch-0.5);
    coinc_mult_f->SetDirectory(0);

}

//_________________________________________________________________
void PPCoincMap::Init(TTree* tree)
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
Bool_t PPCoincMap::Process(Long64_t entry)
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

    bpmval = 0;
    tdcval = 0;
    Double_t adc_val, tdc_val;
    Float_t tofval;
    multiplicity_a = 0;
    multiplicity_f = 0;
    mult_mismatch = 0;

    for (incr=0;incr<nadc_ch;incr++)
    {
        adc_vals_a[incr] = false;
        adc_vals_f[incr] = false;

        b_adc[incr]->GetEntry(entry);
        adc_val = nt_adc[incr];

        b_tdc[incr]->GetEntry(entry);
        tdc_val = nt_tdc[incr];

        // TOF corresponds to appropriate BPM channel
        tofval = nt_tdc[32*(incr/32+1)-1]-tdc_val;

        if (adc_val>fPedThresh[incr])
        {
            adc_vals_a[incr] = true;
            multiplicity_a++;

            if (adc_val>4095)
            {
                std::cout << "Overflow detected in adc"
                        << incr
                        << std::endl;
            }

            for (incr1=0; incr1<=incr; incr1++)
            {
                if (adc_vals_a[incr1]==true)
                {
                    coinc_map_aa->Fill(incr,incr1);
                }
            }

            if (adc_val>fThresh[incr])
            {
                if (tofval>fLowTOFCut[incr] && tofval<fHighTOFCut[incr])
                {
                    adc_vals_f[incr] = true;
                    multiplicity_f++;

                    for (incr1=0; incr1<=incr; incr1++)
                    {
                        if (adc_vals_a[incr1]==true)
                        {
                            coinc_map_fa->Fill(incr,incr1);
                        }
                        if (adc_vals_f[incr1]==true)
                        {
                            coinc_map_ff->Fill(incr,incr1);
                        }
                    }
                }
            }
        }
    }

    coinc_mult_a->Fill(multiplicity_a);
    coinc_mult_f->Fill(multiplicity_f);

    if (multiplicity_a==1)
    {
        UInt_t nonzero_bin = FindNonZeroBin(adc_vals_a);
        coinc_map_anota->Fill(nonzero_bin);

    }

    if (multiplicity_f==1)
    {
        UInt_t nonzero_bin = FindNonZeroBin(adc_vals_f);
        coinc_map_fnota->Fill(nonzero_bin);
    }

#ifdef DEBUG
    if (multiplicity_f==1 && multiplicity_a!=1)
    {
        mult_mismatch++;
    }
#endif

    for (UInt_t i=0; i<nadc_ch; i++)
    {
        if (adc_vals_a[i]==true)
        {
            coinc_map_mult_a->Fill(i,multiplicity_a);

            if (adc_vals_f[i]==true)
            {
                coinc_map_mult_f->Fill(i,multiplicity_f);
            }
        }
    }

    return kTRUE;
}

UInt_t PPCoincMap::FindNonZeroBin(std::vector<Bool_t> &vec)
{
    UInt_t index = 0;
    while (!vec[index] && index<vec.size())
    {
        index++;
    }
    return index;
}

//_________________________________________________________________
void PPCoincMap::SlaveTerminate()
{


}

//_________________________________________________________________
void PPCoincMap::Terminate()
{
    std::cout << std::endl;

#ifdef DEBUG
    std::cout << "found "
            << std::setw(4) << mult_mismatch
            << " mismatched multiplicities b/t anything and fragment"
            << std::endl;
#endif
    FNameManager fnm;
    std::string base = fnm.GetHistFileBase(true);
    if (willSave==kTRUE)
    {
        TFile *histfile = new TFile(Form("%s%i.root",base.data(),runnumber[runcount-1]),"UPDATE");
        TDirectory *dir = dynamic_cast<TDirectory*>(histfile->Get("coinc_map"));
        if (dir==NULL)
        {
            dir = histfile->mkdir("coinc_map");
        }
        dir->cd();

        coinc_map_aa->Write("",TObject::kOverwrite);
        coinc_map_ff->Write("",TObject::kOverwrite);
        coinc_map_fa->Write("",TObject::kOverwrite);
        coinc_map_mult_a->Write("",TObject::kOverwrite);
        coinc_map_mult_f->Write("",TObject::kOverwrite);

        coinc_map_anota->Write("",TObject::kOverwrite);
        coinc_map_fnota->Write("",TObject::kOverwrite);
        coinc_mult_a->Write("",TObject::kOverwrite);
        coinc_mult_f->Write("",TObject::kOverwrite);

        histfile->cd();

        histfile->Close();

    }

}

void PPCoincMap::LoadSettings(ConfigEntry *dbentry)
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

    db->GetValues("PedThreshold",fPedThresh);
    if (fPedThresh.size()==0) throw -1;

#ifdef VERBOSE_PPCoincMap
    std::cout << std::setw(15) << "Pedestal" << ":"; PrintVector(fPed);
    std::cout << std::setw(15) << "Threshold" << ":"; PrintVector(fThresh);
    std::cout << std::setw(15) << "LowCut" << ":"; PrintVector(fLowCut);
    std::cout << std::setw(15) << "HighCut" << ":"; PrintVector(fHighCut);
#endif
}
