
#include <string>
#include <sstream>
#include <stdexcept>
//#include <boost/regex.hpp>
#include <regex>
#include <CLHEP/Units/PhysicalConstants.h>
#include "TFile.h"
#include "TF1.h"
#include "TH2.h"
#include "TH3.h"
#include "Math/IFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "SphIntegrand.h"
#include "TH2Func.h"
#include "Eta2.h"
#include "SphCoordsIntegrand.h"
#include "TList.h"
#include "TKey.h"
#include "TMath.h"
#include "TObject.h"

Eta2::Eta2(TFile* f)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFile(f),
    fNorm(1)
{
    //ComputeNormalization();
    EnforceProbabilities();
}

Eta2* Eta2::Clone() const
{
    return new Eta2(fFile);
}

UInt_t Eta2::NDim() const
{
    TH1* h = GetTKEDistribution();
    if (h==0)
        return 5;
    else
        return 6;
}

TH2* Eta2::Eta2DAtTheta(UInt_t Z, UInt_t A, Double_t theta)
{
    TH3* h = GetTH3(Z, A);
    if (h==0) throw std::runtime_error("In Eta2:Eta2DAtTheta() null TH3 found");

    Int_t bin = h->GetXaxis()->FindBin(theta);
    h->GetXaxis()->SetRange(bin,bin+1);
    return dynamic_cast<TH2*>(h->Project3D("zy"));
}

TH2* Eta2::Eta2DAtPhi(UInt_t Z, UInt_t A, Double_t phi)
{
    TH3* h = GetTH3(Z, A);
    if (h==0) throw std::runtime_error("In Eta2:Eta2DAtPhi() null TH3 found");

    Int_t bin = h->GetYaxis()->FindBin(phi);
    h->GetYaxis()->SetRange(bin,bin+1);
    return dynamic_cast<TH2*>(h->Project3D("zx"));
}

TH2* Eta2::Eta2DAtDepth(UInt_t Z, UInt_t A, Double_t depth)
{
    TH3* h = GetTH3( Z, A);
    if (h==0) throw std::runtime_error("In Eta2:Eta2DAtDepth() null TH3 found");

    Int_t bin = h->GetZaxis()->FindBin(depth);
    h->GetZaxis()->SetRange(bin,bin+1);

    //h->GetZaxis()->SetRangeUser(depth,depth);

    return dynamic_cast<TH2*>(h->Project3D("yx"));
}

TH3* Eta2::GetAngleVsDepth(UInt_t Z, UInt_t A) const
{
    return GetTH3(Z,A);
}

UInt_t Eta2::GetNIsotopes() const
{
    UInt_t count=0;
    TListIter next(fFile->GetListOfKeys());
    TKey* key=0;
    while ( (key = dynamic_cast<TKey*>(next()) ) )
    {
        if (std::string("TDirectory") == key->GetClassName())
        {
            TDirectory* dir = dynamic_cast<TDirectory*>(key->ReadObj());
            if (IsIsotopeFolder(dir)) count++;
        }
    }
    return count;
}

TH1* Eta2::GetMassDistribution() const
{
   return dynamic_cast<TH1*>(fFile->FindObjectAny("mass_distribution"));
}

TH1* Eta2::GetTKEDistribution() const
{
    return dynamic_cast<TH1*>(fFile->FindObjectAny("tke_distribution"));
}

Double_t Eta2::GetNormalization() const
{
    return fNorm;
}

Bool_t Eta2::IsIsotopeFolder(TDirectory *dir) const
{
    Bool_t flag = false;
    if (dir!=0)
    {
      std::regex expr("^[0-9]+_[0-9]+$");
        flag = std::regex_match(dir->GetName(),expr);
    }
    return flag;
}


TH3* Eta2::GetTH3(UInt_t Z, UInt_t A) const
{
    if (fFile==0) throw std::runtime_error("In <Eta2::GetTH3()> null file argument");

    TH3* h = 0;

    std::ostringstream dir_name;
    dir_name << A << "_" << Z;

    TDirectory* dir = fFile->GetDirectory(dir_name.str().data());
    if (dir!=0)
    {
        std::string hname("eta_");
        hname.append(dir_name.str());
        h = dynamic_cast<TH3*>(dir->Get(hname.data()));
    }

    return h;
}

void Eta2::EnforceProbabilities()
{
//    fNorm = 1;
//    TH1* h = 0;

//    h = GetMassDistribution();
//    if (h==0) throw std::runtime_error("in Eta2::GetNormalization() no mass distribution found");

    TKey *key = 0;
    TListIter next(fFile->GetListOfKeys());
    while ( (key = static_cast<TKey*>(next())) )
    {
        if (std::string("TDirectoryFile") == key->GetClassName())
        {
            std::cout << "Found " << key->ReadObj()->GetName() << std::endl;
            TDirectory* dir = static_cast<TDirectory*>(key->ReadObj());
            std::ostringstream os;
            os << "eta_" << dir->GetName();
            TH3* h3 = static_cast<TH3*>(dir->Get(os.str().data()));
            if (h3!=0)
            {
//                Double_t n = ComputeIntegral(h3);
//                h3->Scale(1.0/n);
            }
        }

    }


}

Double_t Eta2::ComputeIntegral(TH3* h)
{
    Double_t prob_escape = h->Integral();

    // use a real integration to compute this...Because I use this in true integral, it is
    // necessary that the normalization is computed in the same fashion.
    h->GetZaxis()->SetRange(1,h->GetNbinsZ());
    TH2* hang = dynamic_cast<TH2*>(h->Project3D("yx"));
    boost::shared_ptr<FS232Th::TH2Func> h2func(new FS232Th::TH2Func(hang));
    ROOT::Math::IntegratorMultiDim integrator;
    integrator.SetFunction(*h2func);

    Double_t xlow[] = {0, -1.0*CLHEP::pi};
    Double_t xhi[] = {CLHEP::pi, CLHEP::pi};
    Double_t integ = integrator.Integral(xlow,xhi);

    return integ/prob_escape;
}

Double_t Eta2::GetEnergyWeight(Double_t E) const
{
    // needs to finished
    return 1.;
}

Double_t Eta2::DoEval(const double *x) const
{
    // x[0] = Z;
    // x[1] = A;
    // x[2] = CosTheta;
    // x[3] = Phi;
    // x[4] = Depth
    TH3* h = GetTH3(x[0],x[1]);
    if (h==0) throw std::runtime_error("In Eta2C::DoEval() null TH3 found");

    Double_t res;

    TH1* hmass = GetMassDistribution();
    Int_t bin = hmass->GetXaxis()->FindBin(x[1]);
    Double_t prob_is_mass = hmass->GetBinContent(bin)/hmass->Integral();

    Int_t xb = h->GetXaxis()->FindBin(x[2]);
    Int_t yb = h->GetYaxis()->FindBin(x[3]);
    Int_t zb = h->GetZaxis()->FindBin(x[4]);

    res = h->Interpolate(x[2],x[3],x[4]);

//    // Because the histogram bins have width, the width needs to be divided out
//    res /= (h->GetXaxis()->GetBinWidth(xb));
//    res /= (h->GetYaxis()->GetBinWidth(yb));
//    res /= (h->GetZaxis()->GetBinWidth(zb));


    return res*prob_is_mass;
}

