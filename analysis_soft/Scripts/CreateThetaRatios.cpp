#include "Rtypes.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TMath.h"
#include "StripInfo.h"
#include "TGraph2DErrors.h"
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <iomanip>

void PrintContributingThetaIndices(TGraph* gr, StripDb& db)
{

    std::cout << "\nContributing theta indices"
            << "\n" << std::endl;
    const Double_t *x = gr->GetX();

    for (Int_t i=0; i<gr->GetN(); i++)
    {
        StripPair strips = db.GetStripPair(x[i]);
        std::cout << db.GetThetaIndex(strips.first) << std::endl;
    }



}


void CreateLinearOverCircularRatios(TDirectory* dlin, TDirectory* dcirc, const std::string& angle_name)
{
    StripDb stdb(angle_name);

    TDirectory *original_dir = gDirectory;
    TGraphErrors* grlin = dynamic_cast<TGraphErrors*>(dlin->Get("ratio"));
    TGraphErrors* grcirc = dynamic_cast<TGraphErrors*>(dcirc->Get("ratio"));;

    if (grlin==0)
    {
        std::cerr << "\tFailed to find linear graph" << std::endl;
        return;
    }
    if (grcirc==0)
    {
        std::cerr << "\tFailed to find linear graph" << std::endl;
        return;
    }

    if (grlin->GetN()!=grcirc->GetN())
    {
        std::cerr << "\tWarning: Graphs have different number of points " << std::endl;
    }

    // assume that these are all in the same order
    // print out the x values just ot make sure...

    TGraphErrors* gr_ratio = new TGraphErrors(TMath::Min(grlin->GetN(),grcirc->GetN()));

    Double_t *xlin = grlin->GetX();
    Double_t *exlin = grlin->GetEX();
    Double_t *ylin = grlin->GetY();
    Double_t *eylin = grlin->GetEY();

    Double_t *xcirc = grcirc->GetX();
    Double_t *excirc = grcirc->GetEX();
    Double_t *ycirc = grcirc->GetY();
    Double_t *eycirc = grcirc->GetEY();


    std::cout << std::setiosflags(std::ios::scientific)
            << std::setprecision(2);

    using TMath::Sqrt;
    using TMath::Power;

    Double_t xratio, exratio;
    Double_t ratio, eratio;

    UInt_t n=0;
    Double_t tolerance = 2.0;
    for (Int_t i=0; i<grlin->GetN(); i++)
    {
        Int_t count = 0;
        for(UInt_t j=0; j<grcirc->GetN(); j++)
        {
            if (TMath::Abs(xcirc[j]-xlin[i])<tolerance) break;
            count++;
        }


        if (count!=grcirc->GetN())
        {
            if (ylin[i]==0 || ycirc[count]==0) continue;

            std::cout << "xlin = " << setw(10) << xlin[i]
                    << " xcirc = " << setw(10) << xcirc[count]
                    << std::endl;

            ratio = ylin[i]/ycirc[i];
            eratio = ratio * Sqrt( Power(eylin[i]/ylin[i],2.0) + Power(eycirc[count]/ycirc[count],2.0));

            xratio = 0.5*(xlin[i] + xcirc[count]);
            exratio = 0.5*Sqrt(Power(exlin[i],2.0) + Power(excirc[count],2.0));
            gr_ratio->SetPoint(count, xratio, ratio);
            gr_ratio->SetPointError(count, exratio, eratio);
            n++;
        }
    }
    gr_ratio->Set(n);

    PrintContributingThetaIndices(gr_ratio, stdb);

    std::string ofname;
    std::cout << "Where to save file?" << std::endl;
    getline(std::cin, ofname);

    TFile* f = new TFile(ofname.data(), "update");
    f->cd();
    grlin->SetName("theta_yield_ratio_lin");
    grlin->SetTitle("Theta Yield Ratios : Linear");
    grlin->Write("",TObject::kOverwrite);

    grcirc->SetName("theta_yield_ratio_circ");
    grcirc->SetTitle("Theta Yield Ratios : Circular");
    grcirc->Write("",TObject::kOverwrite);

    gr_ratio->SetName("theta_yields_ratio_lin_over_circ");
    gr_ratio->SetTitle("Theta Yield Ratios : Linear/Circular");
    gr_ratio->Write("",TObject::kOverwrite);

    f->Close();

    original_dir->cd();
}
