
void DrawRateEstimatesBig()
{
        TFile* f = new TFile("frag_yield_hist2.root","update");
        f->cd(); 

        TCanvas* mycan = new TCanvas("mycan",
                              "Comparison of Rate Estimates To Data",
                              1280,800);
        TH1* hdata;
        gDirectory->GetObject("circ_7p6_data",hdata);

        hdata->SetTitle("Data (bgsubbed, no LTF corr)");
        hdata->Draw();

        hdata->SetAxisRange(1e-5,800,"y");
        hdata->SetAxisRange(0,64,"x");

        TLegend *leg = new TLegend(0.1, 0.5, 0.4, 0.7);
        leg->AddEntry(hdata,hdata->GetTitle(),"LPE");

        ////////////////

        TH1* h0;
        gDirectory->GetObject("circ_7p6_est_big_gx2",h0);

        h0->SetTitle("Estimate [glbl shift (+2,0,0) mm]");
        leg->AddEntry(h0,h0->GetTitle(),"LPE");
        h0->Draw("same");

        ///////////////
        TH1* h1;
        gDirectory->GetObject("circ_7p6_est_big_gxn2",h1);
        // this assumes that the rootaliases.C script has run
        SetColor(h1,kBlue);
        h1->SetTitle("Estimate [glbl shift (-2,0,0) mm]");
        leg->AddEntry(h1,h1->GetTitle(),"LPE");
        h1->Draw("same");

        ///////////////
        TH1* h2;
        gDirectory->GetObject("circ_7p6_est_big_gy2",h2);
        // this assumes that the rootaliases.C script has run
        SetColor(h2,kGreen+1);

        h2->SetTitle("Estimate [glbl shift (0,+2,0) mm]");
        leg->AddEntry(h2,h2->GetTitle(),"LPE");
        h2->Draw("same");

        ///////////////
        TH1* h3;
        gDirectory->GetObject("circ_7p6_est_big_gyn2",h3);
        // this assumes that the rootaliases.C script has run
        SetColor(h3,kOrange+1);

        h3->SetTitle("Estimate [glbl shift (0,-2,0) mm]");
        leg->AddEntry(h3,h3->GetTitle(),"LPE");
        h3->Draw("same");

        ///////////////
        TH1* h4;
        gDirectory->GetObject("circ_7p6_est_big_gx2_gy2",h4);
        // this assumes that the rootaliases.C script has run
        SetColor(h4,kRed+1);

        h4->SetTitle("Estimate [glbl shift (+2,+2,0) mm]");
        leg->AddEntry(h4,h4->GetTitle(),"LPE");
        h4->Draw("same");

        ///////////////
        TH1* h5;
        gDirectory->GetObject("circ_7p6_est_big_gxn2_gyn2",h5);
        // this assumes that the rootaliases.C script has run
        SetColor(h5,kViolet+1);

        h5->SetTitle("Estimate [glbl shift (-2,-2,0) mm]");
        leg->AddEntry(h5,h5->GetTitle(),"LPE");
        h5->Draw("same");

        ///////////////
        TH1* h6;
        gDirectory->GetObject("circ_7p6_est_big_gx2_gyn2",h6);
        // this assumes that the rootaliases.C script has run
        SetColor(h6,kAzure+1);

        h6->SetTitle("Estimate [glbl shift (+2,-2,0) mm]");
        leg->AddEntry(h6,h6->GetTitle(),"LPE");
        h6->Draw("same");

        ///////////////
        TH1* h7;
        gDirectory->GetObject("circ_7p6_est_big_gxn2_gy2",h7);
        // this assumes that the rootaliases.C script has run
        SetColor(h7,kPink+1);

        h7->SetTitle("Estimate [glbl shift (-2,+2,0) mm]");
        leg->AddEntry(h7,h7->GetTitle(),"LPE");
        h7->Draw("same");

        ///////////////
        TH1* h8;
        gDirectory->GetObject("circ_7p6_est_big_gx4_gy2",h8);
        // this assumes that the rootaliases.C script has run
        SetColor(h8,kBlue-2);

        h8->SetTitle("Estimate [glbl shift (+4,+2,0) mm]");
        leg->AddEntry(h8,h8->GetTitle(),"LPE");
        h8->Draw("same");

        leg->Draw();

        mycan->Write("big_yield_comp",TObject::kOverwrite);


}
