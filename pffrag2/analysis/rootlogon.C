{

gStyle->SetCanvasColor(kWhite);
gStyle->SetCanvasBorderMode(0);
gStyle->SetFrameBorderMode(0);
gStyle->SetFrameFillColor(kWhite);

string analysis_dir("home/jrtomps/pffrag2/analysis/analysis_soft/");

gStyle->SetOptStat("neiou");
std::cout << "Loading libdata_structure.so" << std::endl;
gSystem->Load("/home/jeromy/tunl_soft/slib/Linux-i686/libdata_structure.so");
std::cout << "Loading quickinspect.C" << std::endl;
gROOT->ProcessLine(".L quickinspect.C");
std::cout << "Loading CFD_lib.C" << std::endl;
gROOT->ProcessLine(".L CFD_lib.C");
std::cout << "Loading libAnalyzer.so" << std::endl;
gSystem->Load("analysis_soft/lib/libAnalyzer.so");

std::cout << "Loading libFit.so" << std::endl;
gSystem->Load("analysis_soft/lib/libFit.so");
std::cout << "Loading libFragSim232Th.so" << std::endl;
gSystem->Load("/home/jeromy/g4work/tmp/Linux-g++/FragSim232Th/libFragSim232Th.so");
std::cout << "Loading libElectronicsAnalyzer.so" << std::endl;
gSystem->Load("analysis_soft/lib/libElectronicsAnalyzer.so");
std::cout << "Loading analyze.cpp" << std::endl;
gROOT->ProcessLine(".L analysis_soft/analyze.cpp");
//std::cout << "Loading PAnalysis.h" << std::endl;
//gROOT->ProcessLine(".L analysis_soft/PAnalysis.h");i
std::cout << "Loading /home/jeromy/Code/lib/libROOTUtils.so" << std::endl;
gSystem->Load("/home/jeromy/Code/lib/libROOTUtils.so");


gSystem->AddIncludePath("-I/home/jeromy/pffrag2/analysis/analysis_soft/include");
gSystem->AddIncludePath("-I/home/jeromy/pffrag2/analysis/analysis_soft/Scripts");
gSystem->AddIncludePath("-I/home/jeromy/Code/include");
gSystem->AddLinkedLibs("-L/home/jeromy/Code/lib -lROOTUtils -lFragSimAnalyzer-232Th");
gSystem->AddLinkedLibs("-L/home/jeromy/pffrag2/analysis/analysis_soft/lib -lAnalyzer -lFit -lHistAligner");

std::cout << "rootlogon.C tasks completed" << std::endl;



}
