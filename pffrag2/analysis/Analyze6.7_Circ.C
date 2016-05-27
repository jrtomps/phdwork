
Bool_t OkToContinue(const TString& text)
{
    Char_t ans;
    cout << "Next step : " << text << endl;
    cout << "Continue? (y/n/q) : ";
    cin  >> ans;
	if (ans=='q') exit(0);
    return (ans == 'y' || ans == 'Y');
}

Double_t eg=6.7;
TString pol="Circ";
TString target = "238U_232Th";
TString correction_id = "238U_232Th_6.7_Circ";

void Analyze6p7_Circ(void)
{
    FNameManager fnm("fnameconfig.dat");
    TString results_dir = fnm.GetResultsDir(true);
    TString target_id  = TString::Format("%s_%.1f_%s",target.Data(), eg, pol.Data());
    TString combined_hist_run = (results_dir + "/");
    combined_hist_run += (fnm.GetHistFileBase(false) + target_id.Data() + ".root");
    TString int_file = (fnm.GetResultsDir() + "/IntOutput/" + target_id.Data());
    TString angle_file = "angle_data/angles";
    TString sa_corr_file = "angle_data/sa_corrections";
    TString circ_corr_file = TString::Format("corrections%s.root",correction_id.Data());
    TString angdist_dir = (results_dir + "/ang_dists");
    TString angdist_text_file = (angdist_dir + "/angdists_" + target_id + ".dat");
    TString angdist_corr_text_file = (angdist_dir + "/angdists_" + target_id + "_corrected.dat");
    TString angdist_corr_text_file_sorted = (angdist_dir + "/angdists_" + target_id + "_corrected_sorted.dat");
    TString angdist_root_file = (angdist_dir + "/angdists_" + target_id + ".root");


   
    gROOT->ProcessLine(".L analysis_soft/Scripts/SetErrorsForAllHists.C++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateTGraphInputFiles.cpp++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/CreatePlotOverlay.C++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/ApplyCircularCorrections.cpp++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/CleanupCombinedCanvasesDirectory.cpp++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/CreateOverlayCanvases.cpp++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/SolidAngleCorrectionRoutines.cpp++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateRatioAndAsymmetry.cpp++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateAngularDistributions.cpp++");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateTGraph2D.cpp++");

    cout << "BEGIN ANALYSIS" << endl;
    cout << "begin processing runs" << endl;

    TString message = TString::Format("combine %s, Eg=%.1f, %s", target.Data(), eg, pol.Data());
    if (OkToContinue(message))
    {
        CombineAllRunsForTargetWithEnergyAndPol(target.Data(),eg,pol.Data(),true);
        f = new TFile(combined_hist_run,"UPDATE");
        CleanupCombinedCanvasesDirectory(f);
        f->Close();
        f = TFile::Open(combined_hist_run,"UPDATE");
        CreateOverlayCanvases(f);
        f->Close();
    }

    TFile *f, *fcorr;
    if (OkToContinue((TString("Set errors on ") + combined_hist_run)))
    {
        f = new TFile(combined_hist_run,"UPDATE");
        SetErrorsForAllHists(f);
        f->Close();
    }

    if (OkToContinue("integrate all \"adc_gt_thresh\" histograms"))
    {
        PAnalysis::IntegrateSiStrips(combined_hist_run,
                                     TString("adc_gt_thresh"),
                                     TString(""));
    }

    if (OkToContinue("generate angular distribution"))
    {
        GenerateTGraphInputFiles(int_file.Data(),
                                 angle_file.Data(),
                                 angdist_text_file.Data(),
                                 "blacklisted_ch_6.7_Circ.dat");
    }

    if (OkToContinue("apply circular corrections"))
    {
        fcorr = new TFile(circ_corr_file);
        f = new TFile(angdist_root_file,"UPDATE");
        ApplyCircularCorrections(f, fcorr);
        f->Close();
        fcorr->Close();
    }

    if (OkToContinue("apply solid angle corrections"))
    {
        f = new TFile(angdist_root_file,"UPDATE");
        ApplySolidAngleCorrections(f, sa_corr_file.Data());
        f->Close();
    }

    if (OkToContinue("write uncorrected and corrected results to file"))
    {
        PrintUncorrectedAndCorrectedResultsToFile(angdist_text_file.Data(),
                                                  sa_corr_file.Data(),
                                                  angdist_corr_text_file.Data());

        PrintSortedCorrectedAndUncorrectedResultsToFile(angdist_corr_text_file.Data(),
                                                        angdist_corr_text_file_sorted.Data(),
                                                        angle_file.Data());
    }

    if (OkToContinue("generate ratio and asymmetry plots"))
    {
        f = new TFile(angdist_root_file,"UPDATE");
        GenerateRatioAndAsymmetry(f);
        f->Close();
    }

    if (OkToContinue("create overlaid angular distributions"))
    {
        gROOT->SetBatch(true);
        f = new TFile(angdist_root_file,"UPDATE");
        CreatePlotOverlay(f);
        //	f->Close();
        gROOT->SetBatch(false);
    }

    if (OkToContinue("generate 2D angular dist"))
    {
        GenerateTGraph2D(angdist_corr_text_file.Data(), angdist_root_file.Data());
        f = new TFile(angdist_root_file.Data(),"UPDATE");
//        GenerateTH2D(f);
        f->Close();

    }

    cout << "ANALYSIS COMPLETE" << endl;
}
