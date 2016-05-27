
Bool_t
OkToContinue(const Char_t* text)
{
  Char_t ans;
  cout << "Next step : " << text << endl;
  cout << "Continue? (y/n) : ";
  cin  >> ans;
  return (ans == 'y' || ans == 'Y');
}

void
Analyze6p7_Lin(void)
{

//  gROOT->ProcessLine(".L analysis_soft/Scripts/ShiftHistsFromFile.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/SetErrorsForAllHists.C+");
//  gROOT->ProcessLine(".L analysis_soft/Scripts/NormalizeAllHists.C+");
//  gROOT->ProcessLine(".L analysis_soft/Scripts/SubtractBgnd.cpp+");
//  gROOT->ProcessLine(".L analysis_soft/Scripts/UnnormalizeAllHists.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateTGraphInputFiles.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/CreatePlotOverlay.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/ApplyCircularCorrections.cpp+");
  
  cout << "BEGIN ANALYSIS" << endl;
  cout << "begin processing runs" << endl;
  if (OkToContinue("combine runs 238U_232Th, Eg=6.7 MeV, Lin")) 
    {
      CombineAllRunsForTargetWithEnergyAndPol("238U_232Th",6.7,"Lin");
    }

  TFile *f, *fbg;
  if (OkToContinue("Set errors on pffrag2_hist_run238U_232Th_6.7_Lin.root")) 
    {
      f = new TFile("pffrag2_hist_run238U_232Th_6.7_Lin.root","UPDATE");
      SetErrorsForAllHists(f);
      f->Close();
    }

    if (OkToContinue("integrate all histograms")) 
    {
      PAnalysis::IntegrateSiStrips("pffrag2_hist_run238U_232Th_6.7_Lin.root",
				   "intbounds238U_232Th_6.7_Lin.dat",
				   "adc");				   
    }
    
    if (OkToContinue("generate TGraph input files")) 
    {
      GenerateTGraphInputFiles("IntOutput/238U_232Th_6.7_Lin",
                               "angle_data/theta.dat",
                               "ang_dists/angdists238U_232Th_6.7_Lin.dat");				   
    }
    
    if (OkToContinue("apply circular corrections"))
      { 
	fcorr = new TFile("238U_232Th_7.6_Circ_corrections.root");
	f = new TFile("ang_dists/angdists238U_232Th_6.7_Lin.root","UPDATE");
	ApplyCircularCorrections(f, fcorr);
        f->Close();
	fcorr->Close();
      }
    
    if (OkToContinue("create overlaid angular distributions"))
      { 
	
	f = new TFile("ang_dists/angdists238U_232Th_6.7_Lin.root","UPDATE");
	CreatePlotOverlay(f);
	//	f->Close();
      }
    
    
   cout << "ANALYSIS COMPLETE" << endl;
}
