
Bool_t OkToContinue(const TString& text)
{
    Char_t ans;
    cout << "Next step : " << text << endl;
    cout << "Continue? (y/n/q) : ";
    cin  >> ans;
	if (ans=='q') exit(0);
    return (ans == 'y' || ans == 'Y');
}

Double_t eg=7.6;
TString pol="Lin";
TString target = "238U_232Th";
TString correction_id = "238U_232Th_7.6_Lin";
TString bgnd_id = "238U_232Th_bgnd0_0.0_--";
TString bgnd_tar("238U_232Th_bgnd0");
Double_t bgnd_en = 0.0;
TString bgnd_pol="--";

void Analyze7p6_Lin(void)
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
    TString shift_data = "shifts_7.6_Lin_to_bgnd0.dat";

    TString overnight_bgnd_run = (results_dir + "/" + fnm.GetHistFileBase(false).data() + bgnd_id + ".root");
   
    gROOT->ProcessLine(".L analysis_soft/Scripts/SetErrorsForAllHists.C+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateTGraphInputFiles.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/CreatePlotOverlay.C+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/ApplyCircularCorrections.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/CleanupCombinedCanvasesDirectory.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/CreateOverlayCanvases.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/SolidAngleCorrectionRoutines.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateRatioAndAsymmetry.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateAngularDistributions.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateTGraph2D.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/ShiftHistsFromFile.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/SubtractBgnd.cpp+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/NormalizeAllHists.C+");

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
    if (OkToContinue((TString("Set errors on ") + combined_hist_run).Data()))
    {
        f = new TFile(combined_hist_run,"UPDATE");
        SetErrorsForAllHists(f);
        f->Close();
    }

    TString message = TString::Format("normalize %s, Eg=%.1f, %s data by Veto10HzClock",
                                      target.Data(), eg, pol.Data());
     if (OkToContinue(message))
     {
         f = new TFile(combined_hist_run.Data(),"UPDATE");

         std::cout << "\tNormalizing hists in adc directory" << std::endl;
         TDirectory *dir = f->GetDirectory("adc");
         if (dir==0)  std::cout << "cannot find adc histograms" << std::endl;
         else         NormalizeAllHists(dir,1.0);//;1/8656.4);

         std::cout << "\tNormalizing hists in adc_gt_thresh directory" << std::endl;
         dir = f->GetDirectory("adc_gt_thresh");
         if (dir==0)  std::cout << "cannot find adc_gt_thresh histograms" << std::endl;
         else         NormalizeAllHists(dir,1.0);//1/8656.4);

         f->Close();
     }

     message = TString::Format("combine %s, Eg=%.1f, %s", bgnd_tar.Data() , bgnd_en, bgnd_pol.Data());
     if (OkToContinue(message))
     {
         CombineAllRunsForTargetWithEnergyAndPol(bgnd_tar.Data(),bgnd_en,bgnd_pol.Data(),true);
         std::cout << overnight_bgnd_run << std::endl;
         f = new TFile(overnight_bgnd_run.Data(),"UPDATE");
         CleanupCombinedCanvasesDirectory(f);
         f->Close();
         f = TFile::Open(overnight_bgnd_run.Data(),"UPDATE");
         CreateOverlayCanvases(f);
         f->Close();
     }

     message = TString::Format("normalize %s data by Veto10HzClock",
                               bgnd_id.Data());
     if (OkToContinue(message))
     {
         f = new TFile(overnight_bgnd_run.Data(),"UPDATE");

         std::cout << "\tNormalizing hists in adc directory" << std::endl;
         TDirectory *dir = f->GetDirectory("adc");
         if (dir==0)  std::cout << "cannot find adc histograms" << std::endl;
         else         NormalizeAllHists(dir,8656.4/19781.6);

         std::cout << "\tNormalizing hists in adc_gt_thresh directory" << std::endl;
         dir = f->GetDirectory("adc_gt_thresh");
         if (dir==0)  std::cout << "cannot find adc_gt_thresh histograms" << std::endl;
         else         NormalizeAllHists(dir,8656.4/19781.6);

         f->Close();
     }

     message = TString::Format("align %s, Eg=%.1f, %s hists to %s",
                                 target.Data(), eg, pol.Data(), bgnd_id.Data());
       if (OkToContinue(message))
       {
           f = new TFile(combined_hist_run.Data(),"UPDATE");

           std::cout << "\tShifting adc hists" << std::endl;
           TDirectory *dir = f->GetDirectory("normed_adc");
           if (dir==0)  std::cout << "cannot find adc directory" << std::endl;
           else         ShiftHistsFromFile(dir, shift_data.Data());

           std::cout << "\tShifting adc_gt_thresh hists" << std::endl;
           dir = f->GetDirectory("normed_adc_gt_thresh");
           if (dir==0)  std::cout << "cannot find adc_gt_thresh directory" << std::endl;
           else         ShiftHistsFromFile(dir, shift_data.Data());

           f->Close();
       }


     cout << "Subtraction..." << endl;
     message = TString::Format("subtract %s from %s, Eg=%.1f, %s data",
                               bgnd_id.Data(), target.Data(), eg, pol.Data());
     if (OkToContinue(message))
     {
         f = new TFile(combined_hist_run.Data(),"UPDATE");

         std::cout << "\tSubtracting adc hists" << std::endl;
         TDirectory *dir = f->GetDirectory("shifted_normed_adc");
         if (dir==0)
         {
             std::cout << "cannot find shifted adc histograms" << std::endl;
         }
         else
         {
             fbg = new TFile(overnight_bgnd_run.Data(),"UPDATE");
             TDirectory *dirbg = fbg->GetDirectory("normed_adc");
             if (dirbg==0)     std::cout << "cannot find normed histograms in " << fbg->GetName() << std::endl;
             else     	      SubtractBgndRuns(dirbg,dir,"","subtraction_blacklist_7.6_Lin.dat");
             fbg->Close();
         }

         std::cout << "\tSubtracting adc_gt_thresh hists" << std::endl;
         dir = f->GetDirectory("shifted_normed_adc_gt_thresh");
         if (dir==0)
         {
             std::cout << "cannot find shifted adc histograms" << std::endl;
         }
         else
         {
             fbg = new TFile(overnight_bgnd_run.Data(),"UPDATE");
             TDirectory *dirbg = fbg->GetDirectory("normed_adc_gt_thresh");
             if (dirbg==0)    std::cout << "cannot find normed histograms in " << fbg->GetName() << std::endl;
             else             SubtractBgndRuns(dirbg,dir,"", "subtraction_blacklist_7.6_Lin.dat");
             fbg->Close();
         }


         f->Close();

     }

     if (OkToContinue("integrate all \"adc_gt_thresh\" histograms"))
     {
//         PAnalysis::IntegrateSiStrips(combined_hist_run.Data(),
//                                      TString("adc_gt_thresh"),
//                                     TString(""));
         PAnalysis::IntegrateSiStrips(combined_hist_run.Data(),
                                      TString("normed_bgsubbed_adc_gt_thresh"),
                                     TString(""),
                                     "",
                                    "bgsubbed_shifted_normed_adc_gt_thresh");

    }

    if (OkToContinue("generate angular distribution"))
    {
        GenerateTGraphInputFiles(int_file.Data(),
                                 angle_file.Data(),
                                 angdist_text_file.Data(),
                                 "blacklisted_ch_7.6_Lin.dat");
    }

//    if (OkToContinue("apply circular corrections"))
//    {
//        fcorr = new TFile(circ_corr_file);
//        f = new TFile(angdist_root_file,"UPDATE");
//        ApplyCircularCorrections(f, fcorr);
//        f->Close();
//        fcorr->Close();
//    }
//
//    if (OkToContinue("apply solid angle corrections"))
//    {
//        f = new TFile(angdist_root_file,"UPDATE");
//        ApplySolidAngleCorrections(f, sa_corr_file.Data());
//        f->Close();
//    }
//
//    if (OkToContinue("write uncorrected and corrected results to file"))
//    {
//        PrintUncorrectedAndCorrectedResultsToFile(angdist_text_file.Data(),
//                                                  sa_corr_file.Data(),
//                                                  angdist_corr_text_file.Data());
//
//        PrintSortedCorrectedAndUncorrectedResultsToFile(angdist_corr_text_file.Data(),
//                                                        angdist_corr_text_file_sorted.Data(),
//                                                        angle_file.Data());
//    }

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
