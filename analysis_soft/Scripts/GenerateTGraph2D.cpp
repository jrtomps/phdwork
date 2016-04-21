


#include <vector>
#include <iostream>
#include <string>
#include "TFile.h"
#include "TAxis.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TGraph2DErrors.h"
#include "ColumnFileLoader.h"

Int_t GenerateTGraph2D(std::string ang_dist_text_corr_file, std::string outfile)
{

    ColumnFileLoader cfl;
    cfl.LoadData(ang_dist_text_corr_file);

    ColumnData theta = cfl.GetColumnForDet(0,1);
    ColumnData integral = cfl.GetColumnForDet(0,2);
    ColumnData utheta = cfl.GetColumnForDet(0,3);
    ColumnData uintegral = cfl.GetColumnForDet(0,4);
    ColumnData phi = cfl.GetColumnForDet(0,5);
    ColumnData uphi = cfl.GetColumnForDet(0,6);
    ColumnData corrint = cfl.GetColumnForDet(0,9);
    ColumnData ucorrint = cfl.GetColumnForDet(0,10);


    TGraph2DErrors* gr = new TGraph2DErrors(theta.size(),
                                            theta.data(),
                                            phi.data(),
                                            integral.data(),
                                            utheta.data(),
                                            uphi.data(),
                                            uintegral.data());
    gr->SetName("graph2d");
    gr->SetTitle("Uncorrected Angular Dist");
    gr->GetXaxis()->SetTitle("Theta (deg)");
    gr->GetYaxis()->SetTitle("Phi (deg)");
    gr->GetZaxis()->SetTitle("Counts");
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(kGreen+3);
    gr->SetLineColor(kGreen-2);

//    gr->SetDirectory(0);

    TGraph2DErrors* gr_corr = new TGraph2DErrors(theta.size(),
                                                 theta.data(),
                                                 phi.data(),
                                                 corrint.data(),
                                                 utheta.data(),
                                                 uphi.data(),
                                                 ucorrint.data());
    gr_corr->SetName("corr_graph2d");
    gr_corr->SetTitle("SA Corrected Angular Dist");
    gr_corr->GetXaxis()->SetTitle("Theta (deg)");
    gr_corr->GetYaxis()->SetTitle("Phi (deg)");
    gr_corr->GetZaxis()->SetTitle("Yield");
    gr_corr->SetMarkerStyle(20);
    gr_corr->SetMarkerColor(kRed+1);
    gr_corr->SetLineColor(kRed);
//    gr_corr->SetDirectory(0);

    TFile f(outfile.data(),"UPDATE");
    if (!f.IsOpen()) return -1;

    TDirectory* dir = f.GetDirectory("uncorrected");
    if (dir==0) dir = f.mkdir("uncorrected");
    dir->cd();

    gr->Write("",TObject::kOverwrite);

    dir = f.GetDirectory("corrected");
    if (dir==0) dir = f.mkdir("corrected");
    dir->cd();

    gr_corr->Write("",TObject::kOverwrite);

    f.Close();

    //    delete gr;
    //    delete gr_corr;

    return 0;

}
