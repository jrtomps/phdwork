{
	_file0->ReOpen("update");

	gDirectory->Delete("ltf_corr_adc_sanormed;1");
	gDirectory->Delete("ltf_corr_adc_gt_thresh_sanormed;1");
	gDirectory->Delete("ltf_corr_adc_gt_thresh_tofcut_sanormed;1");

}
