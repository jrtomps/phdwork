{

gStyle->SetCanvasColor(kWhite);
gStyle->SetCanvasBorderMode(0);
gStyle->SetFrameBorderMode(0);
gStyle->SetFrameFillColor(kWhite);

string analysis_dir("home/jrtomps/pffrag2/analysis/analysis_soft/");

//gSystem->AddIncludePath("-I/home/jeromy/PhD/PhDPaper/analysis_soft/Scripts");
gSystem->AddIncludePath("-I/home/jeromy/include");

gStyle->SetOptStat("neiou");
std::cout << "Loading libdata_structure.so" << std::endl;
gSystem->Load("/home/jeromy/lib/libdata_structure.so");
std::cout << "Loading quickinspect.C" << std::endl;
gROOT->ProcessLine(".L quickinspect.C");
std::cout << "Loading CFD_lib.C" << std::endl;
gROOT->ProcessLine(".L CFD_lib.C");
std::cout << "Loading libAnalyzer.so" << std::endl;
gSystem->Load("/home/jeromy/lib/libAnalyzer.so");

std::cout << "Loading libFit.so" << std::endl;
gSystem->Load("/home/jeromy/lib/libFit.so");
std::cout << "Loading libFragSim232Th.so" << std::endl;
gSystem->Load("/home/jeromy/lib/libFragSim232Th.so");
std::cout << "Loading libElectronicsAnalyzer.so" << std::endl;
//gSystem->Load("analysis_soft/lib/libElectronicsAnalyzer.so");
std::cout << "Loading analyze.cpp" << std::endl;
gROOT->ProcessLine(".L analyze.cpp");
//std::cout << "Loading PAnalysis.h" << std::endl;
//gROOT->ProcessLine(".L analysis_soft/PAnalysis.h");i
std::cout << "Loading /home/jeromy/Code/lib/libROOTUtils.so" << std::endl;
gSystem->Load("/home/jeromy/lib/libROOTUtils.so");


//gSystem->AddLinkedLibs("-L/home/jeromy/Code/lib -lROOTUtils -lFragSimAnalyzer-232Th");
//gSystem->AddLinkedLibs("-L/home/jeromy/pffrag2/analysis/analysis_soft/lib -lAnalyzer -lFit -lHistAligner");

std::cout << "rootlogon.C tasks completed" << std::endl;



}
