
#include <ROOTUtils.h>
#include <algorithm>

void SubtractBgndRuns(TDirectory* bgndDir, TDirectory* prodDir)
{

  vector<TH1*> bgndHists = ROOTUtils::GetAllTH1InDirectory(bgndDir);
  vector<TH1*> prodHists = ROOTUtils::GetAllTH1InDirectory(prodDir);

  if (bgndHists.size() != prodHists.size()) {
    throw std::runtime_error("SubtractBgndHists(TDirectory*, TDirectory*) found different number of hists in each directory");
  }

  TDirectory* pMotherDir = prodDir->GetMotherDir();
  if (pMotherDir == nullptr) {
    pMotherDir = prodDir;
  }

  TString dirName = TString::Format("bgsubbed_%s",prodDir->GetName());
  pMotherDir->mkdir(dirName);

  pMotherDir->cd();

  for (size_t i=0; i<bgndHists.size(); ++i) {
      TString name = TString::Format("bgsubbed_%s", prodHists.at(i)->GetName());

      TH1* pClone = dynamic_cast<TH1*>(prodHists.at(i)->Clone(name.Data()));

      pClone->Add(bgndHists.at(i), -1);
      pClone->Write();
  }
  
}
