#include "TH1.h"
#include "TMath.h"
#include "TDirectory.h"

void GenerateSystematicUncertainty(TH1* hprod, TH1* hsim, TDirectory* odir)
{

    Double_t bcp, ubcp, bcs, ubcs;

    Int_t nb = TMath::Min(hprod->GetNbinsX(), hsim->GetNbinsX());
    TH1* hsys = new TH1D("systematic_unc",";Strip # ;Diff", nb,-0.5,nb-0.5);
    Double_t diff, ave;
    for (Int_t i=1;i<=nb;i++)
    {
        bcp = hprod->GetBinContent(i);
        ubcp = hprod->GetBinError(i);

        bcs = hsim->GetBinContent(i);
        ubcs = hsim->GetBinError(i);

        if (bcp>0 && bcs>0)
        {
            diff = TMath::Abs(bcp-bcs);
            ave = 0.5*(bcp+bcs);
            hsys->SetBinContent(i,diff/ave);
        }
    }

    TDirectory* d = gDirectory;
    odir->cd();
    hsys->Write("",TObject::kOverwrite);



}

