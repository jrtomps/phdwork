
Bool_t OkToContinue(const TString& text)
{
    Char_t ans;
    cout << "Next step : " << text << endl;
    cout << "Continue? (y/n/q) : ";
    cin  >> ans;
    if (ans == 'y' || ans == 'Y')
        return true;
    else if (ans =='q')
        exit(0);
    else
        return false;
}

Double_t eg=5.9;
TString pol="Lin";
TString target = "238U_232Th";
TString bgnd_id = "238U_232Th_bgnd0";
TString shift_data = "shifts_5.9_Lin_to_bgnd0.dat";
TString calib_file = "eg5.9_Lin_calib.dat";
TString bgnd_calib_file = "overnight_bgnd0_calib.dat";
TString g4_sacorr_fname = "g4_sa_corr_232Th.dat";

Double_t norm = 15870.8/20485.5;; // (total 10Hz clock time prod. data)/(total 10Hz clock time bgnd)

void Analyze5p9_Lin_ind(bool force_recompile=true)
{
    FNameManager fnm("fnameconfig.dat");
    TString results_dir = fnm.GetResultsDir(true);
    TString target_id  = TString::Format("%s_%.1f_%s",target.Data(), eg, pol.Data());
    TString combined_hist_run = (results_dir + "/");
    combined_hist_run += (fnm.GetHistFileBase(false) + target_id.Data() + "_ind.root");

    TString int_file = (fnm.GetResultsDir() + "/IntOutput/" + target_id.Data() + "_ind");
    TString int_subbed_file = (fnm.GetResultsDir() + "/IntOutput/" + target_id.Data() + "_bgsubbed_ind");
    TString int_sanormed_file = (fnm.GetResultsDir() + "/IntOutput/" + target_id.Data() + "_sanormed_ind");
    TString int_subbed_sanormed_file = (fnm.GetResultsDir() + "/IntOutput/" + target_id.Data() + "_bgsubbed_sanormed_ind");


    TString angle_file = "angle_data/angles";
    TString sa_corr_file = "angle_data/sa_corrections";
    TString correction_id = TString::Format("%s_%.1f_%s", target.Data(), eg, pol.Data());
    TString circ_corr_file = TString::Format("corrections%s.root",correction_id.Data());

    TString angdist_dir = (results_dir + "/ang_dists");
    TString angdist_text_file = (angdist_dir + "/angdists_" + target_id + "_ind.dat");
    TString angdist_subbed_text_file = (angdist_dir + "/angdists_" + target_id + "_bgsubbed_ind.dat");

    TString angdist_sanormed_text_file = (angdist_dir + "/angdists_" + target_id + "_sanormed" + "_ind.dat");
    TString angdist_subbed_sanormed_text_file = (angdist_dir + "/angdists_" + target_id + "_sanormed_bgsubbed_ind.dat");

    TString angdist_root_file = (angdist_dir + "/angdists_" + target_id + "_ind.root");
    TString angdist_subbed_root_file = (angdist_dir + "/angdists_" + target_id + "_bgsubbed_ind.root");
    TString angdist_sanormed_root_file = (angdist_dir + "/angdists_" + target_id + "_sanormed" + "_ind.root");
    TString angdist_subbed_sanormed_root_file = (angdist_dir + "/angdists_" + target_id + "_sanormed_bgsubbed_ind.root");


    TString overnight_hist_run = (results_dir + "/" + fnm.GetHistFileBase(false).data() + bgnd_id + "_0.0_--_ind.root");


    if (force_recompile==true)
    {
        gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateTGraphInputFiles.cpp++");
        gROOT->ProcessLine(".L analysis_soft/Scripts/CreatePlotOverlay.C++");
        gROOT->ProcessLine(".L analysis_soft/Scripts/CleanupCombinedCanvasesDirectory.cpp++");
        gROOT->ProcessLine(".L analysis_soft/Scripts/CreateOverlayCanvases.cpp++");
        gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateRatioAndAsymmetry.cpp++");
        gROOT->ProcessLine(".L GenerateSubtractionUncertainties.cpp++");
        gROOT->ProcessLine(".L NormalizeByLiveTimeFractionForRuns.cpp++");
        gROOT->ProcessLine(".L NormalizeAllHists.C++");
        gROOT->ProcessLine(".L ShiftHistsFromFile.cpp++");
        gROOT->ProcessLine(".L SubtractBgnd.cpp++");
        gROOT->ProcessLine(".L UnnormalizeAllHists.C++");
        gROOT->ProcessLine(".L NormalizeAlphaCountsByReference.cpp++");
        gROOT->ProcessLine(".L CalibrateHIsts.cpp++");
        gROOT->ProcessLine(".L CreateSysCorrRatios.cpp++");
        gROOT->ProcessLine(".L CreateRatioFromAlphaInts.cpp++");
    }
    else
    {
        gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateTGraphInputFiles.cpp+");
        gROOT->ProcessLine(".L analysis_soft/Scripts/CreatePlotOverlay.C+");
        gROOT->ProcessLine(".L analysis_soft/Scripts/CleanupCombinedCanvasesDirectory.cpp+");
        gROOT->ProcessLine(".L analysis_soft/Scripts/CreateOverlayCanvases.cpp+");
        gROOT->ProcessLine(".L analysis_soft/Scripts/GenerateRatioAndAsymmetry.cpp+");
        gROOT->ProcessLine(".L GenerateSubtractionUncertainties.cpp+");
        gROOT->ProcessLine(".L NormalizeByLiveTimeFractionForRuns.cpp+");
        gROOT->ProcessLine(".L NormalizeAllHists.C+");
        gROOT->ProcessLine(".L ShiftHistsFromFile.cpp+");
        gROOT->ProcessLine(".L SubtractBgnd.cpp+");
        gROOT->ProcessLine(".L UnnormalizeAllHists.C+");
        gROOT->ProcessLine(".L NormalizeAlphaCountsByReference.cpp+");
        gROOT->ProcessLine(".L CalibrateHIsts.cpp+");
        gROOT->ProcessLine(".L CreateSysCorrRatios.cpp+");
        gROOT->ProcessLine(".L CreateRatioFromAlphaInts.cpp+");


    }
    cout << "BEGIN ANALYSIS" << endl;
    cout << "begin processing runs" << endl;

    TString message = TString::Format("Form LTF corrected histograms");
    if (OkToContinue(message))
    {
        FormAlphaLTFCorrection(target.Data(), eg, pol.Data(), 678, g4_sacorr_fname.Data(),"alpha_ints_rate");
//        FormAlphaLTFCorrection(target.Data(), eg, pol.Data(), 678, g4_sacorr_fname.Data());
    }

//    message = TString::Form("Correct individual histograms");

    TString message = TString::Format("combine %s, Eg=%.1f, %s", target.Data(), eg, pol.Data());
    if (OkToContinue(message))
    {
        CombineAllRunsForTargetWithEnergyAndPol(target.Data(),eg,pol.Data(),true,"_ind");
//        system(Form("mv %s/pffrag2_hist_run%s_%.1f_%s.root %s/pffrag2_hist_run%s_%.1f_%s_ind.root",
//                    results_dir.Data(), target.Data(), eg, pol.Data(),
//                    results_dir.Data(), target.Data(), eg, pol.Data()));
        f = new TFile(combined_hist_run,"UPDATE");
        CleanupCombinedCanvasesDirectory(f);
        f->Close();
        f = TFile::Open(combined_hist_run,"UPDATE");
        CreateOverlayCanvases(f);
        f->Close();
    }

    message = TString::Format("calibrate %s, Eg=%.1f, %s", target.Data(), eg, pol.Data());
    if (OkToContinue(message))
    {
        f = new TFile(combined_hist_run.Data(),"UPDATE");
        TDirectory* dir = f->GetDirectory("adc");
        if (dir==0)
        {
            std::cout << "\tfailed to find adc folder in " << f->GetName() << std::endl;
        }
        else
        {
            std::cout << "\tCalibrating adc with " << calib_file
                    << std::endl;
            CalibrateHists(dir, calib_file.Data());
        }
        f->Close();
        dir = 0;

        f = new TFile(combined_hist_run.Data(),"UPDATE");
        dir = f->GetDirectory("ltf_corr_adc");
        if (dir==0)
        {
            std::cout << "\tfailed to find ltf_corr_adc folder in " << f->GetName() << std::endl;
        }
        else
        {
            std::cout << "\tCalibrating ltf_corr_adc with " << calib_file
                    << std::endl;
            CalibrateHists(dir, calib_file.Data());
        }

        dir = 0;

        dir = f->GetDirectory("ltf_corr_adc_gt_thresh");
        if (dir==0)
        {
            std::cout << "\tfailed to find ltf_corr_adc_gt_thresh folder in " << f->GetName() << std::endl;
        }
        else
        {
            std::cout << "\tCalibrating ltf_corr_adc_gt_thresh with " << calib_file
                    << std::endl;
            CalibrateHists(dir, calib_file.Data());
        }

        f->Close();
    }
    //    TFile *f, *fcorr;
//    message = TString::Format("normalize %s, Eg=%.1f, %s data by 10HzClock",
//                              target.Data(), eg, pol.Data());
//    if (OkToContinue(message))
//    {
//        f = new TFile(combined_hist_run.Data(),"UPDATE");
//
//        std::cout << "\tNormalizing hists in ltf_corr_adc directory" << std::endl;
//        TDirectory *dir = f->GetDirectory("ltf_corr_adc_cut_tofcut");
//        if (dir==0)  std::cout << "cannot find ltf_corr_adc histograms" << std::endl;
//        else         NormalizeAllHists(dir,1/16042.0);
//
//        std::cout << "\tNormalizing hists in ltf_corr_adc_gt_thresh directory" << std::endl;
//        dir = f->GetDirectory("ltf_corr_adc_gt_thresh_tofcut");
//        if (dir==0)  std::cout << "cannot find ltf_corr_adc_gt_thresh histograms" << std::endl;
//        else         NormalizeAllHists(dir,1/16042.0);
//
//        f->Close();
//    }

    cout << "\n\nBegin processing runs overnight background runs\n" << endl;
     cout << "Do you want to skip the overnight bgnd subtraction? (y/n) " << endl;
     Char_t myans;
     cin >> myans;
     if (myans=='n')
     {
         message = TString::Format("Form LTF corrected histograms");
         if (OkToContinue(message))
         {
             FormAlphaLTFCorrection(bgnd_id.Data(), // target label
                                   0,               // energy
                                   "--",            // polarization
                                   678,             // reference run number
                                   g4_sacorr_fname.Data(),//"", // this should make little difference but forms the alpha_ints_rate_sanormed
                                   "alpha_ints_rate");
//             FormAlphaLTFCorrection(bgnd_id.Data(), 0, "--", 678, g4_sacorr_fname.Data());
         }

         message = TString::Format("combine runs and set errors on %s", bgnd_id.Data());
         if (OkToContinue(message))
         {
             CombineAllRunsForTargetWithEnergyAndPol(bgnd_id.Data(),0,"--",true, "_ind");

//             system(Form("mv %s/pffrag2_hist_run%s_0.0_--.root %s/pffrag2_hist_run%s_0.0_--_ind.root",
//                         results_dir.Data(), bgnd_id.Data(),
//                         results_dir.Data(), bgnd_id.Data()));

         }

         message = TString::Format("normalize %s by 10HzClock", bgnd_id.Data());
         if (OkToContinue(message))
         {
             f = new TFile(overnight_hist_run.Data(),"UPDATE");

             std::cout << "\tNormalizing hists in ltf_corr_adc directory" << std::endl;
             TDirectory *dir = f->GetDirectory("ltf_corr_adc");
             if (dir==0) std::cout << "Cannot find ltf_corr_adc folder" << std::endl;
             else        NormalizeAllHists(dir,norm);

             std::cout << "\tNormalizing hists in ltf_corr_adc_gt_thresh directory" << std::endl;
             dir = f->GetDirectory("ltf_corr_adc_gt_thresh");
             if (dir==0) std::cout << "Cannot find ltf_corr_adc_gt_thresh folder" << std::endl;
             else        NormalizeAllHists(dir,norm);

             std::cout << "\tNormalizing hists in ltf_corr_adc_gt_thresh_tofcut directory" << std::endl;
             dir = f->GetDirectory("ltf_corr_adc_gt_thresh_tofcut");
             if (dir==0) std::cout << "Cannot find ltf_corr_adc_gt_thresh_tofcut folder" << std::endl;
             else        NormalizeAllHists(dir,norm);
             f->Close();
         }

         message = TString::Format("calibrate %s", bgnd_id.Data());
         if (OkToContinue(message))
         {
             f = new TFile(overnight_hist_run,"UPDATE");
             TDirectory* dir = f->GetDirectory("adc");
             if (dir==0)
             {
                 std::cout << "\tfailed to find adc folder in " << f->GetName() << std::endl;
             }
             else
             {
                 std::cout << "\tCalibrating adc with " << bgnd_calib_file
                         << std::endl;
                 CalibrateHists(dir, bgnd_calib_file.Data());
             }
			 dir = 0;

             dir = f->GetDirectory("normed_ltf_corr_adc");
             if (dir==0)
             {
                 std::cout << "\tfailed to find normed_ltf_corr_adc folder in " << f->GetName() << std::endl;
             }
             else
             {
                 std::cout << "\tCalibrating normed_ltf_corr_adc with " << bgnd_calib_file
                         << std::endl;
                 CalibrateHists(dir, bgnd_calib_file.Data());
             }
			 dir = 0;

             dir = f->GetDirectory("normed_ltf_corr_adc_gt_thresh");
             if (dir==0)
             {
                 std::cout << "\tfailed to find normed_ltf_corr_adc_gt_thresh folder in " << f->GetName() << std::endl;
             }
             else
             {
                 std::cout << "\tCalibrating normed_ltf_corr_adc_gt_thresh with " << bgnd_calib_file
                         << std::endl;
                 CalibrateHists(dir, bgnd_calib_file.Data());
             }

//             dir = f->GetDirectory("normed_ltf_corr_adc_gt_thresh_sanormed");
//             if (dir==0)
//             {
//                 std::cout << "\tfailed to find normed_ltf_corr_adc_gt_thresh_sanormed folder in " << f->GetName() << std::endl;
//             }
//             else
//             {
//                 std::cout << "\tCalibrating normed_ltf_corr_adc_gt_thresh_sanormed with " << bgnd_calib_file
//                         << std::endl;
//                 CalibrateHists(dir, bgnd_calib_file.Data());
//             }

             f->Close();
         }

         message = TString::Format("align %s, Eg=%.1f, %s hists to %s",
                                   target.Data(), eg, pol.Data(), bgnd_id.Data());
         if (OkToContinue(message))
         {
             f = new TFile(combined_hist_run.Data(),"UPDATE");

             std::cout << "\tShifting ltf_corr_adc hists" << std::endl;
             TDirectory *dir = f->GetDirectory("ltf_corr_adc");
             if (dir==0)  std::cout << "cannot find ltf_corr_adc directory" << std::endl;
             else         ShiftHistsFromFile(dir, shift_data.Data());

             std::cout << "\tShifting ltf_corr_adc_gt_thresh hists" << std::endl;
             dir = f->GetDirectory("ltf_corr_adc_gt_thresh");
             if (dir==0)  std::cout << "cannot find ltf_corr_adc_gt_thresh directory" << std::endl;
             else         ShiftHistsFromFile(dir, shift_data.Data());

             std::cout << "\tShifting ltf_corr_adc_gt_thresh_tofcut hists" << std::endl;
             dir = f->GetDirectory("ltf_corr_adc_gt_thresh_tofcut");
             if (dir==0)  std::cout << "cannot find ltf_corr_adc_gt_thresh_tofcut directory" << std::endl;
             else         ShiftHistsFromFile(dir, shift_data.Data());
             f->Close();
         }

         cout << "Subtraction..." << endl;
         message = TString::Format("subtract %s from %s, Eg=%.1f, %s data",
                                   bgnd_id.Data(), target.Data(), eg, pol.Data());
         if (OkToContinue(message))
         {
             f = new TFile(combined_hist_run.Data(),"UPDATE");

             std::cout << "\tSubtracting shifted_ltf_corr_adc hists" << std::endl;
             TDirectory *dir = f->GetDirectory("shifted_ltf_corr_adc");
             if (dir==0)
             {
                 std::cout << "cannot find shifted shifted_ltf_corr_adc histograms" << std::endl;
             }
             else
             {
                 fbg = new TFile(overnight_hist_run.Data(),"UPDATE");
                 TDirectory *dirbg = fbg->GetDirectory("normed_ltf_corr_adc");
                 if (dirbg==0)   std::cout << "cannot find normed histograms in " << fbg->GetName() << std::endl;
                 else     	      SubtractBgndRuns(dirbg,dir);
                 fbg->Close();
             }

             std::cout << "\tSubtracting shifted_ltf_corr_adc_gt_thresh hists" << std::endl;
             dir = f->GetDirectory("shifted_ltf_corr_adc_gt_thresh");
             if (dir==0)
             {
                 std::cout << "cannot find shifted_ltf_corr_adc_gt_thresh histograms" << std::endl;
             }
             else
             {
                 fbg = new TFile(overnight_hist_run.Data(),"UPDATE");
                 TDirectory *dirbg = fbg->GetDirectory("normed_ltf_corr_adc_gt_thresh");
                 if (dirbg==0) std::cout << "cannot find normed histograms in " << fbg->GetName() << std::endl;
                 else          SubtractBgndRuns(dirbg,dir);
                 fbg->Close();
             }

             std::cout << "\tSubtracting shifted_ltf_corr_adc_gt_thresh_tofcut hists" << std::endl;
             dir = f->GetDirectory("shifted_ltf_corr_adc_gt_thresh_tofcut");
             if (dir==0)
             {
                 std::cout << "cannot find shifted_ltf_corr_adc_gt_thresh_tofcut histograms" << std::endl;
             }
             else
             {
                 fbg = new TFile(overnight_hist_run.Data(),"UPDATE");
                 TDirectory *dirbg = fbg->GetDirectory("normed_ltf_corr_adc_gt_thresh_tofcut");
                 if (dirbg==0) std::cout << "cannot find normed histograms in " << fbg->GetName() << std::endl;
                 else          SubtractBgndRuns(dirbg,dir);
                 fbg->Close();
             }


             f->Close();
         }

     }

     if (OkToContinue("compute systematic unc. from overnight bgnd histograms"))
     {
         TString uncorr_dir = "ltf_corr_adc_gt_thresh";
         TString corr_dir = "bgsubbed_shifted_ltf_corr_adc_gt_thresh";
         f = new TFile(combined_hist_run.Data(),"update");
         TDirectory* du = f->GetDirectory(uncorr_dir.Data());
         TDirectory* ds = f->GetDirectory(corr_dir.Data());

         if (du==0)      std::cout << "Cannot find " << uncorr_dir << std::endl;
         else if (ds==0) std::cout << "Cannot find " << corr_dir << std::endl;
         else
         {
             TGraph* gr = GenerateSubtractionUncertainties(du, ds);
             f->cd();
             gr->Write("", TObject::kOverwrite);
         }

         f->Close();
     }

     if (OkToContinue("Form alpha normalized spectra"))
     {
         f = new TFile(combined_hist_run.Data(),"update");

         TString uncorr_dir = "ltf_corr_adc_gt_thresh";
         TString corr_dir = "bgsubbed_shifted_ltf_corr_adc_gt_thresh";
         TDirectory* du = f->GetDirectory(uncorr_dir.Data());


         if (du==0)      std::cout << "Cannot find " << uncorr_dir << std::endl;
         else
         {
            CreateAlphaNormedHists(du, g4_sacorr_fname.Data());
         }

         TDirectory* dc = f->GetDirectory(corr_dir);

         if (dc==0)      std::cout << "Cannot find " << corr_dir << std::endl;
         else
         {
            CreateAlphaNormedHists(dc, g4_sacorr_fname.Data());
         }

         f->Close();
     }

    if (OkToContinue("Calibrate fully corrected spectra"))
    {
            f = new TFile(combined_hist_run.Data(),"UPDATE");
            TDirectory* dir = f->GetDirectory("ltf_corr_adc_gt_thresh_sanormed");
            if (dir==0)
            {
                    std::cout << "\tfailed to find ltf_corr_adc_gt_thresh_sanormed folder in " << f->GetName() << std::endl;
            }
            else
            {
                    std::cout << "\tCalibrating ltf_corr_adc_gt_thresh_sanormed with " << bgnd_calib_file
                            << std::endl;
                    CalibrateHists(dir, bgnd_calib_file.Data());
            }
            f->Close();
            dir = 0;

            f = new TFile(combined_hist_run.Data(),"UPDATE");
            dir = f->GetDirectory("bgsubbed_shifted_ltf_corr_adc_gt_thresh_sanormed");
            if (dir==0)
            {
                    std::cout << "\tfailed to find bgsubbed_shifted_ltf_corr_adc_gt_thresh_sanormed folder in " << f->GetName() << std::endl;
            }
            else
            {
                    std::cout << "\tCalibrating bgsubbed_shifted_ltf_corr_adc_gt_thresh_sanormed with " << bgnd_calib_file
                            << std::endl;
                    CalibrateHists(dir, bgnd_calib_file.Data());
            }
            f->Close();
    }

    if (OkToContinue("integrate all \"adc_gt_thresh_tofcut\" histograms"))
    {
        PAnalysis::IntegrateSiStrips(combined_hist_run,
                                     TString("ltf_corr_adc_gt_thresh"),
                                     TString(""));

        PAnalysis::IntegrateSiStrips(combined_hist_run,
                                     "normed_bgsubbed_ltf_corr_adc_gt_thresh",
                                     "",
                                     int_subbed_file,
                                     "bgsubbed_shifted_ltf_corr_adc_gt_thresh");

        PAnalysis::IntegrateSiStrips(combined_hist_run,
                                     "ltf_corr_adc_gt_thresh",
                                     "_sanormed",
                                     int_sanormed_file,
                                     "ltf_corr_adc_gt_thresh_sanormed");

        PAnalysis::IntegrateSiStrips(combined_hist_run,
                                     "ltf_corr_normed_bgsubbed_ltf_corr_adc_gt_thresh",
                                     "_sanormed",
                                     int_subbed_sanormed_file,
                                     "bgsubbed_shifted_ltf_corr_adc_gt_thresh_sanormed");

    }

    if (OkToContinue("generate angular distribution"))
    {
//        GenerateTGraphInputFiles(int_file.Data(),
//                                 angle_file.Data(),
//                                 angdist_text_file.Data());
//
//        GenerateTGraphInputFiles(int_subbed_file.Data(),
//                                 angle_file.Data(),
//                                 angdist_subbed_text_file.Data());

//        GenerateTGraphInputFiles(int_sanormed_file.Data(),
//                                 angle_file.Data(),
//                                 angdist_sanormed_text_file.Data());

        GenerateTGraphInputFiles(int_subbed_sanormed_file.Data(),
                                 angle_file.Data(),
                                 angdist_subbed_sanormed_text_file.Data());

    }

    if (OkToContinue("generate ratio and asymmetry plots"))
    {
//        f = new TFile(angdist_root_file.Data(),"UPDATE");
//        GenerateRatioAndAsymmetry(f);
//        f->Close();
//
//        f = new TFile(angdist_subbed_root_file.Data(),"UPDATE");
//        GenerateRatioAndAsymmetry(f);
//        f->Close();
//
//        f = new TFile(angdist_sanormed_root_file.Data(),"UPDATE");
//        GenerateRatioAndAsymmetry(f);
//        f->Close();

        f = new TFile(angdist_subbed_sanormed_root_file.Data(),"UPDATE");
        GenerateRatioAndAsymmetry(f);
        f->Close();

    }

    if (OkToContinue("create overlaid angular distributions"))
    {
        gROOT->SetBatch(true);
//        f = new TFile(angdist_root_file.Data(),"UPDATE");
//        CreatePlotOverlay(f, Form("%s E_{#gamma}=%.1f MeV %s Yields", target.Data(), eg, pol.Data()));
//        f->Close();
//
//        f = new TFile(angdist_subbed_root_file.Data(),"UPDATE");
//        CreatePlotOverlay(f, Form("%s E_{#gamma}=%.1f MeV %s Yields (subbed)", target.Data(), eg, pol.Data()));
//        f->Close();
//
//        f = new TFile(angdist_sanormed_root_file.Data(),"UPDATE");
//        CreatePlotOverlay(f, Form("%s E_{#gamma}=%.1f MeV %s Yields SA Normed", target.Data(), eg, pol.Data()));
//        f->Close();

        f = new TFile(angdist_subbed_sanormed_root_file.Data(),"UPDATE");
        CreatePlotOverlay(f, Form("%s E_{#gamma}=%.1f MeV %s Yields SA Normed (subbed)", target.Data(), eg, pol.Data()));
        f->Close();

        gROOT->SetBatch(false);
    }



    if (OkToContinue("form alpha_ratio graph"))
    {
        f = new TFile(combined_hist_run.Data(),"UPDATE");

        StripDb sdb(angle_file.Data());
        TH1* halpha_ints;
        f->GetObject("alpha_ints",halpha_ints);
        TGraphErrors* gr_alpha_ratio = CreateRatioFromStripHist(halpha_ints,sdb);

        gr_alpha_ratio->Write("alpha_ratio");
        f->Close();
    }


    if (OkToContinue("incorporate systematic uncertainty into the ratio"))
    {
        f = new TFile(combined_hist_run.Data(),"UPDATE");
//        if (gr_==0)
//        {
//            std::cout << "Failed to find the ratio graph" << std::endl;
//        }
//        else
//        {
            TFile* fsim = new TFile("alpha_source_r3_r5_x1_y0_z0_80M_analyzed.root");
            // the next will look for the alpha_ratio graphs in specified directories
            CreateSysCorrRatios(f, fsim, angdist_subbed_sanormed_root_file.Data());
            fsim->Close();
  //      }
        f->Close();

        f = new TFile(angdist_subbed_sanormed_root_file.Data(),"UPDATE");
        TDirectory* d = f->GetDirectory("uncorrected");
        TGraphErrors* gr_ = 0;
        d->GetObject("ratio",gr_);
        if (gr_==0)
        {
            std::cout << "Failed to find the ratio graph" << std::endl;
        }
        else
        {
            TGraphErrors* grsys = 0;
            f->GetObject("systematic_alpha_diffs",grsys);
            if (grsys!=0)
            {
                CreateCorrectedRatios(gr_, grsys, f);
            }
            else
            {
                std::cout << "Couldn't find systematic_alpha_diffs graph" << std::endl;
            }
            fsim->Close();
        }

        f->Close();

    }





//    if (OkToContinue("generate 2D angular dist"))
//    {
//        GenerateTGraph2D(angdist_corr_text_file.Data(), angdist_root_file.Data());
//        f = new TFile(angdist_root_file.Data(),"UPDATE");
////        GenerateTH2D(f);
//        f->Close();
//
//        GenerateTGraph2D(angdist_corr_text_file.Data(), angdist_root_file.Data());
//        f = new TFile(angdist_root_file.Data(),"UPDATE");
////        GenerateTH2D(f);
//        f->Close();
//
//
//    }

    cout << "ANALYSIS COMPLETE" << endl;
}
