
#include <sstream>
#include <stdexcept>
#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "Math/IFunction.h"
#include "Eta.h"

Eta::Eta(TFile* f)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFile(f)
{}

Eta* Eta::Clone() const
{
    return new Eta(fFile);
}

TH2* Eta::Eta2DAtTheta(UInt_t strip, UInt_t Z, UInt_t A, Double_t theta)
{
    TH3* h = GetTH3(strip, Z, A);
    if (h==0) throw std::runtime_error("In Eta:Eta2DAtTheta() null TH3 found");

    Int_t bin = h->GetXaxis()->FindBin(theta);
    h->GetXaxis()->SetRange(bin,bin+1);
    return dynamic_cast<TH2*>(h->Project3D("yz"));
}

TH2* Eta::Eta2DAtPhi(UInt_t strip, UInt_t Z, UInt_t A, Double_t phi)
{
    TH3* h = GetTH3(strip, Z, A);
    if (h==0) throw std::runtime_error("In Eta:Eta2DAtPhi() null TH3 found");

    Int_t bin = h->GetYaxis()->FindBin(phi);
    h->GetYaxis()->SetRange(bin,bin+1);
    return dynamic_cast<TH2*>(h->Project3D("xz"));
}

TH2* Eta::Eta2DAtDepth(UInt_t strip, UInt_t Z, UInt_t A, Double_t depth)
{
    TH3* h = GetTH3(strip, Z, A);
    if (h==0) throw std::runtime_error("In Eta:Eta2DAtDepth() null TH3 found");

    Int_t bin = h->GetZaxis()->FindBin(depth);
    h->GetZaxis()->SetRange(bin,bin+1);

    h->GetZaxis()->SetRangeUser(depth,depth);
    return dynamic_cast<TH2*>(h->Project3D("xy"));
}

TH3* Eta::GetTH3(UInt_t strip, UInt_t Z, UInt_t A) const
{
    if (fFile==0) throw std::runtime_error("In <Eta::GetTH3()> null file argument");

    TH3* h = 0;

    std::ostringstream dir_name;
    dir_name << A << "_" << Z;

    TDirectory* dir = fFile->GetDirectory(dir_name.str().data());
    if (dir!=0)
    {
        std::ostringstream hname;
        hname << "eta" << strip;
        h = dynamic_cast<TH3*>(dir->Get(hname.str().data()));
    }

    return h;
}

Double_t Eta::DoEval(const double *x) const
{
    // x[0] = strip
    // x[1] = Z;
    // x[2] = A;
    // x[3] = Theta;
    // x[4] = Phi;
    // x[5] = Depth
    TH3* h = GetTH3(x[0],x[1],x[2]);
    if (h==0) throw std::runtime_error("In Eta::DoEval() null TH3 found");

    Int_t xb = h->GetXaxis()->FindBin(x[3]);
    Int_t yb = h->GetYaxis()->FindBin(x[4]);
    Int_t zb = h->GetZaxis()->FindBin(x[5]);

    return h->GetBinContent(xb,yb,zb);
}

