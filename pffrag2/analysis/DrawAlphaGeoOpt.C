

{
	Int_t data_col = kRed+1;;
	Int_t sim_col = kAzure+1;
	gr_dataints_par->Draw("APE");
	gr_dataints_perp->Draw("PE");

	SetColor(gr_dataints_par,data_col);
	SetColor(gr_dataints_perp,data_col);

	gr_dataints_par->SetMarkerSize(1.2);
	gr_dataints_perp->SetMarkerSize(1.2);

        Double_t m = TMath::MaxElement(gr_dataints_par->GetN(),gr_dataints_par->GetY());
        Double_t n = TMath::MaxElement(gr_stripints_par->GetN(),gr_stripints_par->GetY());
	Double_t v = m/n;

//	TGraphErrors* grpar, *grperp;
//        gDirectory->GetObject("gr_stripints_par",grpar);
//        gDirectory->GetObject("gr_stripints_perp",grperp);

        std::cout << "scale_factor = " << v << std::endl;
	ScaleGraph(gr_stripints_par, v);
	ScaleGraph(gr_stripints_perp, v);
	
	SetColor(gr_stripints_par,sim_col);
	SetColor(gr_stripints_perp,sim_col);

	gr_stripints_par->SetMarkerStyle(22);
	gr_stripints_perp->SetMarkerStyle(26);
	gr_stripints_par->SetMarkerSize(1.2);
	gr_stripints_perp->SetMarkerSize(1.2);
//	gr_stripints_par->SetMarkerColor(kRed-7);
//	gr_stripints_perp->SetMarkerColor(kRed-7);

//	gr_stripints_par->SetLineColor(kRed-7);
//	gr_stripints_perp->SetLineColor(kRed-7);

	gr_stripints_par->Draw("PE");
	gr_stripints_perp->Draw("PE");

	TLegend* leg = new TLegend(0.50, 0.67, 0.88, 0.88);
	leg->SetNColumns(2);
	leg->AddEntry(gr_dataints_par,"Data - Para","LPE");
	leg->AddEntry(gr_dataints_perp,"Data - Perp", "LPE");
	leg->AddEntry(gr_stripints_par,"Sim - Para","LPE");
	leg->AddEntry(gr_stripints_perp,"Sim - Perp", "LPE");

	leg->Draw();

	gPad->SetLeftMargin(0.10);
	gPad->SetRightMargin(0.10);
	gPad->SetBottomMargin(0.12);	
	gPad->SetTopMargin(0.08);
}
