{
	TH2* hbg = new TH2I("hbg",";#theta (deg);Ratio",180, 0, 180, 100, 0, 2.0);
	hbg->SetStats(0);
	hbg->Draw(); 

	TGraphErrors* gr0 = static_cast<TGraphErrors*>(gDirectory->Get("ratio_xn5mm_y0_z0"));
	gr0->Draw("pe5");

	TGraphErrors* gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_xn3mm_y0_z0"));
	gr->Draw("pe5");

	TLegend * leg = new TLegend(0.2, 0.7, 0.4, 1.0);
	leg->SetHeader("x offset");
	
	leg->AddEntry(gr0, "-5 mm","LE");
	leg->AddEntry(gr, "-3 mm","LE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x0_y0_z0"));
	gr->Draw("pe5");
	leg->AddEntry(gr, " 0 mm","LE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x3mm_y0_z0"));
	gr->Draw("pe5");
	leg->AddEntry(gr, " 3 mm","LE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x5mm_y0_z0"));
	gr->Draw("pe5");
	leg->AddEntry(gr, " 5 mm","LE");

	gr = static_cast<TGraphErrors*>(gDirectory->Get("ratio_x10mm_y0_z0"));
	gr->Draw("pe5");
	leg->AddEntry(gr, "10 mm","LE");


	leg->Draw();
}
