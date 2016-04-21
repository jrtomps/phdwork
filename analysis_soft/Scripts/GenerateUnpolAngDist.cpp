#include "Rtypes.h"
#include <string>
#include <iostream>
#include "TFile.h"
#include "TMath.h"
#include "TDirectory.h"
#include "TGraphErrors.h"

void GenerateUnpolAngDist(TDirectory* dir)
{
    std::string gr_phi0_name(dir->GetName());
    gr_phi0_name.append("_phi0_yield");

    std::string gr_phi90_name(dir->GetName());
    gr_phi90_name.append("_phi90_yield");


    TGraphErrors* gr0 = static_cast<TGraphErrors*>(dir->Get(gr_phi0_name.data()));
    TGraphErrors* gr90 = static_cast<TGraphErrors*>(dir->Get(gr_phi90_name.data()));

    std::string gr_ave_name(dir->GetName());
    gr_ave_name.append("_phiaveraged");

    TGraphErrors* gr_ave = new TGraphErrors(TMath::Min(gr0->GetN(), gr90->GetN()));
    gr_ave->SetName(gr_ave_name.data());

    Double_t tolerance = 2;
    Double_t x0=0, x1=0;
    Double_t y0=0, y1=0;
    Double_t ux0=0, ux1=0;
    Double_t uy0=0, uy1=0;
    Double_t ave=0, uave=0;
    Int_t count=0;
    for (Int_t i=0; i<gr0->GetN(); i++)
    {
        std::cout << "point " << i;
        gr0->GetPoint(i, x0, y0);
        ux0 = gr0->GetErrorX(i);
        uy0 = gr0->GetErrorY(i);

        Int_t j=0;
        for (; j<gr90->GetN(); j++)
        {
            gr90->GetPoint(j, x1, y1);
            ux1 = gr90->GetErrorX(j);
            uy1 = gr90->GetErrorY(j);

            if (TMath::Abs(x1-x0) > tolerance) continue;
            else break;
        }

        if (y0==0 || y1==0)
        {
            std::cout << " no match" << std::endl;
            continue;
        }
        else
        {
            std::cout << " " << j << std::endl;
        }


        ave = 0.5*(y0 + y1);
        uave = 0.5*TMath::Sqrt(uy0*y0 + uy1*uy1);

        gr_ave->SetPoint(count, x0, ave);
        gr_ave->SetPointError(count, ux0, uave);

        count++;
    }

    std::cout << "Averaged " << count << " points" << std::endl;
    gr_ave->Set(count);

    TDirectory* d = gDirectory;
    dir->cd();
    gr_ave->Write("",TObject::kOverwrite);

    d->cd();

}

