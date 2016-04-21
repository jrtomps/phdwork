{
   std::cout << "local rootlogon.C";
 
  Int_t ret =	gSystem->Load("/home/jrtomps/Code/lib/libFragSimAnalyzer-232Th.so");
  if (ret!=0) std::cout << "libFragSimAnalyzer-232Th.so loaded returned : " << ret << std::endl;
	
  TString codedir("/home/jrtomps/Code");
	TString incdir = codedir + "/include";
	incdir.Prepend("-I");
	TString libdir = codedir + "/lib -lROOTUtils";
	libdir.Prepend("-L");
	incdir.Append(" -I/home/jrtomps/g4work/FragSim232Th/include");

	libdir.Append(" -lFragSimAnalyzer-232Th");

	gSystem->AddIncludePath(incdir.Data());
	
  gSystem->AddLinkedLibs(libdir.Data());
	gSystem->Load("/usr/local/geant4/geant4.9.5.p01/lib/Linux-g++/libG4global.so");

	gSystem->Load("/home/jrtomps/Code/lib/libROOTUtils.so");
	gSystem->Load("/home/jrtomps/Code/lib/libNuclearChart.so");
//    gSystem->Load("/home/jrtomps/pffrag2/analysis/analysis_soft/lib/libFit.so");
   std::cout << "... done" << std::endl;
}
