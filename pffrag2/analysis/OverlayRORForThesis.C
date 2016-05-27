
{

	TFile* f= new TFile("eg6p2_ROR_th232.root");
	TGraphErrors* gr_th; 
	f->GetObject("theta_yields_ratio_lin_over_circ",gr_th);

	gr_th->SetLineColor(kRed);
	gr_th->SetMarkerStyle(20);
	gr_th->SetMarkerColor(kRed);
	gr_th->SetEditable(false);
	

	TFile* f2= new TFile("u238_6.2_ratios_wsys_no30.root");
	TGraphErrors* gr_u; 
	f2->GetObject("theta_yields_ratio_lin_over_circ",gr_u);
	gr_u->SetLineColor(kBlue);
	gr_u->SetMarkerStyle(24);
	gr_u->SetMarkerColor(kBlue);
	gr_u->SetEditable(false);
	

	gr_th->Draw("APE");
	gr_u->Draw("PE");
	

}
