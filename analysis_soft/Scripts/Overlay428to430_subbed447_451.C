{
  TFile * fbg = new TFile("pffrag1_hist_run/normed_hist_run447_451.root");
  TFile * fprod = new TFile("pffrag1_hist_run/normed_hist_run428to430_shifted_compto447.root");

  TString fout = "pffrag1_hist_run/bgsubbed_normed_run447_451_from_run428to430.root";

  TFile *fsub = new TFile(fout.Data());

  Visualizer vis;
  fbg->cd();
  vis.Draw("adc%i_norm_vclk");
  gSystem->Sleep(100);
  fprod->cd();
  vis.Draw("adc%i_shifted_norm_vclk","same color=2 canvas=adc%i_norm_vclk_run451");
  fsub->cd();
  vis.Draw("normed_bgsubbed_adc%i","same color=4 canvas=adc%i_norm_vclk_run451");

  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_0",100,600,"x");
  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_1",100,600,"x");
  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_2",100,600,"x");
  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_3",100,600,"x");

  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_0",-0.01,0.012,"y");
  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_1",-0.01,0.012,"y");
  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_2",-0.01,0.012,"y");
  SetAxisRangesOnAllPads("adc%i_norm_vclk_run451_3",-0.01,0.012,"y");
}
