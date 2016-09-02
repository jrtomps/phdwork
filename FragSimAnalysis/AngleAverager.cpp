// AngleAverager.cpp
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <CLHEP/Units/PhysicalConstants.h>
#include "TH1.h"
#include "TObject.h"
#include "TDirectory.h"
#include "TString.h"
#include "TColor.h"
#include "TMath.h"
#include "TSelectorInsert.h"
#include "AngleAverager.h"
#include "DataBucket.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph2DErrors.h"
#include "PhDConfig.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <typeinfo>

#ifdef DEBUG_AngleAverager
#define ENTRY entry
#else
#define ENTRY 
#endif

ClassImp(AngleAverager)

AngleAverager::AngleAverager(TTree* tree, TFile *outfile, TFile* infile, Double_t ethresh)
    : TSelectorInsert(),
      fNDets(4),
      fNStrips(16),
      fTree(tree),
      fFile(outfile),
      fThetaHists(fNStrips*fNDets,(TH1D*) NULL),
      fPhiHists(fNStrips*fNDets,(TH1D*) NULL),
      fTheta(0),
      fPhi(0),
      fStripID(0),
      fDetID(0),
      fTotalEvents(0),
      fTarTotDepE(0),
      fTarKE(0),
      fMinSSDTotDepE(ethresh)
{

    const char* pAnalDir = std::getenv("PHD_ANALYSIS_DIR");
    if  (pAnalDir == nullptr) {
      throw std::runtime_error("User must set PHD_ANALYSIS_DIR environment variable");
    } else {
      fAnalysisDir = pAnalDir;
    }

#ifdef DEBUG_AngleAverager
    std::cout << "AngleAverager constructed for TTree="
              << fTree->GetName()
              << std::endl;
#endif
    if (!fFile->IsOpen())
    {
        fFile = new TFile("AngleAverager_out.root","UPDATE");
    }
    fFile->ReOpen("UPDATE");

    ConstructHists();

    DataBucket *db = dynamic_cast<DataBucket*>(infile->Get("sim_data"));
    if (db!=0)
    {
        db->DumpContents();
        fTotalEvents = db->GetIntByKey("nevents");
    }
    if (fTotalEvents==0) throw AngleAveragerException();

}

AngleAverager::~AngleAverager()
{
}

Bool_t 
AngleAverager::Process(const Long64_t ENTRY)
{
#ifdef DEBUG_AngleAverager
    if (entry%200==0)
    {
        std::cout << "\rProcessing event"
                  << std::setw(8) << std::right << entry
                  << " for " << std::setw(8) << fDetID*fNStrips +fStripID;
    }
    if (entry==fTree->GetEntries())
        std::cout << std::endl;
#endif

    if (fTarKE - fTarTotDepE <= fMinSSDTotDepE) return false;

    Int_t index = fDetID*fNStrips+fStripID;
    if (index<0 ) return false;

    fThetaHists.at(index)->Fill(TMath::Cos(fTheta));
    fPhiHists.at(index)->Fill(fPhi);
    return true;
}

void
AngleAverager::Save(void)
{
    fFile->cd("AngleAveragerOutput");
    for (UInt_t i=0; i<fThetaHists.size(); i++)
        fThetaHists.at(i)->Write("",TObject::kOverwrite);


    for (UInt_t i=0; i<fPhiHists.size(); i++)
        fPhiHists.at(i)->Write("",TObject::kOverwrite);


    TGraph2DErrors* gr = GenerateTGraph2DErrors();
    gr->SetName("ang_dist");
    gr->SetTitle("ang_dist");
    gr->Write("", TObject::kOverwrite);
    fFile->cd();

    delete gr;
}

void
AngleAverager::ConstructHists(void)
{
    if (fFile->Get("AngleAveragerOutput")==NULL)
        fFile->mkdir("AngleAveragerOutput","AngleAveragerOutput");

    fFile->cd("AngleAveragerOutput");

    TString name;
    for (UInt_t i=0; i<fThetaHists.size(); i++)
    {
        fThetaHists.at(i) = new TH1D("","",200,-1,1);
        name = FormName(i);
        name += "_theta";
        fThetaHists.at(i)->SetName(name.Data());
    }
    for (UInt_t i=0; i<fPhiHists.size(); i++)
    {
        fPhiHists.at(i) = new TH1D("","",360,-1.0*CLHEP::pi,CLHEP::pi);
        name = FormName(i);
        name += "_phi";
        fPhiHists.at(i)->SetName(name.Data());
    }
    fFile->cd();

}

void
AngleAverager::LoadHists()
{
    TDirectory* dir = fFile->GetDirectory("AngleAveragerOutput");
    if (dir==NULL)
        dir = fFile->mkdir("AngleAveragerOutput","AngleAveragerOutput");

    TString name;
    TH1D* hp;
    for (UInt_t i=0; i<fThetaHists.size(); i++)
    {
        name = FormName(i);
        name += "_theta";
        hp = dynamic_cast<TH1D*>(dir->Get(name));
        if (hp)
        {
            fThetaHists.at(i) = hp;
        }
    }
    for (UInt_t i=0; i<fPhiHists.size(); i++)
    {
        name = FormName(i);
        name += "_phi";
        hp = dynamic_cast<TH1D*>(dir->Get(name));
        if (hp)
        {
            fPhiHists.at(i) = hp;
        }
    }

}


void
AngleAverager::GetValuesFromProxy(TTreeProxy *proxy)
{
    fTheta   = *(proxy->GetAddressOfDouble_t("TarTheta"));
    fPhi     = *(proxy->GetAddressOfDouble_t("TarPhi"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fTarTotDepE = *(proxy->GetAddressOfDouble_t("TarTotDepE"));
    fTarKE = *(proxy->GetAddressOfDouble_t("TarKE"));

}


TString
AngleAverager::FormName(const Int_t i) const
{
    return TString::Format("det%i_strip%i",i/fNStrips, i%fNStrips);
}

void
AngleAverager::PrintResults(std::ostream& stream)
{
    PrintThetaResults(stream);
    PrintPhiResults(stream);
    PrintResultsForExpData();
}

void
AngleAverager::PrintResultsForExpData()
{
    for (Int_t i=0; i<fNDets; i++)
    {
        TString fname = TString::Format("%s/angle_data/angles%i",fAnalysisDir.c_str(), i);
        std::ofstream stream(fname, std::ofstream::out);
        if (stream.fail()) return;

        stream << std::setiosflags(std::ios::fixed)
               << std::left
               << std::setprecision(3);
        PrintResultsForExpData(stream, i);
        stream.close();
    }

    ConcatenateResultsForExpData(0,fNDets);
}

void
AngleAverager::PrintResultsForExpData(std::ostream& stream, UInt_t det_index)
{
    if (det_index>static_cast<UInt_t>(fNDets)) return;

    stream << std::right;

    std::ios::fmtflags angle_fmt = std::ios::fixed;
    std::ios::fmtflags yield_fmt = std::ios::fixed;

    for (UInt_t i=det_index*fNStrips; i<(det_index+1)*fNStrips; i++)
    {
        stream << std::setiosflags(angle_fmt)
                << std::setw(8) << fThetaHists.at(i)->GetMean()
                << std::setw(8) << fThetaHists.at(i)->GetRMS()
                << std::setw(8) << ComputePhiMean(fPhiHists.at(i))
                << std::setw(8) << ComputePhiStdDev(fPhiHists.at(i))
                << std::resetiosflags(angle_fmt)
                << std::setiosflags(yield_fmt) << std::setprecision(1)
                << std::setw(8) << fThetaHists.at(i)->Integral()
                << std::setw(8) << TMath::Sqrt(fThetaHists.at(i)->Integral())
                << std::resetiosflags(yield_fmt) << std::setprecision(3)
                << "\n";
    }

}

void
AngleAverager::ConcatenateResultsForExpData(UInt_t first_det_index, UInt_t ndets)
{
    TString fname((fAnalysisDir + "/angle_data/angles").c_str());
    std::ofstream stream(fname, std::ofstream::out);
    if (stream.fail()) return;

    stream << std::setiosflags(std::ios::fixed)
           << std::left
           << std::setprecision(3);
    stream << "Data from test.root, Units: degrees"
           << "\n"
           << std::setw(8) << "Theta"
           << std::setw(8) << "uTheta"
           << std::setw(8) << "Phi"
           << std::setw(8) << "uPhi"
           << std::setw(8) << "Yield"
           << std::setw(8) << "uYield"
           << "\n"
           << std::setfill('-')
           << " " << std::setw(7) << '-'
           << " " << std::setw(7) << '-'
           << " " << std::setw(7) << '-'
           << " " << std::setw(7) << '-'
           << " " << std::setw(7) << '-'
           << " " << std::setw(7) << '-'
           << std::setfill(' ')
           << std::endl;

    for (UInt_t i=first_det_index; i<first_det_index+ndets; i++)
    {
        PrintResultsForExpData(stream, i);
    }

    stream.close();
}

Double_t
AngleAverager::ComputePhiMean(TH1D *h)
{
    Bool_t isStraddling = PhiIsStraddlingEdge(h);

    TAxis *ax = h->GetXaxis();
    UInt_t begin = ax->GetFirst();
    UInt_t end   = ax->GetLast()+1;

    Double_t angle;
    Double_t sum=0;
    Double_t n=0;
    for (UInt_t i=begin; i<end; i++)
    {
        angle = ax->GetBinCenter(i);

        if (isStraddling && angle<0)
        {
            angle += 360.0;
        }
        sum += angle*h->GetBinContent(i);
        n += h->GetBinContent(i);
    }
    return sum/n;
}

Double_t
AngleAverager::ComputePhiStdDev(TH1D *h)
{
    using TMath::Power;
    using TMath::Sqrt;

    Bool_t isStraddling = PhiIsStraddlingEdge(h);

    TAxis *ax = h->GetXaxis();
    UInt_t begin = ax->GetFirst();
    UInt_t end   = ax->GetLast()+1;

    Double_t angle;
    Double_t sum_sqdiff=0;
    Double_t n=0;

    //    if (isStraddling)
    //        std::cout << h->GetName() << std::endl;

    Double_t mean = ComputePhiMean(h);
    for (UInt_t i=begin; i<end; i++)
    {
        angle = ax->GetBinCenter(i);
        if (isStraddling && angle<0)
        {
            angle += 360.0;
        }
        sum_sqdiff += Power(angle-mean,2.0)*h->GetBinContent(i);
        n += h->GetBinContent(i);
    }
    if (n>1)
        return Sqrt(sum_sqdiff/(n-1));
    else
        return -1.0;
}

Bool_t
AngleAverager::PhiIsStraddlingEdge(TH1D *h)
{
    TAxis *ax = h->GetXaxis();
    UInt_t begin = ax->GetFirst();
    UInt_t end   = ax->GetLast()+1;

    UInt_t peak_low_side = 0;
    UInt_t peak_high_side = 0;
    UInt_t ntrailingbins=10;
    Double_t c_prev, c_curr;

    UInt_t i=begin+1+ntrailingbins;
    c_prev = h->Integral(i-1-ntrailingbins,i-1);
    do
    {
        c_curr = h->Integral(i-ntrailingbins,i);

        if (c_curr==0 && c_prev!=0)
        {
            peak_high_side = i-ntrailingbins;
        }

        c_prev = c_curr;
        i++;
    } while (i<end);

    i=end-2-ntrailingbins;
    c_prev = h->Integral(i+1, i+1+ntrailingbins);
    do
    {
        c_curr = h->Integral(i,i+ntrailingbins);
        if (c_curr==0 && c_prev!=0)
        {
            peak_low_side = i+ntrailingbins;
        }

        c_prev = c_curr;
        i--;
    } while (i>0);

    return (peak_low_side > peak_high_side);
}

void
AngleAverager::PrintThetaResults(std::ostream& stream)
{
    stream << "Mean Theta per det (deg)";
    stream << "\n" << std::setfill('-') << std::setw(9*8) << "-";
    stream << std::setfill(' ');
    for (UInt_t i=0; i<fThetaHists.size(); i++)
    {
        if (i%fNStrips==0)
        {
            stream << std::endl;
            stream << "det" << i/fNStrips << std::endl;
            stream << std::setw(8) << " ";
        }
        else if (i%fNStrips==8)
        {
            stream << std::endl;
            stream << std::setw(8) << " ";
        }

        stream << std::setw(8) << std::setprecision(3)
//<< std::right << fThetaHists.at(i)->GetMean();
               <<  fThetaHists.at(i)->GetMean();
    }
    stream << std::endl;
    stream << std::endl;

    stream << "RMS Theta per det (deg)";
    stream << "\n" << std::setfill('-') << std::setw(9*8) << "-";
    stream << std::setfill(' ');
    for (UInt_t i=0; i<fThetaHists.size(); i++)
    {
        if (i%fNStrips==0)
        {
            stream << std::endl;
            stream << "det" << i/fNStrips << std::endl;
            stream << std::setw(8) << " ";
        }
        else if (i%fNStrips==8)
        {
            stream << std::endl;
            stream << std::setw(8) << " ";
        }

        stream << std::setw(8) << std::setprecision(3)
               << std::right << fThetaHists.at(i)->GetRMS();
    }
    stream << std::endl;
    stream << std::endl;
}

void
AngleAverager::PrintPhiResults(std::ostream& stream)
{
    stream << "Mean Phi per det (deg)";
    stream << "\n" << std::setfill('-') << std::setw(9*8) << "-";
    stream << std::setfill(' ');
    for (UInt_t i=0; i<fPhiHists.size(); i++)
    {
        if (i%fNStrips==0)
        {
            stream << std::endl;
            stream << "det" << i/fNStrips << std::endl;
            stream << std::setw(8) << " ";
        }
        else if (i%fNStrips==8)
        {
            stream << std::endl;
            stream << std::setw(8) << " ";
        }

        stream << std::setw(8) << std::setprecision(3)
               << std::right << ComputePhiMean(fPhiHists.at(i));
    }
    stream << std::endl;
    stream << std::endl;

    stream << "RMS Phi per det (deg)";
    stream << "\n" << std::setfill('-') << std::setw(9*8) << "-";
    stream << std::setfill(' ');
    for (UInt_t i=0; i<fPhiHists.size(); i++)
    {
        if (i%fNStrips==0)
        {
            stream << std::endl;
            stream << "det" << i/fNStrips << std::endl;
            stream << std::setw(8) << " ";
        }
        else if (i%fNStrips==8)
        {
            stream << std::endl;
            stream << std::setw(8) << " ";
        }

        stream << std::setw(8) << std::setprecision(3)
               << std::right << ComputePhiStdDev(fPhiHists.at(i));
    }
    stream << std::endl;
    stream << std::endl;
}


TGraph2DErrors* AngleAverager::GenerateTGraph2DErrors()
{

    Double_t theta, phi, utheta, uphi, yield, uyield;

    TGraph2DErrors* gr = new TGraph2DErrors(fThetaHists.size());
    for (UInt_t i=0; i<fThetaHists.size(); i++)
    {
        theta = fThetaHists.at(i)->GetMean();
        utheta = fThetaHists.at(i)->GetRMS();
        phi = ComputePhiMean(fPhiHists.at(i));
        uphi = ComputePhiStdDev(fPhiHists.at(i));
        yield = fThetaHists.at(i)->Integral();
        uyield = TMath::Sqrt(fThetaHists.at(i)->Integral());
        gr->SetPoint(i,theta, phi, yield/fTotalEvents);
        gr->SetPointError(i, utheta, uphi, uyield/fTotalEvents);
    }
    gr->SetDirectory(0);
    return gr;
}
