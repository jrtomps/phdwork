


{

 TCanvas* c = new TCanvas("c","Yield Integrals Different Thresholds",1280,800);

 TH1* h ;
 gDirectory->GetObject("angdist_thresh9.3;1",h);
 h->SetMarkerStyle(20);
 h->SetStats(0);
 h->Draw("LP");
 h->GetXaxis()->SetLabelSize(0.06);
 h->GetXaxis()->SetTitleSize(0.06);

 h->GetYaxis()->SetLabelSize(0.06);
 h->GetYaxis()->SetTitleSize(0.06);
 h->GetYaxis()->SetTitleOffset(0.80);
 
TLegend * leg = new TLegend(0.1, 0.4,0.4,0.8,"Thresh");
leg->AddEntry(h,"9.3 MeV", "LP");

 gDirectory->GetObject("angdist_thresh14.3;1",h);
 h->SetLineColor(2);
 h->SetMarkerColor(2);
 h->SetMarkerStyle(21);
 h->SetStats(0);
 h->Draw("same LP");
leg->AddEntry(h,"14.3 MeV", "LP");

// gDirectory->GetObject("angdist_thresh19.3;1",h);
// h->SetLineColor(3);
// h->Draw("same LP");
//leg->AddEntry(h,"19.3 MeV", "LP");
//
 gDirectory->GetObject("angdist_thresh24.3;1",h);
 h->SetLineColor(4);
 h->SetMarkerColor(4);
 h->SetMarkerStyle(22);
 h->SetStats(0);
 h->Draw("same LP");
leg->AddEntry(h,"24.3 MeV", "LP");

// gDirectory->GetObject("angdist_thresh29.3;1",h);
// h->SetLineColor(5);
// h->Draw("same LP");
//leg->AddEntry(h,"29.3 MeV", "LP");

 gDirectory->GetObject("angdist_thresh34.3;1",h);
 h->SetLineColor(6);
 h->SetStats(0);
 h->SetMarkerColor(6);
 h->SetMarkerStyle(23);
 h->Draw("same LP");
leg->AddEntry(h,"34.3 MeV", "LP");

// gDirectory->GetObject("angdist_thresh39.3;1",h);
// h->SetLineColor(7);
// h->Draw("same LP");
//leg->AddEntry(h,"39.3 MeV", "LP");

 gDirectory->GetObject("angdist_thresh44.3;1",h);
 h->SetLineColor(8);
 h->SetMarkerColor(8);
 h->SetStats(0);
 h->SetMarkerStyle(24);
 h->Draw("same LP");
leg->AddEntry(h,"44.3 MeV", "LP");

// gDirectory->GetObject("angdist_thresh49.3;1",h);
// h->SetLineColor(9);
// h->Draw("same LP");
//leg->AddEntry(h,"49.3 MeV", "LP");
//
// gDirectory->GetObject("angdist_thresh54.3;1",h);
// h->SetLineColor(1);
// h->SetMarkerColor(2);
// h->SetMarkerStyle(21);
// h->SetLineStyle(2);
// h->Draw("same LP");
//leg->AddEntry(h,"54.3 MeV", "LP");
  leg->Draw();
}



