
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
Analyze428to430()
{

  gROOT->ProcessLine(".L analysis_soft/Scripts/ShiftHistsFromFile.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/SetErrorsForAllHists.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/NormalizeAllHists.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/SubtractBgnd.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/UnnormalizeAllHists.C+");
    
  cout << "BEGIN ANALYSIS" << endl;
  cout << "begin processing runs 428 through 430" << endl;
  if (OkToContinue("combine runs 428-430")) 
    {
      PAnalysis::CombineRuns("pffrag1_hist_run/pffrag1_hist_run428to430.root");
    }

  TFile *f, *fbg;
  if (OkToContinue("align hist run 428to430 to 447")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run428to430.root","UPDATE");
      ShiftHistsFromFile(f, "shifts_run428to447.dat",
			 "pffrag1_hist_run/pffrag1_hist_run428to430_shifted_compto447.root"); 
      f->Close();
    }
  
  if (OkToContinue("set errors on shifted run 428to430")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run428to430_shifted_compto447.root","UPDATE");
      SetErrorsForAllHists(f);
      f->Close();
    }

  if (OkToContinue("normalize shifted run 428to430 by Veto10HzClock")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run428to430_shifted_compto447.root","UPDATE");
      NormalizeAllHists(f,1/8137.3); 
      f->Close();
    }

  cout << "begin processing runs 447 and 451 bgnd runs" << endl;
  if (OkToContinue("combine runs 447 and 451")) 
    {
      PAnalysis::CombineRuns("pffrag1_hist_run/pffrag1_hist_run447_451.root");
    }
  
  if (OkToContinue("set errors on run 447_451")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run447_451.root","UPDATE");
      SetErrorsForAllHists(f);
      f->Close();
    }

  if (OkToContinue("normalize run 447_451 by Veto10HzClock")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run447_451.root","UPDATE");
      NormalizeAllHists(f,1/66285.2); 
      f->Close();
    }
  
    cout << "Subtraction..." << endl;
    if (OkToContinue("subtract run 447_451 from run 428to430")) 
    {
      f = new TFile("pffrag1_hist_run/normed_hist_run428to430_shifted_compto447.root","UPDATE");
      fbg = new TFile("pffrag1_hist_run/normed_hist_run447_451.root","UPDATE");
      SubtractBgndRuns(fbg,f,"pffrag1_hist_run/bgsubbed_normed_run447_451_from_run428to430.root");
      f->Close();
      fbg->Close();
    }

    if (OkToContinue("unnormalize run bgsubbed 428to430")) 
    {
      f = new TFile("pffrag1_hist_run/bgsubbed_normed_run447_451_from_run428to430.root");
      UnnormalizeAllHists(f,8137.3);
      f->Close();
    }

    if (OkToContinue("integrate all histograms")) 
    {
      PAnalysis::IntegrateSiStrips("pffrag1_hist_run/bgsubbed_run447_451_from_run428to430.root",
				   "intbounds.2.dat",
				   "bgsubbed_adc");				   
    }

  cout << "ANALYSIS COMPLETE" << endl;
}
