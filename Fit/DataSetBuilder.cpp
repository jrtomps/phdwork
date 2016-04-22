#include <iostream>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <CLHEP/Units/SystemOfUnits.h>
#include "Math/DistSampler.h"
#include "Math/IFunction.h"
#include "Fit/DataRange.h"
#include "Fit/UnBinData.h"
#include "TRandom3.h"
#include "TH2.h"
#include "TGraph2DErrors.h"
#include "TAxis.h"
#include "TMath.h"
#include "TFoam.h"
#include "MakePositiveSmoothFcn.h"
#include "TFoamIntegrandWrapper.h"
#include "DataSetBuilder.h"

DataSetBuilder::DataSetBuilder()
    : ROOT::Math::DistSampler(),
    fFoam(new TFoam("DataSetBuilder::fFoam")),
    fRndmEng(),
    fIntWrap()
{
    fFoam->SetPseRan(&fRndmEng);
    fFoam->SetkDim(2);
}

DataSetBuilder::~DataSetBuilder()
{
}

void DataSetBuilder::SetFunction(ROOT::Math::IBaseFunctionMultiDim *func)
{
    if (func==NULL) throw 1000;
    if (func->NDim()!=2)  throw 1001;

    MakePositiveSmoothFcn pfunc(*func);
    ROOT::Math::DistSampler::SetFunction(pfunc);

    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> f_ptr(pfunc.Clone());
    fIntWrap.reset(new TFoamIntegrandWrapper(f_ptr));

    std::cout << "Set the TFoamIntegrandWrapper" << std::endl;
    fFoam->SetRho(fIntWrap.get());
    std::cout << "fFoam::GetRho() = "
            << std::hex << std::showbase << fFoam->GetRho()
            << std::dec << std::endl;
}

Bool_t DataSetBuilder::Init(const char *)
{
    fFoam->SetnCells(2000);
    fFoam->SetOptDrive(1); // Varedu option
    fFoam->Initialize();
    return true;
}

Bool_t DataSetBuilder::Sample(double *x)
{

    fFoam->MakeEvent();
    fFoam->GetMCvect(x);

    return kTRUE;
}

TGraph2DErrors* DataSetBuilder::GenerateSmallSampleGraph(UInt_t nsize, Double_t rel_err)
{
    TGraph2DErrors* gr = 0;

    if (NDim()!=2)
    {
        std::cerr << "Dimension mismatch" << std::endl;
        return gr;
    }

    Double_t xlow[2];
    Double_t xhi[2];
    PdfRange().GetRange(xlow,xhi);

    ROOT::Fit::UnBinData data(PdfRange(),nsize,2);
    ROOT::Fit::DataRange::RangeSet xrs = PdfRange().Ranges(0);
    ROOT::Fit::DataRange::RangeSet yrs = PdfRange().Ranges(1);

    const Double_t *x;
    UInt_t i=0;

    if (Generate(nsize,data)==false)
    {
        std::cerr << "Failed to generate sample" << std::endl;
        return gr;
    }

    gr = new TGraph2DErrors(nsize);

    Double_t xt[2];
    Double_t z;
    for (UInt_t i=0; i<data.NPoints(); i++)
    {
        x = data.Coords(i);
        xt[0] = TMath::ACos(2*x[0]-1.0)*CLHEP::rad;
        xt[1] = 2*TMath::Pi()*(x[1]-0.5)*CLHEP::rad;
        z = ParentPdf()(xt);

        xt[0]/=CLHEP::deg;
        xt[1]/=CLHEP::deg;

        gr->SetPoint(i,xt[0],xt[1],z);
        gr->SetPointError(i,
                          xt[0]*rel_err,
                          xt[1]*rel_err,
                          z*rel_err);
    }

    return gr;
}

TH2D* DataSetBuilder::GenerateSampleHist(UInt_t nsize)
{
    using CLHEP::deg;
    using CLHEP::rad;

    TH2D* h = 0;
    if (NDim()!=2)
    {
        std::cerr << "Dimension mismatch" << std::endl;
        return h;
    }

    Double_t xlow[2];
    Double_t xhi[2];
    PdfRange().GetRange(xlow,xhi);

    ROOT::Fit::UnBinData data(PdfRange(),nsize,2);
    ROOT::Fit::DataRange::RangeSet xrs = PdfRange().Ranges(0);
    ROOT::Fit::DataRange::RangeSet yrs = PdfRange().Ranges(1);

    h = new TH2D("h","",
                 180,
                 xrs[0].first/deg,
                 xrs[0].second/deg,
                 180,
                 yrs[0].first/deg,
                 yrs[0].second/deg);
    h->SetDirectory(0);

    const Double_t *x;
    UInt_t i=0;

    if (Generate(nsize,data)==false)
    {
        std::cerr << "Failed to generate sample" << std::endl;
        return h;
    }
    Double_t th, ph;
    for (UInt_t i=0; i<data.NPoints(); i++)
    {
        x = data.Coords(i);
        th = TMath::ACos(2*x[0]-1.0)*rad/deg;
        ph = 2*TMath::Pi()*(x[1]-0.5)*rad/deg;
        h->Fill(th,ph);
    }

    return h;
}

TGraph2DErrors* DataSetBuilder::GenerateSampleGraph(UInt_t nsize, Double_t rel_err)
{
    TGraph2DErrors* gr=NULL;

    boost::scoped_ptr<TH2D> h(GenerateSampleHist(nsize));

    TAxis *ax = h->GetXaxis();
    TAxis *ay = h->GetYaxis();

    UInt_t xfirst = ax->GetFirst();
    UInt_t xlast  = ax->GetLast();
    UInt_t yfirst = ay->GetFirst();
    UInt_t ylast  = ay->GetLast();

    UInt_t n = (xlast-xfirst +1)*(ylast-yfirst+1);
    gr = new TGraph2DErrors(n);
    UInt_t j=0;
    for(UInt_t binx=xfirst; binx<=xlast; binx++)
    {
        for (UInt_t biny=yfirst; biny<=ylast; biny++)
        {
            gr->SetPoint(j,
                         ax->GetBinCenter(binx),
                         ay->GetBinCenter(biny),
                         h->GetBinContent(binx,biny));

            gr->SetPointError(j,
                              ax->GetBinWidth(binx)/2.0,
                              ay->GetBinWidth(biny)/2.0,
                              h->GetBinContent(binx,biny)*rel_err);
            j++;
        }
    }

//    std::cout << "Copying complete" << std::endl;

    return gr;
}
