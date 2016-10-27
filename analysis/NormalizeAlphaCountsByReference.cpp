

#include <ConfigManager.h>
#include <ConfigEntry.h>
#include <TH1.h>
#include <vector>
#include <ROOTUtils.h>
#include <string>
#include <stdexcept>
#include <TFile.h>

using namespace std;
FNameManager fnm("/home/jeromy/PhD/PhDPaper/analysis/fnameconfig.dat");
string expbase = fnm.GetHistFileDir();

void normalizeDirectory(TFile& file, const std::vector<double>& integrals, const std::string& dirName);

void FormAlphaLTFCorrection(const char* target, Double_t eg, const char* polarity, Int_t run)
{

  ConfigManager cm;
  cm.LoadDbFromFile(configfile);

  ConfigEntry* pCEntry = cm.GetRun(run);
  if (pCEntry==nullptr) {
    cerr << "Run doesn't exist in the config database" << endl;
    return;
  }

  // open the bgnd hists
  TString fname = expbase + TString::Format("/pffrag2_hist_run/pffrag2_hist_run%d.root",run);
  TFile f(fname.Data(),"UPDATE");
  if (!f.IsOpen()) {
    std::cout << "Could not open file " << fname.Data() << std::endl;
    return;
  }

  TDirectory* pDirectory = f.GetDirectory("adc_gt_thresh_tofcut");
  if (pDirectory == nullptr) {
    std::cout << "Could not locate adc_gt_thresh_tofcut in file" << std::endl;
    return;
  }

  vector<double> integrals(pCEntry->GetNADCChannels());
  vector<TH1*>   hists = ROOTUtils::GetAllTH1InDirectory(pDirectory);

  for (size_t i=0;i<min(size_t(pCEntry->GetNADCChannels()),hists.size()); ++i) {
    integrals[i] = hists[i]->Integral();
  }
   
  double normalization = integrals[0];
  vector<double> normalizations;

  for (auto& value : integrals) {
    value = value/normalization;
    normalizations.push_back(value);
  }

  TFile fresults(fnm.GetResultsDir(true)+TString::Format("/pffrag2_%s_%f_%s.root",target,eg,polarity),"RECREATE");
  
  normalizeDirectory(fresults, normalizations, "adc");
  normalizeDirectory(fresults, normalizations, "adc_gt_thresh");
  normalizeDirectory(fresults, normalizations, "adc_gt_thresh_tofcut");

  // 
  f.Close();
}


// Normalize the histograms in a directory
//
//    h0_normed = h0 * (alpha_sim0 / alpha_data0)
//
void normalizeDirectory(TFile& file, const std::vector<double>& integrals, const std::string& dirName) 
{
  TDirectory* pOldDir = gDirectory;
  TDirectory* pDirectory = file.GetDirectory(dirName.c_str());
  if (pDirectory == nullptr) {
    string msg("Could not locate ");
    msg += dirName + " in file";
    throw std::runtime_error(msg);
  }

  vector<TH1*> hists = ROOTUtils::GetAllTH1InDirectory(pDirectory);

  if (integrals.size() != hists.size()) {
    throw std::runtime_error("The # of integrals and hists differ.");
  }

  TDirectory* pNewDir = file.mkdir(TString::Format("%s_normed",dirName.c_str()));
  pNewDir->cd();

  for (size_t i=0; i<hists.size(); ++i) {
    hists[i]->Scale(integrals[i]);
    auto pClone = hists[i]->Clone(TString::Format("%s_normed%d",dirName.c_str(),i).Data());
    pClone->Write();
  }

  pOldDir->cd();
}



/// normalizatioin procedure
//
//
// 1. Create normalization file from alpha simulation yields and overnight run
//
//      norm = alpha_sim / alpha_data
//
// 2. For each file we are interested in:
//    a. load normalizations 
//    b. open a directory with hists
//    c. create new _normed directory,
//    d. scale all hists in directroy with normalization factor
//    e. write all normed hists to the _normed directory
