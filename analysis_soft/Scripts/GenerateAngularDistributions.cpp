#include <iostream>
#include <string>
#include <algorithm>
#include "TFile.h"
#include "TDirectory.h"
#include "TH2.h"
#include "TAxis.h"
#include "TGraph2DErrors.h"
#include "ColumnFileLoader.cpp"


using namespace std;

Bool_t FileGood(TFile* f)
{
    Bool_t flag=true;
    if (f==NULL)
    {
        std::cerr << "NULL file argument" << std::endl;
        flag = false;
    }
    else if (!f->IsOpen())
    {
        std::cerr << f->GetName() << " is not open" << std::endl;
        flag = false;
    }

    return flag;
}

TDirectory* GetDirectory(TFile* f, TString dir_name)
{
    TDirectory *dir = 0;
    if (f==NULL)
        return dir;

    dir = f->GetDirectory(dir_name.Data());
    if (dir==NULL)
    {
        std::cout << f->GetName() << ":/" << dir_name << " doesn't exist" << std::endl;
    }

    return dir;
}


void GenerateTGraph2D(std::string angle_fname, std::string yield_fname,
                      std::string out_root_fname)
{
    ColumnFileLoader angfile;
    angfile.LoadData(angle_fname);

    ColumnFileLoader yieldfile;
    yieldfile.LoadData(yield_fname);

    ColumnData theta  = angfile.GetColumnForDet(0,0);
    ColumnData utheta = angfile.GetColumnForDet(0,1);
    ColumnData phi    = angfile.GetColumnForDet(0,2);
    ColumnData uphi   = angfile.GetColumnForDet(0,3);

    ColumnData yield  = yieldfile.GetColumnForDet(0,3);
    ColumnData uyield = yieldfile.GetColumnForDet(0,4);

    UInt_t nstrips = 64;
    Double_t *thetap  = new Double_t[nstrips];
    Double_t *uthetap = new Double_t[nstrips];
    Double_t *phip    = new Double_t[nstrips];
    Double_t *uphip   = new Double_t[nstrips];
    Double_t *yieldp  = new Double_t[nstrips];
    Double_t *uyieldp = new Double_t[nstrips];

    std::copy(theta.begin(), theta.begin()+nstrips, thetap);
    std::copy(utheta.begin(), utheta.begin()+nstrips, uthetap);
    std::copy(phi.begin(), phi.begin()+nstrips, phip);
    std::copy(uphi.begin(), uphi.begin()+nstrips, uphip);
    std::copy(yield.begin(), yield.begin()+nstrips, yieldp);
    std::copy(uyield.begin(), uyield.begin()+nstrips, uyieldp);

    TGraph2DErrors *gr2 = new TGraph2DErrors(nstrips,
                                             thetap,
                                             phip,
                                             yieldp,
                                             uphip,
                                             uthetap,
                                             uyieldp);

    gr2->SetName("angdist_graph");
    //    gr2->Draw("tri2");

    TFile *f = new TFile(out_root_fname.data(),"UPDATE");
    if (! FileGood(f)) return;

    TDirectory *dir = GetDirectory(f,"results");
    if (dir==NULL) return;

    dir->cd();
    gr2->Write("",TObject::kOverwrite);

    std::cout << gr2->GetName() << " saved in "
            << out_root_fname << ":/" << dir->GetName()
            << std::endl;

    f->Close();

    delete [] thetap;
    delete [] uthetap;
    delete [] phip;
    delete [] uphip;
    delete [] yieldp;
    delete [] uyieldp;

}


void GenerateTH2D (TFile* f)
{
    if (! FileGood(f)) return;

    TDirectory *dir = GetDirectory(f,"results");
    if (dir==NULL) return;

    TGraph2DErrors* gr = dynamic_cast<TGraph2DErrors*>(dir->FindObjectAny("angdist_graph"));
    if (gr==NULL)
    {
        std::cerr << "angdist_graph doesn't exist" << std::endl;
        return;
    }

    TH2D* h = new TH2D("angdist_hist",";Theta (deg);Phi (deg); Yield (counts)",
                       90,0,180,
                       180,-180,180);

    Double_t* x = gr->GetX();
    Double_t* y = gr->GetY();
    Double_t* z = gr->GetZ();
    Double_t* ez = gr->GetEZ();

    TAxis *ax = h->GetXaxis();
    TAxis *ay = h->GetYaxis();
    Int_t gbin;
    for (UInt_t i=0; i<gr->GetN(); i++)
    {
        gbin = h->GetBin(ax->FindBin(x[i]),ay->FindBin(y[i]));
        h->SetBinContent(gbin, z[i]);
        h->SetBinError(gbin, ez[i]);
    }

    dir->cd();
    h->Write("",TObject::kOverwrite);
    std::cout << "Angular distribution histogram saved to "
            << f->GetName() << std::endl;
}
