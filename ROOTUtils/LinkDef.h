#ifdef __CLING__

#pragma link off all globals;
#pragma link off all class;
#pragma link off all functions;

#pragma link C++ class STDatime;
#pragma link C++ class HistFiller;

#pragma link C++ namespace ROOTUtils;
#pragma link C++ nestedclass;
#pragma link C++ nestedfunction;
#pragma link C++ nestedtypedef;

#pragma link C++ function ROOTUtils::ToTH1F(TH1*);
#pragma link C++ function ROOTUtils::ReplaceWithTH1F(TH1*);
#pragma link C++ function ROOTUtils::ToTH1D(TH1*);
#pragma link C++ function ROOTUtils::ReplaceWithTH1D(TH1*);
#pragma link C++ function ROOTUtils::ToTH2F(TH2*);
#pragma link C++ function ROOTUtils::ReplaceWithTH2F(TH2*);
#pragma link C++ function ROOTUtils::ToTH2D(TH2*);
#pragma link C++ function ROOTUtils::ReplaceWithTH2D(TH2*);
#pragma link C++ function ROOTUtils::GetAllFromDirectory<TGraph>(TDirectory*);
#pragma link C++ function ROOTUtils::GetAllFromDirectory<TH1>(TDirectory*);
#pragma link C++ function ROOTUtils::GetAllFromDirectory<TH2>(TDirectory*);
#pragma link C++ function ROOTUtils::GetAllFromDirectory<TGraph>(TDirectory*,const TString&);
#pragma link C++ function ROOTUtils::GetAllFromDirectory<TH1>(TDirectory*,const TString&);
#pragma link C++ function ROOTUtils::GetAllFromDirectory<TH2>(TDirectory*,const TString&);
#pragma link C++ function ROOTUtils::GetAllTGraphsInFile(TFile*);
#pragma link C++ function ROOTUtils::GetAllTH1InFile(TFile*);
#pragma link C++ function ROOTUtils::GetAllTH2InFile(TFile*);
#pragma link C++ function ROOTUtils::GetAllTGraphsInFile(TFile*, const TString&);
#pragma link C++ function ROOTUtils::GetAllTH1InFile(TFile*, const TString&);
#pragma link C++ function ROOTUtils::GetAllTH2InFile(TFile*, const TString&);
#pragma link C++ function ROOTUtils::GetAllTGraphsInDirectory(TDirectory*);
#pragma link C++ function ROOTUtils::GetAllTH1InDirectory(TDirectory*);
#pragma link C++ function ROOTUtils::GetAllTH2InDirectory(TDirectory*);
#pragma link C++ function ROOTUtils::GetAllTGraphsInDirectory(TDirectory*, const TString&);
#pragma link C++ function ROOTUtils::GetAllTH1InDirectory(TDirectory*, const TString&);
#pragma link C++ function ROOTUtils::GetAllTH2InDirectory(TDirectory*, const TString&);
#pragma link C++ function ROOTUtils::EnforceProperOrdering(std::vector<TH1*>&);
#pragma link C++ function ROOTUtils::EnforceProperOrdering(std::vector<TH2*>&);
#pragma link C++ function ROOTUtils::FormatTPad(TVirtualPad*);
#pragma link C++ function ROOTUtils::CalibrateHistWithConsistentAxis(TH1*, UInt_t, Double_t, Double_t);
#pragma link C++ function ROOTUtils::GetAllTDirectoriesInDirectory(TDirectory*);
#pragma link C++ class VectorSorter;
#pragma link C++ class VectorWriter;
#pragma link C++ class std::vector<Double_t>;
#pragma link C++ class std::vector<Int_t>;

#pragma link C++ class DataBucket;

#pragma link C++ class ROOTUtils::TreeMaker;
#pragma link C++ class ROOTUtils::SingleTreeMaker;
#pragma link C++ class ROOTUtils::MultiTreeMaker;
#pragma link C++ class ROOTUtils::ConsistentAxisConverter;
#pragma link C++ class ROOTUtils::TH2GaussianSmear;
#pragma link C++ class ROOTUtils::TH2CosPhiSmear;
#pragma link C++ class ColumnFileLoader;

#pragma link C++ global gROOT;
#pragma link C++ global gEnv;

#endif
