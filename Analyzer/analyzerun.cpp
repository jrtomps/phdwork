// analyze.cpp
//
// AUthor : Jeromy Tompkins
// Date   : 12/2/2010
// Descrip: Load and analyze data with ProofPlot

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include "TMath.h"
#include "TROOT.h"
#include "TTree.h"
#include "TH3F.h"
#include "TH1F.h"
#include "TString.h"
#include "TF1.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "PFVisualizer.h"
#include "Visualizer.h"
#include "AdjPP.h"
#include "ProofPlot.h"
#include "TFile.h"
using namespace std;
using namespace TMath;

static const Int_t ndet = 96;
//static const Int_t ndet = 32;
ConfigManager cm1;
PFVisualizer *v0;
Visualizer gVis;

static const Char_t *configfile   = "/home/jrtomps/pffrag2/analysis/configdatabase.dat";
static const Char_t *hist_run_dir = "/home/jrtomps/pffrag2/analysis/pffrag2_hist_run";
static const Char_t *base         = "pffrag2_hist_run";
static const Char_t *run_tree_dir = "/home/jrtomps/pffrag2/analysis/run_tree_dir";
TString strbase(base);


void
RunCoda2Root(const Int_t runnumber)
{
  TString release_my = getenv("RELEASE_MY");
  if (release_my.Length() == 0)
    system("source ../tunl_soft.csh");
  
  system(Form("coda2root %i",runnumber));
}

int
main()
{
  Int_t runnumber; 
  Bool_t force_reanalyze=false; 
  Bool_t check_to_add=true;
  
  cout << "Enter run : ";
  cin  >> runnumber;

  //  cm1 = ConfigManager(ndet);
  cm1 = ConfigManager();
  cm1.LoadDbFromFile(configfile);
  cm1.PrintRun(runnumber);
  ConfigEntry *ce = cm1.GetRun(runnumber);
  if (ce == 0)
    {
      if (!check_to_add) return 1;
      Char_t response;
      std::cout << "Do you want to add the run to the database? (y/n)" << std::endl;
      std::cin >> response;
      if (response != 'y' && response != 'Y') 
	return 0;
      else 
	{
	  cm1.AddNewEntry(runnumber);
	}
      ce = cm1.GetRun(runnumber);
    }
  
  ProofPlot *pp = new ProofPlot(ce);
  //      AdjPP *pp = new AdjPP(ce);
  
  TString runfile = TString::Format("%s/run%i_tree.root", run_tree_dir,runnumber);

  if (force_reanalyze)
     RunCoda2Root(runnumber);
  else
     system(Form("make RUN=%i",runnumber));

  TFile f(runfile.Data());
  if (!f.IsOpen())
    {
      Char_t resp;
      std::cout << "\nThis data has not yet been processed by coda2root"
                   "\n\nDo you want to run it now? (y/n)"
                << std::endl;
      std::cin  >> resp;
      
      if (resp == 'y' || resp == 'Y')
         RunCoda2Root(runnumber); 
    } 
   f.Close();

  TFile f1(runfile.Data());


  std::cout << "Analyzing run " << runnumber << std::endl;
  TTree *nt = (TTree*) f1.Get("flat_tree");
  
  nt->Process(pp);
  delete pp;
  f1.Close();

  return 0;
}

