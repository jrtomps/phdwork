// SelectPPCoincMapRegions.cpp
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <boost/bind.hpp>

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
//#include "CombinationEngine.h"
#include "MultiplicityGroupGenerator.h"
#include "SelectPPCoincMapRegions.h"

ClassImp(SelectPPCoincMapRegions);

const Char_t*  SelectPPCoincMapRegions::outfname_base = "pffrag2_hist_run";

//___________________________________________________________________________
SelectPPCoincMapRegions::SelectPPCoincMapRegions(UInt_t nregions, UInt_t lowch, UInt_t hich, ConfigEntry *ce, bool save)
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
    fGlobalUpperBound(),
    fNRegions(nregions),
    ndet(ce->GetNDets()),
    nadc_ch(ce->GetNADCChannels()),
    ntdc_ch(ce->GetNTDCChannels()), 

    regional_coinc(),
    regional_coinc_map(),

    data_in_region_of_strip(fNRegions+2, std::vector<Bool_t>()),

    runnumber(),
    starttime(),
    endtime(),
    n_phys_events(),
    region_multiplicity(fNRegions+2,0),
    mult_mismatch(fNRegions+2,0),
    fMaxMult(6),
    fChLow(lowch),
    fChHigh(hich)
{
#ifdef DEBUG
    std::cout << "Constructing PPCoinMap" << std::endl; std::cout.flush();
#endif
    willSave = save;

    std::for_each(data_in_region_of_strip.begin(),
                  data_in_region_of_strip.end(),
                  boost::bind(&std::vector<Bool_t>::resize, _1, 96, false));

    for (UInt_t i=1; i<=fMaxMult; i++)
    {
        MultiplicityGroupGenerator mgg(i,fNRegions+2);

        regional_coinc_map.insert(std::make_pair(i,mgg.GetCombos()));
        regional_coinc[i] = NULL;
    }

    runcount=0;
    if (ce)
    {
        pce = ce;
        LoadSettings(pce);
        runnumber.push_back(pce->GetRun());
    }
    else {
        std::cout << "Error in <SelectPPCoincMapRegions::SelectPPCoincMapRegions> : Invalid ConfigEntry" << std::endl;
        TSelector::Abort("Invalid config entry",TSelector::kAbortProcess);
        return;
    }
}

SelectPPCoincMapRegions::SelectPPCoincMapRegions(SelectPPCoincMapRegions const& obj)
    : fPed(),
    fThresh(),
    fLowCut(),
    fHighCut(),
    fLowCut2(),
    fHighCut2(),
    fLowCut3(),
    fHighCut3(),
    fPedThresh(),
    fGlobalUpperBound(),
    fNRegions(0),

    regional_coinc(),
    regional_coinc_map(),

    data_in_region_of_strip(),

    region_multiplicity(fNRegions+2,0),
    mult_mismatch(fNRegions+2,0),
    fMaxMult(obj.fMaxMult),
    fChLow(obj.fChLow),
    fChHigh(obj.fChHigh)
{
#ifdef DEBUG
    std::cout << "Copy constructing SelectPPCoincMapRegions" << std::endl; std::cout.flush();
#endif

    if (this != &obj)
    {
        runcount = obj.runcount;
        nentries = obj.nentries;
        fNRegions = obj.fNRegions;
        regional_coinc_map = obj.regional_coinc_map;

        pce = obj.pce;   // Do not copy the object
        LoadSettings (pce);

        std::map<UInt_t, TH1D*>::const_iterator it;
        std::string key;

        it = obj.regional_coinc.begin();
        for (;it!=obj.regional_coinc.end(); it++)
        {
            TH1D* tmp = static_cast<TH1D*>(it->second->Clone());
            tmp->SetDirectory(0);

            regional_coinc.insert(std::make_pair(it->first, tmp));
        }

        runnumber = obj.runnumber;
        starttime = obj.starttime;
        endtime = obj.endtime;
        n_phys_events = obj.n_phys_events;

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

        data_in_region_of_strip = obj.data_in_region_of_strip;

        willSave = obj.willSave;

        region_multiplicity = obj.region_multiplicity;

        mult_mismatch = obj.mult_mismatch;

    }
}

void SelectPPCoincMapRegions::SafeDeleteTH2D(TH2D* hist)
{
    if (hist!=NULL) delete hist;
    hist=0;
}

void SelectPPCoincMapRegions::SafeDeleteTH1D(TH1D* hist)
{
    if (hist!=NULL) delete hist;
    hist=0;
}

//_________________________________________________________________
SelectPPCoincMapRegions::~SelectPPCoincMapRegions()
{
#ifdef DEBUG
    std::cout << "destructing" << std::endl;
#endif

    std::map<UInt_t,TH1D*>::iterator it;
    for (it=regional_coinc.begin(); it!=regional_coinc.end(); it++)
    {
        SafeDeleteTH1D(it->second);
    }

}

//_________________________________________________________________
void SelectPPCoincMapRegions::Begin(TTree* /*tree*/)
{
#ifdef DEBUG
    std::cout << "begin" << std::endl;
#endif

    TString option = GetOption();

    PrintBoundaryInfo();

}

void SelectPPCoincMapRegions::PrintBoundaryInfo(void)
{
    Float_t pv = std::accumulate(fPedThresh.begin(), fPedThresh.end(), 0, std::plus<Float_t>());
    pv /= fPedThresh.size();

    Float_t v = std::accumulate(fThresh.begin(), fThresh.end(), 0, std::plus<Float_t>());
    v /= fThresh.size();

    Float_t uv = std::accumulate(fGlobalUpperBound.begin(),
                                 fGlobalUpperBound.end(),
                                 0,
                                 std::plus<Float_t>());
    uv /= fGlobalUpperBound.size();

    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

    std::cout << std::left << std::setw(20) << "\nNRegions" << ":"
            << std::right << std::setw(5) << fNRegions
            << std::left << std::setw(20) << "\nAve Ped Thresh" << ":"
            << std::right << std::setw(5) << pv
            << std::left << std::setw(20) << "\nAve Thresh" << ":"
            << std::right << std::setw(5) << v;

    Float_t width = (uv - v)/fNRegions;
    for (UInt_t i=0; i<fNRegions; i++)
    {
        std::cout << std::left << "\nBound" << std::setw(14) << i << ":"
                << std::right << std::setw(5) << v+(i+1)*width;
    }
    std::cout << std::endl;
    std::cout << std::resetiosflags(std::ios::fixed) << std::setprecision(6);

}

//_________________________________________________________________
void SelectPPCoincMapRegions::SlaveBegin(TTree* /*tree*/)
{
#ifdef DEBUG
    std::cout << "slave begin" << std::endl;
#endif

    TH1::SetDefaultSumw2(true);

    TString option = GetOption();

    std::map<UInt_t,TH1D*>::iterator it;
    it=regional_coinc.begin();

    UInt_t i;
    std::ostringstream ss("",std::ios::out | std::ios::app);
    std::ostringstream sst("",std::ios::out | std::ios::app);

    for (; it!=regional_coinc.end(); it++)
    {
        ss.str("mult"); ss << fChLow << "to" << fChHigh << "_" << it->first;

        sst.str("Multiplicity "); sst << it->first;
        sst << ": Chs. " << fChLow << " to " << fChHigh;
        sst << " Events;Multiplicity Mask; Counts";
        it->second = new TH1D(ss.str().data(),
                              ss.str().data(),
                              regional_coinc_map[it->first].size(),
                              0,
                              regional_coinc_map[it->first].size());

        std::set<std::string>::reverse_iterator it2 = regional_coinc_map[it->first].rbegin();
//        std::cout << "\nMultiplicity " << it->first;
//        std::cout << "\n--------------------" << std::endl;
        for (UInt_t j=1; j<=it->second->GetNbinsX(); j++, it2++)
        {
//            std::cout << j << "\t" << *it2 << std::endl;
            it->second->GetXaxis()->SetBinLabel(j,it2->data());
        }
    }

}
//_________________________________________________________________
void SelectPPCoincMapRegions::Init(TTree* tree)
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
Bool_t SelectPPCoincMapRegions::Process(Long64_t entry)
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

    bpmval = 0;
    tdcval = 0;
    Double_t adc_val, tdc_val;
    Float_t tofval;
    std::fill(region_multiplicity.begin(), region_multiplicity.end(),0);
    std::fill(mult_mismatch.begin(), mult_mismatch.end(),0);

    UInt_t region = 0;
    for (incr=fChLow;incr<=fChHigh;incr++)
    {
        for (UInt_t j=0; j<fNRegions+2; j++)
        {
            data_in_region_of_strip[j][incr] = false;
        }

        b_adc[incr]->GetEntry(entry);
        adc_val = nt_adc[incr];


        b_tdc[incr]->GetEntry(entry);
        tdc_val = nt_tdc[incr];

        // TOF corresponds to appropriate BPM channel
        tofval = nt_tdc[32*(incr/32+1)-1]-tdc_val;

        if (adc_val>fPedThresh[incr])
        {
            region = ComputeRegion(incr, adc_val);
            data_in_region_of_strip[region][incr] = true;
            region_multiplicity[region]++;
        }
    }

    // count nonzero regions
    Int_t nregions = std::count_if(region_multiplicity.begin(),
                                   region_multiplicity.end(),
                                   std::bind2nd(std::greater<UInt_t>(),0));

    // sum the multiplicities of each region
    Int_t mult = std::accumulate(region_multiplicity.begin(),
                                 region_multiplicity.end(),
                                 0,
                                 std::plus<UInt_t>());


//    std::cout << "Generating mask " << std::endl; std::cout.flush();
    std::string mask = GenerateMask(region_multiplicity);
//    std::cout << "consolidating mask " << mask << std::endl; std::cout.flush();
//    mask = MultiplicityGroupGenerator::ConsolidateCombo(mask, fNRegions+2);

    if (mult>0 && mult<=fMaxMult)
    {
//        std::cout << "filling mult=" << mult << "\tmask=" << mask << std::endl;  std::cout.flush();
        regional_coinc[mult]->Fill(mask.data(),1.0);
    }

    return kTRUE;
}

std::string SelectPPCoincMapRegions::GenerateMask(std::vector<UInt_t>& vec)
{
    std::ostringstream ss;
    for (UInt_t i=0; i<vec.size(); i++)
    {
        ss << vec[i];
    }
    return ss.str();
}

UInt_t SelectPPCoincMapRegions::ComputeRegion(UInt_t det_index, Double_t adc_val)
{
    UInt_t reg = 0;
    Double_t region_width = (fGlobalUpperBound[det_index]-fThresh[det_index])/fNRegions;
    if (adc_val<fThresh[det_index])
    {
        reg = 0;
    }
    else if (adc_val>fGlobalUpperBound[det_index])
    {
        reg = fNRegions+1;
    }
    else
    {
        UInt_t flr = static_cast<UInt_t>(floor( (adc_val-fThresh[det_index]) / region_width ));
        reg = flr + 1;
    }

    return reg;
}

UInt_t SelectPPCoincMapRegions::FindNonZeroBin(std::vector<Bool_t> &vec)
{
    UInt_t index = 0;
    while (!vec[index] && index<vec.size())
    {
        index++;
    }
    return index;
}

//_________________________________________________________________
void SelectPPCoincMapRegions::SlaveTerminate()
{


}

//_________________________________________________________________
void SelectPPCoincMapRegions::Terminate()
{
    std::cout << std::endl;

#ifdef DEBUG
    std::cout << "found mismatched multiplicities b/t anything and fragment ";
    for (UInt_t i=0; i<fNRegions+2; i++)
    {
        std::cout << "\nregion: " << std::setw(3) << i
                << "mult: " << std::setw(10) << std::setw(4) << mult_mismatch[i];
    }
    std::cout << std::endl;

#endif
    FNameManager fnm;
    std::string base = fnm.GetHistFileBase(true);
    if (willSave==kTRUE)
    {
        TFile *histfile = new TFile(Form("%s%i.root",base.data(),runnumber[runcount-1]),"UPDATE");
        TDirectory *dir = dynamic_cast<TDirectory*>(histfile->Get("coinc_map_regions"));
        if (dir==0)
        {
            dir = histfile->mkdir("coinc_map_regions");
        }
        dir->cd();

        std::map<UInt_t,TH1D*>::iterator it;
        for (it=regional_coinc.begin(); it!=regional_coinc.end(); it++)
        {
            it->second->Write("",TObject::kOverwrite);
        }

        histfile->cd();

        histfile->Close();

    }

}

void SelectPPCoincMapRegions::LoadSettings(ConfigEntry *dbentry)
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

    db->GetValues("GlobalUpperBound", fGlobalUpperBound);
    if (fGlobalUpperBound.size()==0) throw -1;

#ifdef VERBOSE_SelectPPCoincMapRegions
    std::cout << std::setw(15) << "Pedestal" << ":"; PrintVector(fPed);
    std::cout << std::setw(15) << "Threshold" << ":"; PrintVector(fThresh);
    std::cout << std::setw(15) << "LowCut" << ":"; PrintVector(fLowCut);
    std::cout << std::setw(15) << "HighCut" << ":"; PrintVector(fHighCut);
#endif
}
