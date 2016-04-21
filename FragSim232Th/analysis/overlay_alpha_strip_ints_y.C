

{
	TH1* gr0 = static_cast<TH1*>(gDirectory->Get("x0_yn5mm_z0"));
	gr0->SetStats(0);
	gr0->Draw("e");

	TH1* gr = static_cast<TH1*>(gDirectory->Get("x0_yn3mm_z0"));
	gr->SetStats(0);
	gr->Draw("e same");

	TLegend * leg = new TLegend(0.2, 0.7, 0.4, 1.0);
	leg->SetHeader("y offset");
	
	leg->AddEntry(gr0, "-5 mm","LE");
	leg->AddEntry(gr, "-3 mm","LE");

	gr = static_cast<TH1*>(gDirectory->Get("x0_y0_z0"));
	gr->SetStats(0);
	gr->Draw("e same");
	leg->AddEntry(gr, " 0 mm","LE");

	gr = static_cast<TH1*>(gDirectory->Get("x0_y3mm_z0"));
	gr->SetStats(0);
	gr->Draw("e same");
	leg->AddEntry(gr, " 3 mm","LE");

	gr = static_cast<TH1*>(gDirectory->Get("x0_y5mm_z0"));
	gr->SetStats(0);
	gr->Draw("e same");
	leg->AddEntry(gr, " 5 mm","LE");

	gr = static_cast<TH1*>(gDirectory->Get("x0_y10mm_z0"));
	gr->SetStats(0);
	gr->Draw("e same");
	leg->AddEntry(gr, "10 mm","LE");

	gr0->SetAxisRange(0, 1.1*gr->GetMaximum(),"Y");
	leg->Draw();
}
