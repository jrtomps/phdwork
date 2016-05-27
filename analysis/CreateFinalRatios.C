#include "TGraphErrors.h"
#include "TFile.h"
#include "StripInfo.h"
#include "TH1.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>
#include <cmath>
#include <iomanip>
#include "FNameManager.h"
#include "TROOT.h"
#include "CreateRatioFromAlphaInts.cpp"

std::string suffix = "_ind";

TGraphErrors* CreateNo150(const TGraphErrors* gr, const std::string& name)
{
    TGraphErrors* grret = 0;
    const Double_t *x = gr->GetX();
    Int_t i=0; 
    while (i<gr->GetN() && x[i]!=150) ++i;
	if (i!=gr->GetN())
    {
         grret = dynamic_cast<TGraphErrors*>(gr->Clone(name.data()));
         grret->SetName(name.data());
         grret->RemovePoint(i);
         grret->SetEditable(0);
         grret->Sort();
    }
    return grret;
}

TGraphErrors* CreateNo101150(const TGraphErrors* gr, const std::string& name)
{
    TGraphErrors* grret = 0;
    const Double_t *x = gr->GetX();
    Int_t i=0; 
    while (i<gr->GetN() && x[i]!=101) ++i;
	if (i==gr->GetN()) return grret;
    else
    {
         grret = dynamic_cast<TGraphErrors*>(gr->Clone(name.data()));
         grret->SetName(name.data());
         grret->RemovePoint(i);
         grret->SetEditable(0);
         Int_t j=0; 
         x = grret->GetX();
         while (j<grret->GetN() && x[j]!=150) ++j;
         if (j!=grret->GetN())
         {
             grret->RemovePoint(j);
             grret->Sort();
         }
         else
         {
             delete grret;
             grret = 0;
         }
    }
    return grret;
}

TGraphErrors* CreateNo101105150(const TGraphErrors* gr, const std::string& name)
{
    TGraphErrors* grret = 0;
    const Double_t *x = gr->GetX();
    Int_t i=0; 
    while (i<gr->GetN() && x[i]!=101) ++i;
	if (i==gr->GetN()) return grret;
    else
    {
         grret = dynamic_cast<TGraphErrors*>(gr->Clone(name.data()));
         grret->SetName(name.data());
         grret->RemovePoint(i);
         grret->SetEditable(0);
         Int_t j=0; 
         x = grret->GetX();
         while (j<grret->GetN() && x[j]!=105) ++j;
         if (j!=grret->GetN())
         {
             grret->RemovePoint(j);
             grret->Sort();

             Int_t k=0;
             x = grret->GetX();
             while (k<grret->GetN() && x[k]!=150) ++k;
			 if (k!=grret->GetN())
             {
                grret->RemovePoint(k);
             }
             else
             {
                delete grret;
                grret = 0;
             }
         }
         else
         {
             delete grret;
             grret = 0;
         }
    }
    return grret;
}

void CreateFinalRatios(const std::string& target, const Double_t en, const std::string& pol_)
{
	std::string pol = pol_;
	FNameManager fnm("fnameconfig.dat");
	std::stringstream fname; 
	fname.precision(1);
	fname.flags(std::ios::fixed);
	fname << fnm.GetResultsDir(true) << "/" << fnm.GetHistFileBase(false)
	<< target << "_" <<  en << "_" << pol << suffix << ".root";

	TFile* f = new TFile(fname.str().data(), "update");
	if (f->IsOpen()==false)
{
	std::cout << "couldn't open " << fname.str() << std::endl;
	return;
}
	else
		std::cout << "Opened file : " << f->GetName() << std::endl;

	std::string dname = "bgsubbed_shifted_ltf_corr_adc_gt_thresh_sanormed_cal_above_ints";
	TDirectory *dir = f->GetDirectory(dname.data());
	if (dir==0)
	{
		std::cout << "Failed to find directory " << dname << std::endl;
		return;
}
	else std::cout << "opening " << dname << std::endl;

	TH1* h9p3, *h14p3;
	dir->GetObject("angdist_thresh9.3;1",h9p3);
    if (h9p3==0)
    {
        std::cout << "Cannot find angdist_thresh9.3;1" << std::endl;
        return;
    }

	dir->GetObject("angdist_thresh14.3;1",h14p3);
    if (h14p3==0)
    {
        std::cout << "Cannot find angdist_thresh14.3;1" << std::endl;
        return;
    }

	StripDb sdb("angle_data/angles");

	std::cout << "\nGenerating angular distribution for Ethr = 9.3 MeV" << std::endl;
    TGraphErrors* gr9p3 = CreateRatioFromStripHist(h9p3,sdb);
	gr9p3->SetEditable(0);
	gr9p3->SetLineColor(kBlue-2);
	gr9p3->SetMarkerColor(kBlue-2);
	gr9p3->SetFillColor(kBlue-2);
	gr9p3->SetMarkerStyle(20);

    f->cd();
	gr9p3->SetName("frag_ratio_9p3");
	gr9p3->Write("frag_ratio_9p3");

	std::cout << "\nGenerating angular distribution for Ethr = 14.3 MeV" << std::endl;
    TGraphErrors* gr14p3 = CreateRatioFromStripHist(h14p3,sdb);
	gr14p3->SetEditable(0);
	gr14p3->SetLineColor(kRed-2);
	gr14p3->SetMarkerColor(kRed-2);
	gr14p3->SetFillColor(kRed-2);
	gr14p3->SetMarkerStyle(21);

    f->cd();
	gr14p3->SetName("frag_ratio_14p3");
	gr14p3->Write("frag_ratio_14p3");

    f->Close();

	std::stringstream ofname;
	ofname.precision(0);
	ofname.flags(std::ios::fixed);
	
    double integ_part;
    double dec_part = ::modf(en, &integ_part);
    pol[0] = ::tolower(pol[0]);
	ofname << "eg" << integ_part << "p" << 10*dec_part << "_" << pol << "_aan_ratio.root";
	TFile *fout = new TFile(ofname.str().data(),"update");

	gr9p3->Write("frag_ratio_9p3");
    gr14p3->Write("frag_ratio_14p3");    

    TGraphErrors* gr = CreateNo150(gr9p3,"frag_ratio_9p3_no150");
    if (gr!=0)
        gr->Write("frag_ratio_9p3_no150",TObject::kOverwrite);
    gr = CreateNo150(gr14p3, "frag_ratio_14p3_no150");
    if (gr!=0)
        gr->Write("frag_ratio_14p3_no150",TObject::kOverwrite);

    gr = CreateNo101150(gr9p3,"frag_ratio_9p3_no101_150");
    if (gr!=0)
        gr->Write("frag_ratio_9p3_no101_150",TObject::kOverwrite);
    gr = CreateNo101150(gr14p3, "frag_ratio_14p3_no101_150");
    if (gr!=0)    
        gr->Write("frag_ratio_14p3_no101_150",TObject::kOverwrite);
    
    gr = CreateNo101105150(gr9p3,"frag_ratio_9p3_no101_105_150");
    if (gr!=0)
        gr->Write("frag_ratio_9p3_no101_105_150",TObject::kOverwrite);
    gr = CreateNo101105150(gr14p3, "frag_ratio_14p3_no101_105_150");
    if (gr!=0)    
        gr->Write("frag_ratio_14p3_no101_105_150",TObject::kOverwrite);

    fout->Close();
}
