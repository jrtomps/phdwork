
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
Analyze448to450()
{

  gROOT->ProcessLine(".L analysis_soft/Scripts/ShiftHistsFromFile.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/SetErrorsForAllHists.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/NormalizeAllHists.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/SubtractBgnd.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/UnnormalizeAllHists.C+");
    
  cout << "BEGIN ANALYSIS" << endl;
  cout << "begin processing runs 448 through 450" << endl;
  if (OkToContinue("combine runs 448to450")) 
    {
      PAnalysis::CombineRuns("pffrag1_hist_run/pffrag1_hist_run448to450.root");
    }

  TFile *f, *fbg;
  if (OkToContinue("align hist run 448to450 to 447_451")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run448to450.root","UPDATE");
      ShiftHistsFromFile(f, "shifts_run449to447_451.dat",
			 "pffrag1_hist_run/pffrag1_hist_run448to450_shifted_compto447_451.root"); 
      f->Close();
    }
  
  if (OkToContinue("set errors on shifted run 448to450")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run448to450_shifted_compto447_451.root","UPDATE");
      SetErrorsForAllHists(f);
      f->Close();
    }

  if (OkToContinue("normalize shifted run 448to450 by Veto10HzClock")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run448to450_shifted_compto447_451.root","UPDATE");
      NormalizeAllHists(f,1/43985.7); 
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
    if (OkToContinue("subtract run 447_451 from run 448to450")) 
    {
      f = new TFile("pffrag1_hist_run/normed_hist_run448to450_shifted_compto447_451.root","UPDATE");
      fbg = new TFile("pffrag1_hist_run/normed_hist_run447_451.root","UPDATE");
      SubtractBgndRuns(fbg,f,"pffrag1_hist_run/bgsubbed_normed_run447_451_from_run448to450.root");
      f->Close();
      fbg->Close();
    }

    if (OkToContinue("unnormalize run bgsubbed 448to450")) 
    {
      f = new TFile("pffrag1_hist_run/bgsubbed_normed_run447_451_from_run448to450.root");
      UnnormalizeAllHists(f,43985.7);
      f->Close();
    }

    if (OkToContinue("integrate all histograms")) 
    {
      PAnalysis::IntegrateSiStrips("pffrag1_hist_run/bgsubbed_run447_451_from_run448to450.root",
				   "intbounds.2.dat",
				   "bgsubbed_adc");				   
    }
  cout << "ANALYSIS COMPLETE" << endl;
}
