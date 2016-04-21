{
	gSystem->AddIncludePath("-I/home/jrtomps/pffrag2/analysis/analysis_soft/include ");
	gSystem->AddIncludePath("-I/home/jrtomps/Code/include ");
	gSystem->AddLinkedLibs("-L/home/jrtomps/Code/lib -lROOTUtils");

	cout << "Loading libAnalyzer.so" << endl;
	gSystem->Load("../lib/libAnalyzer.so");
	cout << "Loading libHistAligner.so" << endl;
	gSystem->Load("../lib/libHistAligner.so");

	cout << "completed local roologon.C tasks" << endl;
}
