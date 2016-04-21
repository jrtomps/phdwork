#include "TTree.h"
#include "TGraph.h"
#include "TFile.h"
#include <string>
#include <iostream>

Double_t um = 1.0e-3;
Double_t mm = 1.0;
Double_t cm = 10.0;

Double_t CreatePointFromTTreeFile(const std::string& fname)
{
    std::cout << "\tExtracting data from " << fname << std::endl;
    TFile* f = new TFile(fname.data());
    TTree* nt = static_cast<TTree*>(f->Get("Sim_Tree"));
    nt->SetBranchStatus("*",0);
    nt->SetBranchStatus("TarKE",1);
    nt->SetBranchStatus("TarTotDepE",1);

    Double_t tke, depe;
    nt->SetBranchAddress("TarKE",&tke);
    nt->SetBranchAddress("TarTotDepE",&depe);

    UInt_t nescape = 0;
    Int_t nentries = nt->GetEntries();
    for (Int_t i=0; i<nentries; i++)
    {
        nt->GetEntry(i);

        if (depe < tke) nescape++;

    }
    f->Close();

    return (nescape*1.0)/nentries;

}

Double_t GetVolume(const Double_t surf_thickness)
{
    Double_t w = 2.80*cm;
    Double_t l = 5.01*cm;
    Double_t h = 0.10*cm;

    return l*w*h - (l-2.0*surf_thickness)*(w-2.0*surf_thickness)*(h-2.0*surf_thickness);
}

TGraph* InspectProbEscapeVsSurfThickness()
{
    TGraph* gr = new TGraph(5);
    gr->SetName("prob_escape_graph");
    gr->SetMarkerStyle(21);
    gr->SetMarkerColor(kGreen+3);
    gr->SetLineColor(kGreen-6);
    gr->SetTitle("ProbEscapePerThickness;Surf Thickness (#mum);Prob Escape * Volume");
    Double_t point = 0, volume=0;
    point = CreatePointFromTTreeFile("../sim_output/alpha_source_surf5um_0.root");
    volume = GetVolume(5.0*um);
//    gr->SetPoint(0,5,point*volume);
    gr->SetPoint(0,5,point);

    point = CreatePointFromTTreeFile("../sim_output/alpha_source_surf15um_0.root");
    volume = GetVolume(15.0*um);
//    gr->SetPoint(1,15,point*volume);
    gr->SetPoint(1,15,point);

    point = CreatePointFromTTreeFile("../sim_output/alpha_source_surf30um_0.root");
    volume = GetVolume(30.0*um);
//    gr->SetPoint(2,30,point*volume);
    gr->SetPoint(2,30,point);

    point = CreatePointFromTTreeFile("../sim_output/alpha_source_surf45um_0.root");
    volume = GetVolume(45.0*um);
//    gr->SetPoint(3,45,point*volume);
    gr->SetPoint(3,45,point);

    point = CreatePointFromTTreeFile("../sim_output/alpha_source_surf60um_0.root");
    volume = GetVolume(60.0*um);
//    gr->SetPoint(4,60,point*volume);
    gr->SetPoint(4,60,point);

    return gr;

}
