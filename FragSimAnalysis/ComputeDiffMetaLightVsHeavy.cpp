#include <string>
#include "TFile.h"
#include "TH2.h"
#include "Eta2C.h"
#include "AngMarginalEta2C.h"

TH2* ComputeDiffMetaLightVsHeavy()
{
        /////////////////////////////////////////
        /// light 
        std::string file_name = "eta2_data_frag_z33_a86_fxd2_4M.root";
        TFile* f = new TFile(file_name.data());
        Eta2C eta(f);

        AngMarginalEta2C meta_light(eta, 33, 86);
        TH2* hmeta_light = dynamic_cast<TH2*>(meta_light.GetHist()->Clone("hmeta_light"));
        hmeta_light->SetDirectory(0);
        f->Close();

        /////////////////////////////////////////
        /// Heavy 
        file_name = "eta2_data_frag_z55_a143_fxd2_4M.root";
        f = new TFile(file_name.data());
        Eta2C eta2(f);
        AngMarginalEta2C meta_heavy(eta2, 55,143);

        TH2* hmeta_heavy = dynamic_cast<TH2*>(meta_heavy.GetHist()->Clone("hmeta_heavy"));
        hmeta_heavy->SetDirectory(0);
        f->Close();

        hmeta_light->Rebin2D(2,2);
        hmeta_heavy->Rebin2D(2,2);

        hmeta_light->Smooth(1);
        hmeta_light->Smooth(1);
        hmeta_heavy->Smooth(1);
        hmeta_heavy->Smooth(1);

        TH2* hdiv = dynamic_cast<TH2*>(hmeta_light->Clone("hdiv"));
        hdiv->SetTitle("AngMarginalEta Light (33,86) / Heavy (55,143);Cos(#theta);#phi (rad)");
        hdiv->Divide(hmeta_heavy);

        hdiv->Draw("surf1 z");
        return hdiv;
}
