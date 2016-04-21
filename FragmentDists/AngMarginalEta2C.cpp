#include <iostream>
#include <stdexcept>
#include "TMath.h"
#include "AngMarginalEta2C.h"
#include <CLHEP/Units/PhysicalConstants.h>


AngMarginalEta2C::AngMarginalEta2C(TH2* h)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fMarginalEta(dynamic_cast<TH2*>(h->Clone("AngMarginalEta"))),
    fNorm(1),
    fZ(0),
    fA(0)
{
    fMarginalEta->SetDirectory(0);
}


AngMarginalEta2C::AngMarginalEta2C(const Eta2C& eta, UInt_t Z, UInt_t A)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fMarginalEta(new TH2D("AngMarginalEta","",200,-1.0,1.0,120,-1.0*CLHEP::pi,CLHEP::pi)),
    fNorm(eta.GetNormalization()),
    fZ(Z),
    fA(A)
{
    fMarginalEta->SetDirectory(0);
    SetUpMarginalEta(eta);
}

AngMarginalEta2C::AngMarginalEta2C(const AngMarginalEta2C &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fMarginalEta(obj.fMarginalEta),
    fNorm(obj.fNorm)
{
}

AngMarginalEta2C* AngMarginalEta2C::Clone() const
{
    return new AngMarginalEta2C(*this);
}

const TH2* AngMarginalEta2C::GetHist() const
{
    return fMarginalEta.get();
}

TH2* AngMarginalEta2C::GetHist()
{
    return fMarginalEta.get();
}

void AngMarginalEta2C::SetUpMarginalEta(const Eta2C& eta)
{

    // sum up all A
    //  sum up all Z
    //    sum up all depths
    TH1* hmass = eta.GetMassDistribution();
    Double_t mass_int = hmass->Integral();
    if (mass_int==0) throw std::runtime_error("in AngMarginalEta2C::SetUpMarginalEta() mass distr. integral = 0");

    if (fZ==0 || fA==0)
    {
        TH3* h3 = 0;
        for (Int_t a=1; a<=hmass->GetNbinsX(); a++)
        {
            if (hmass->GetBinContent(a)>0)
            {
                for (Int_t z=1; z<=a; z++)
                {
                    h3 = eta.GetAngleVsDepth(z,a);
                    if (h3!=0)
                    {
                        h3->SetDirectory(0);
                        h3->GetZaxis()->SetRange(1,h3->GetNbinsZ());
                        TH1* h2 = h3->Project3D("yx");
                        //                    std::cout << h2->Integral() << std::endl;
                        //                    std::cout << hmass->GetBinContent(a) << "/" << mass_int << std::endl;
                        h2->SetDirectory(0);
                        h2->Scale(hmass->GetBinContent(a)/mass_int);

                        fMarginalEta->Add(h2,1.0);
                        h2->Delete();
                        h3->Delete();
                    }

                }
            }
        }
    }
    else
    {

        TH3* h3 = 0;
        Int_t bin = hmass->FindBin(fA);
        if (hmass->GetBinContent(bin)>0)
        {
            h3 = eta.GetAngleVsDepth(fZ, fA);
            if (h3!=0)
            {
                h3->SetDirectory(0);
                h3->GetZaxis()->SetRange(1,h3->GetNbinsZ());
                TH1* h2 = h3->Project3D("yx");
                //                    std::cout << h2->Integral() << std::endl;
                //                    std::cout << hmass->GetBinContent(a) << "/" << mass_int << std::endl;
                h2->SetDirectory(0);
                h2->Scale(1.0/hmass->GetBinContent(bin));

                fMarginalEta->Add(h2,1.0);
                h2->Delete();
                h3->Delete();
            }

        }
    }

}

Double_t AngMarginalEta2C::DoEval(const Double_t *x) const
{
    // x[0] = cos(theta)
    // x[1] = phi in radians

    Int_t theta_bin = fMarginalEta->GetXaxis()->FindBin(x[0]);
    Int_t phi_bin = fMarginalEta->GetYaxis()->FindBin(x[1]);

    Double_t costheta_width = fMarginalEta->GetXaxis()->GetBinWidth(theta_bin);
    Double_t phi_width = fMarginalEta->GetYaxis()->GetBinWidth(phi_bin);

    return fMarginalEta->Interpolate(x[0],x[1]);
//    return fMarginalEta->Interpolate(x[0],x[1])/(fNorm*costheta_width*phi_width);
//    return fMarginalEta->GetBinContent(theta_bin, phi_bin)/fNorm;
}

