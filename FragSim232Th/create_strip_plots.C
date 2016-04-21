

void create_strip_plots(const std::string& name, const std::string& title, Color_t color)
{
	Sim_Tree->Draw("SiStripID+16*DetID >> h(64,-0.5,63.5)","SSDTotDepE>0");
	TH1* hc = (TH1D*) h->Clone(name.data());
	hc->SetTitle(title.data());
	hc->SetDirectory(0);
	hc->SetLineColor(color);

	TFile *f = new TFile("analysis/alpha_strip_int.root", "update");
	hc->Write("",TObject::kOverwrite);
	f->Close();
}
