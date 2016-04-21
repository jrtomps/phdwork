#include <iostream>
#include "TMath.h"
#include "AngMarginalEta2.h"
#include <CLHEP/Units/PhysicalConstants.h>

AngMarginalEta2::AngMarginalEta2(const Eta2& eta)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fMarginalEta(new TH2D("AngMarginalEta","",180,0,CLHEP::pi,36,-1.0*CLHEP::pi,CLHEP::pi)),
    fNorm(eta.GetNormalization())
{
    fMarginalEta->SetDirectory(0);
    SetUpMarginalEta(eta);
}

AngMarginalEta2::AngMarginalEta2(const AngMarginalEta2 &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fMarginalEta(obj.fMarginalEta),
    fNorm(obj.fNorm)
{
}

AngMarginalEta2* AngMarginalEta2::Clone() const
{
    return new AngMarginalEta2(*this);
}

const TH2* AngMarginalEta2::GetHist() const
{
    return fMarginalEta.get();
}

void AngMarginalEta2::SetUpMarginalEta(const Eta2& eta)
{

    // sum up all A
    //  sum up all Z
    //    sum up all depths
    TH1* h = eta.GetMassDistribution();
    TH3* h3 = 0;
    for (Int_t a=1; a<=h->GetNbinsX(); a++)
    {
        if (h->GetBinContent(a)>0)
        {
            for (Int_t z=1; z<=a; z++)
            {
                h3 = eta.GetAngleVsDepth(z,a);
                if (h3!=0)
                {
                    h3->GetZaxis()->SetRange(1,h3->GetNbinsZ());
                    TH1* h2 = h3->Project3D("yx");
                    h2->Scale(h->GetBinContent(a));

                    fMarginalEta->Add(h2,1.0);
                }
            }
        }
    }
}

Double_t AngMarginalEta2::DoEval(const Double_t *x) const
{
    // x[0] = theta
    // x[1] = phi

    // this expects the units to be degrees.
    Int_t theta_bin = fMarginalEta->GetXaxis()->FindBin(x[0]);
    Int_t phi_bin = fMarginalEta->GetYaxis()->FindBin(x[1]);
    return fMarginalEta->GetBinContent(theta_bin, phi_bin)/fNorm;
}
