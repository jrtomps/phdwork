{
	TH1* h = fStripInts;
	StripDb sdb("angle_data/angles");

	gROOT->ProcessLine(".L CreateRatioFromAlphaInts.cpp+");
	
	TGraphErrors* gr = CreateRatioFromStripHist(h,sdb);

	_file0->ReOpen("update");


}
