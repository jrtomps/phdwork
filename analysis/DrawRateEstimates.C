
void DrawRateEstimates()
{
        TCanvas* mycan = new TCanvas("mycan","Comparison of Rate Estimates To Data",1280,800);
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
	gDirectory->GetObject("circ_7p6_est_big_gxn2",h0);

	h0->SetTitle("Rate Estimate [glbl shift (-2,0,0) mm]");
	leg->AddEntry(h0,h0->GetTitle(),"LPE");
	h0->Draw("same");
	
///////////////
	TH1* h1;
	gDirectory->GetObject("circ_7p6_est_gx2_big_gy2",h1);

	h1->SetTitle("Rate Estimate [glbl shift (+2,+2,0) mm]");
	leg->AddEntry(h1,h1->GetTitle(),"LPE");
	h1->Draw("same");

///////////////
	TH1* h2;
	gDirectory->GetObject("circ_7p6_est_gy3",h2);

	h2->SetTitle("Rate Estimate [glbl shift (0,+3,0) mm]");
	leg->AddEntry(h2,h2->GetTitle(),"LPE");
	h2->Draw("same");

///////////////
	TH1* h3;
	gDirectory->GetObject("circ_7p6_est_gy4",h3);

	h3->SetTitle("Rate Estimate [glbl shift (0,+4,0) mm]");
	leg->AddEntry(h3,h3->GetTitle(),"LPE");
	h3->Draw("same");

///////////////
	TH1* h4;
	gDirectory->GetObject("circ_7p6_est_gyn2",h4);

	h4->SetTitle("Rate Estimate [glbl shift (0,-2,0) mm]");
	leg->AddEntry(h4,h4->GetTitle(),"LPE");
	h4->Draw("same");

	leg->Draw();
}
