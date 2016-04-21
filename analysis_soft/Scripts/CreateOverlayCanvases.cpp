#include <iostream>
#include <vector>
#include "Visualizer.h"
#include "ConfigManager.h"
#include "FNameManager.h"
#include "ExtensibleDb.h"
#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TKey.h"
#include "TPRegexp.h"
#include "TLine.h"
using namespace std;

FNameManager ___fnm("fnameconfig.dat");

TList* GetAllDirectories(TList* li)
{
    TList *dir = new TList;
    TListIter next(li);
    TObject *key, *obj;

    TString can_dir("canvases");
    while((key = next()))
    {
        obj = (static_cast<TKey*>(key))->ReadObj();
        if (obj->InheritsFrom(TDirectory::Class())
            && ! can_dir.EqualTo(obj->GetName()) )
            dir->Add(obj);
    }

    return dir;
}

TDirectory* GetDirOrCreateIfNotThere(TDirectory* parent_dir, const TString& dirname)
{
    TDirectory *dir = parent_dir->GetDirectory(dirname);
    if (dir==NULL)
    {
        dir = parent_dir->mkdir(dirname);
    }

    return dir;
}

Int_t DetermineRunNumberFromFileName(TString name)
{
    Int_t run = 0;
    ConfigManager cm;
    TString cfile = ___fnm.GetConfigFileName(true).data();
    cm.LoadDbFromFile(cfile.Data());

    TPRegexp pre ("^((([0-9]+[a-zA-Z]+)|([a-zA-Z]+[0-9]+))_)+([0-9]+\.[0-9]{1}_)(.+)$");

    TString base = ___fnm.GetHistFileBase(false);
    if (name.Last('/')!=-1)
    {
        name.Remove(0,name.Last('/')+1);
    }

    name.Remove(0,base.Length());
    name.Remove(name.Length()-5,5);

    // 235U_44_Lin
    // 01234567890
    // Length = 11;
    // in = 7
    cout << name << endl;
    TObjArray* res = pre.MatchS(name);

    for (Int_t i = 0; i < res->GetLast()+1; i++) {
       const TString subStr = ((TObjString *)res->At(i))->GetString();
       cout << "\"" << subStr << "\" ";
    }
    cout << flush;

    TString pol = ((TObjString*) res->Last())->GetString();

    TString enstr = ((TObjString*) res->At(res->GetEntries()-2))->GetString();

    name.Remove(name.Index(pol), pol.Length());
    name.Remove(name.Index(enstr), enstr.Length());
    enstr.Remove(enstr.Length()-1,1);

    name.Remove(name.Length()-1,1);

    cout << name << "\t" << enstr << "\t" << pol << endl;
    vector<Int_t> runs = cm.GetListOfTargetRunsWithEnergyAndPol(name, enstr.Atof(), pol);
    if (runs.size()>0)
        run = runs.at(0);

    return run;
}

std::vector<TLine*> GenerateTLines(std::vector<Float_t> &xvals)
{
    std::vector<TLine*> lines(xvals.size(),static_cast<TLine*>(NULL));

    for (UInt_t i=0; i<xvals.size(); i++)
    {
        lines[i] = new TLine(xvals[i],0,xvals[i],10000);
    }

    return lines;
}


void CreateOverlayCanvases(TFile* f)
{
    if (f==NULL) return;
    if (!f->IsOpen())
    {
        cerr << f->GetName() << " doesn't exist" << endl;
        return;
    }

    TList *li = f->GetListOfKeys();

    TList *dir = GetAllDirectories(li);

    ConfigManager cm;
    cm.LoadDbFromFile(___fnm.GetConfigFileName(true).data());
    Int_t run = DetermineRunNumberFromFileName(f->GetName());
    cout << "Found run " << run << endl; cout.flush();
    if (run==0) return;

    ExtensibleDb* edb = cm.GetRun(run)->GetExtensibleDb();
    if (edb==NULL)
    {
        cerr << "No ExtensibleDb found for run " << run << endl;
        cerr.flush();
        return;
    }

    Visualizer vis;
    vector<TLine*> vlow, vhi;
    vector<Float_t> xlow, xhi;

    gROOT->SetBatch(true);
    std::cout << "Drawing ADC" << std::endl; cout.flush();
    TDirectory* d = static_cast<TDirectory*>(dir->FindObject("adc"));
    if (d) d->cd();
    vis.Draw("adc%i","logy");

    d = static_cast<TDirectory*>(dir->FindObject("inv_adc_cut"));
    if (d) d->cd();
    vis.Draw("inv_adc_cut%i","canvas=adc color=3 same");

    d = static_cast<TDirectory*>(dir->FindObject("adc_cut"));
    if (d) d->cd();
    vis.Draw("adc_cut%i","canvas=adc color=2 same");

    d = static_cast<TDirectory*>(dir->FindObject("adc_cut_tofcut"));
    if (d) d->cd();
    vis.Draw("adc_cut_tofcut%i",("canvas=adc color=6 same"));

    d = static_cast<TDirectory*>(dir->FindObject("adc_gt_thresh_tofcut"));
    if (d) d->cd();
    vis.Draw("adc_gt_thresh_tofcut","canvas=adc color=14 same");

    d = static_cast<TDirectory*>(dir->FindObject("adc_gt_thresh"));
    if (d) d->cd();
    vis.Draw("adc_gt_thresh",("canvas=adc color=7 same"));
    edb->GetValues("Threshold",xlow);
    vlow = GenerateTLines(xlow);
    vis.DrawLines(vlow,"canvas=adc color=4 same");

    ////////////////////////////////////////////////////////////
    std::cout << "Drawing TDC" << std::endl; cout.flush();
    d = static_cast<TDirectory*>(dir->FindObject("tdc"));
    if (d) d->cd();
    vis.Draw("tdc%i","logy");

    d = static_cast<TDirectory*>(dir->FindObject("inv_tdc_cut"));
    if (d) d->cd();
    vis.Draw("inv_tdc_cut%i",("canvas=tdc color=3 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tdc_cut"));
    if (d) d->cd();
    vis.Draw("tdc_cut%i",("canvas=tdc color=2 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tdc_cut_tofcut"));
    if (d) d->cd();
    vis.Draw("tdc_cut_tofcut%i",("canvas=tdc color=6 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tdc_gt_thresh"));
    if (d) d->cd();
    vis.Draw("tdc_gt_thresh",("canvas=tdc color=7 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tdc_gt_thresh_tofcut"));
    if (d) d->cd();
    vis.Draw("tdc_gt_thresh_tofcut","canvas=tdc color=14 same");

    d = static_cast<TDirectory*>(dir->FindObject("tof"));
    if (d) d->cd();
    vis.Draw("tof%i","logy");
    edb->GetValues("LowCut",xlow);
    edb->GetValues("HighCut",xhi);
    vlow = GenerateTLines(xlow);
    vhi = GenerateTLines(xhi);
    vis.DrawLines(vlow,"canvas=tdc color=4 same");
    vis.DrawLines(vhi,"canvas=tdc color=4 same");

    /////////////////////////////////////////////////////////////
    std::cout << "Drawing TOF" << std::endl; cout.flush();
    d = static_cast<TDirectory*>(dir->FindObject("inv_tof_cut"));
    if (d) d->cd();
    vis.Draw("inv_tof_cut%i",("canvas=tof color=3 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tof_cut"));
    if (d) d->cd();
    vis.Draw("tof_cut%i",("canvas=tof color=2 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tof_cut_tofcut"));
    if (d) d->cd();
    vis.Draw("tof_cut_tofcut%i",("canvas=tof color=6 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tof_gt_thresh"));
    if (d) d->cd();
    vis.Draw("tof_gt_thresh",("canvas=tof color=7 same"));

    d = static_cast<TDirectory*>(dir->FindObject("tof_gt_thresh_tofcut"));
    if (d) d->cd();
    vis.Draw("tof_gt_thresh_tofcut","canvas=tof color=14 same");

    edb->GetValues("LowTOFCut",xlow);
    edb->GetValues("HighTOFCut",xhi);
    vlow = GenerateTLines(xlow);
    vhi = GenerateTLines(xhi);
    vis.DrawLines(vlow,"canvas=tof color=4 same");
    vis.DrawLines(vhi,"canvas=tof color=4 same");

    TDirectory* can_dir = GetDirOrCreateIfNotThere(f, "canvases");
    vis.SaveAllCanvases(can_dir);

    gROOT->SetBatch(false);
    delete dir;

}

void CreateOverlayCanvases(const TString& filename)
{
    TFile *f = new TFile(filename, "UPDATE");

    CreateOverlayCanvases(f);

    f->Close();

}

