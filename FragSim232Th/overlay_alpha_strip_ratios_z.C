{
	TH2* hbg = new TH2I("hbg",";#theta (deg);Ratio",180, 0, 180, 100, 0, 2.0);
	hbg->SetStats(0);
	hbg->Draw(); 

	TGraphErrors* gr0 = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x0_y0_zn5mm"));
	gr0->Draw("pe5");

	TGraphErrors* gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x0_y0_zn3mm"));
	gr->Draw("pe5");

	TLegend * leg = new TLegend(0.2, 0.7, 0.4, 1.0);
	leg->SetHeader("z offset");
	
	leg->AddEntry(gr0, "-5 mm","FLE");
	leg->AddEntry(gr, "-3 mm","FLE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x0_y0_z0"));
	gr->Draw("pe5");
	leg->AddEntry(gr, " 0 mm","FLE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x0_y0_z3mm"));
	gr->Draw("pe5");
	leg->AddEntry(gr, " 3 mm","FLE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x0_y0_z5mm"));
	gr->Draw("pe5");
	leg->AddEntry(gr, " 5 mm","FLE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x0_y0_z10mm"));
	gr->Draw("pe5");
	leg->AddEntry(gr, "10 mm","FLE");


	leg->Draw();
}
