#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include <string>


TGraphErrors* GetGraph(const std::string& fname, const std::string graph_name="alpha_ratio")
{
    TGraphErrors* gr = 0;
    TFile *f = TFile::Open(fname.data());
    if (f->IsOpen()==false) 
    {
      std::cout << "Failed to open file " << fname << std::endl;
      return gr;
    }
   
    f->GetObject(graph_name.data(),gr);

    return gr;
}


void SetErrorXToZero(TGraphErrors* gr)
{
    Double_t *ex = gr->GetEX();

    for (Int_t i=0; i<gr->GetN(); ++i)
    {
        ex[i] = 0;    
    }

}

TGraphErrors* GetAlphaRatio(const std::string& fname)
{
  return GetGraph(fname, "alpha_ratio");
}

TGraphErrors* GetData(const std::string& fname)
{
  return GetGraph(fname, "ratio_asys");
}

void OverlayBestRatioBig()
{

  TGraphErrors* gr_data = GetData("results_narrow/ang_dists/angdists_238U_232Th_7.6_Circ_sanormed_bgsubbed_ind.root");
  if (gr_data==0)
  {
          std::cout << "couldn't find data : ratio_asys" << std::endl;
          return;
  }
  gr_data->SetEditable(0);

  TGraphErrors* gr1 = GetAlphaRatio("/home/jrtomps/g4work/FragSim232Th/analysis/frag_2_gx2_gy2_gz0_r3_r5_x1_y0_z0_fxd2_4M_analyzed.root");
  if (gr1==0)
  {
          std::cout << "couldn't find data : alpha_ratio (+2,+2,0)" << std::endl;
          return;
  }
  SetErrorXToZero(gr1);
  gr1->Sort();
  gr1->SetEditable(0);

  TGraphErrors* gr2 = GetAlphaRatio("/home/jrtomps/g4work/FragSim232Th/analysis/frag_2_gx0_gyn2_gz0_r3_r5_x1_y0_z0_fxd2_4M_analyzed.root");
  if (gr2==0)
  {
          std::cout << "couldn't find data : alpha_ratio (0,-2,0)" << std::endl;
          return;
  }
  SetErrorXToZero(gr2);
  gr2->Sort();
  gr2->SetEditable(0);

  TGraphErrors* gr3 = GetAlphaRatio("/home/jrtomps/g4work/FragSim232Th/analysis/frag_2_gx2_gyn2_gz0_r3_r5_x1_y0_z0_fxd2_4M_analyzed.root");
  if (gr3==0)
  {
          std::cout << "couldn't find data : alpha_ratio (+2,-2,0)" << std::endl;
          return;
  }
  SetErrorXToZero(gr3);
  gr3->Sort();
  gr3->SetEditable(0);

  gr_data->Draw("APE");
  gr1->Draw("PE");
  gr2->Draw("PE");
  gr3->Draw("PE");

  TLegend* leg = new TLegend(0.1,0.7,0.4,0.9);
  leg->AddEntry(gr_data,"7.6 MeV Circ AAN","LPE");
  leg->AddEntry(gr1,"Glbl shift (+2,+2,0) mm","LPE");
  leg->AddEntry(gr2,"Glbl shift (0,-2,0) mm","LPE");
  leg->AddEntry(gr3,"Glbl shift (+2,-2,0) mm","LPE");
  leg->Draw();

}
