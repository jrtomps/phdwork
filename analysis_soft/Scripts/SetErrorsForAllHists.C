#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "PAnalysis.h"
#include "TList.h"
#include "TObject.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1.h"
#include "TDirectoryFile.h"
#include "TDirectory.h"
#include "TPRegexp.h"
#include "TMath.h"
#include "ROOTUtils.h"

using namespace std;
using namespace PAnalysis;

void SetError(TH1* h)
{
    Int_t bin;
    for (Int_t i=0; i<h->GetNbinsX()+1; i++)
    {
        for (Int_t j=0; j<h->GetNbinsY()+1; j++)
        {
            for (Int_t k=0; k<h->GetNbinsZ()+1; k++)
            {
                bin = h->GetBin(i,j,k);
                h->SetBinError(bin,TMath::Sqrt(h->GetBinContent(bin)));
            }
        }
    }
}

void SetErrorsForAllHistInDirectory(TDirectory* dir)
{
    vector<TH1*> hvec = ROOTUtils::GetAllTH1InDirectory(dir);
    // this doesn't require that the order is correct.
    vector<TH1*>::iterator it;
    std::clog << " : " << std::setw(4) << hvec.size() << " hists";

    dir->cd();
    for (it=hvec.begin(); it!=hvec.end(); it++)
    {
        SetError(*it);

        (*it)->Write("",TObject::kOverwrite);
    }

}

void SetErrorsForAllHistsInFile(TFile *f)
{
    TListIter next(f->GetListOfKeys());
    TKey *key;
    std::vector<TDirectory*> dirs;
    TString tdirectory("TDirectoryFile");

    while(( key = static_cast<TKey*>(next()) ))
    {
        if ( tdirectory.EqualTo(key->GetClassName()) )
        {
            dirs.push_back( static_cast<TDirectory*>(key->ReadObj()) );
        }
    }

    vector<TDirectory*>::iterator dit;
    for (dit=dirs.begin(); dit!=dirs.end(); dit++)
    {
        std::clog << "Setting errors for histograms in "
                << std::setw(16) << (*dit)->GetName();
        SetErrorsForAllHistInDirectory((*dit));
        std::clog << std::endl;
    }

}

void SetErrorsForAllHists(TFile* f)
{
    SetErrorsForAllHistsInFile(f);
}
