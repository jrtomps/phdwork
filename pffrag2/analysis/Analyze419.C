
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
Analyze419()
{

  gROOT->ProcessLine(".L analysis_soft/Scripts/ShiftHistsFromFile.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/SetErrorsForAllHists.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/NormalizeAllHists.C+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/SubtractBgnd.cpp+");
  gROOT->ProcessLine(".L analysis_soft/Scripts/UnnormalizeAllHists.C+");
 
  cout << "BEGIN ANALYSIS" << endl;
  cout << "begin processing runs 419" << endl;

  TFile *f, *fbg;
  if (OkToContinue("align hist run 419 to 420")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run419.root","UPDATE");
      ShiftHistsFromFile(f, "shifts_run419.dat",
			 "pffrag1_hist_run/pffrag1_hist_run419_shifted_compto420.root"); 
      f->Close();
    }
  
  if (OkToContinue("set errors on shifted run 419")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run419_shifted_compto420.root","UPDATE");
      SetErrorsForAllHists(f);
      f->Close();
    }

  if (OkToContinue("normalize shifted run 419 by Veto10HzClock")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run419_shifted_compto420.root","UPDATE");
      NormalizeAllHists(f,1/13218.1); 
      f->Close();
    }

  if (OkToContinue("set errors on run 420")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run420.root","UPDATE");
      SetErrorsForAllHists(f);
      f->Close();
    }

  if (OkToContinue("normalize run 420 by Veto10HzClock")) 
    {
      f = new TFile("pffrag1_hist_run/pffrag1_hist_run420.root","UPDATE");
      NormalizeAllHists(f,1/34313.2); 
      f->Close();
    }
  
    cout << "Subtraction..." << endl;
    if (OkToContinue("subtract run 420 from run 419")) 
    {
      f = new TFile("pffrag1_hist_run/normed_hist_run419_shifted_compto420.root","UPDATE");
      fbg = new TFile("pffrag1_hist_run/normed_hist_run420.root","UPDATE");
      SubtractBgndRuns(fbg,f,"pffrag1_hist_run/bgsubbed_normed_run420_from_run419.root");
      f->Close();
      fbg->Close();
    }

    if (OkToContinue("unnormalize run bgsubbed 419")) 
    {
      f = new TFile("pffrag1_hist_run/bgsubbed_normed_run420_from_run419.root");
      UnnormalizeAllHists(f,13218.1);
      f->Close();
    }

    if (OkToContinue("integrate all histograms")) 
    {
      PAnalysis::IntegrateSiStrips("pffrag1_hist_run/bgsubbed_run420_from_run419.root",
				   "intbounds.2.dat",
				   "bgsubbed_adc");				   
    }

  cout << "ANALYSIS COMPLETE" << endl;
}
