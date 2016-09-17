// PAnalysis.cpp
//
// Author : Jeromy Tompkins
// Date   : 12/15/2010
//
// Descrip: A namespace for post analysis of the data.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "TObject.h"
#include "TString.h"
#include "TRegexp.h"
#include "TMath.h"
#include "TKey.h"
#include "TList.h"
#include "FNameManager.h"
#include "PAnalysis.h"

TString base("pffrag1_hist_run/pffrag1_hist_run");
TString root(".root");


void ParseFileName(const Char_t *fname, std::vector<Int_t>& run, std::vector<Int_t>& skptwice)
        //PAnalysis::ParseFileName(const Char_t *fname, std::vector<Int_t>& run, std::vector<Int_t>& skptwice)
{ 
    TString fstr(fname);
    fstr.Strip();
    fstr.ToLower();
    if (!fstr.Contains(base) || !fstr.Contains(root))
    {
        std::cout << "Invalid file name" << std::endl;
        run.push_back(0);
        skptwice.push_back(0);
        return;
    }

    TString fstr1 = fstr(base.Length(),fstr.Length()-base.Length()-root.Length());
    TString fstr2;
    Ssiz_t pos1 = fstr1.First("[t_]");
    Int_t n2skip;
    Int_t strlen;
    Int_t i=0;
#ifdef DB_PARSER
    std::cout << fstr.Length() << std::endl;
    std::cout << base.Length() << std::endl;
    std::cout << root.Length() << std::endl;
#endif      

    while (pos1 != -1)
    {
        fstr2 = fstr1(0,pos1);
        run.push_back(fstr2.Atoi());

#ifdef DB_PARSER
        std::cout << fstr1.Data() << std::endl;
        std::cout << fstr2.Data() << std::endl;
#endif      

        if (fstr1[pos1] == 't')
	{
            n2skip = 2;
            skptwice.push_back(1);
	}
        else
	{
            n2skip = 1;
            skptwice.push_back(0);
	}

        fstr1 = fstr1(pos1+n2skip,fstr1.Length()-pos1-n2skip);
        pos1 = fstr1.First("[t_]");
    }

    run.push_back(fstr1.Atoi());
    skptwice.push_back(0);
    //   for (Int_t i=0; i<run.size(); i++)
    //     std::cout << run.at(i) << " : " << skptwice.at(i) << std::endl;

}


void PAnalysis::CombineRuns(const Char_t* filename)
{
    std::vector<Int_t> runs;
    std::vector<Int_t> skiptwice;
    TString command("hadd -f ");

    ParseFileName(filename, runs, skiptwice);

    command += filename;
    command += " ";
    Int_t lastrun=runs[0];
    for (Int_t i=0; i<runs.size(); i++)
    {
        //      std::cout << runs[i] << std::endl;

        if (runs[i] == 0)
	{
            std::cout << "Parser finds no run info in filename" << std::endl;
            return;
	}
        if (runs[i] < lastrun)
	{
            std::cout << "Error: Run numbers must be increasing order." << std::endl;
            return;
	}
        else
	{
            for (Int_t j=runs[i]; j<=runs[i+skiptwice[i]]; j++)
	    {
                command += Form("%s%i%s ",
                                (const Char_t*) base,
                                j,
                                (const Char_t*) root);
	    }
            lastrun = runs[i+skiptwice[i]];
            i += skiptwice[i];

	}
    }

    //  std::cout << (const Char_t*) command << std::endl;
    Int_t dummy = system((const Char_t*) command);

}

void PAnalysis::IntegrateSiStrips(TString fname,
                                  TString hist_prefix,
                                  TString hist_suffix,
                                  TString outfname,
                                  TString dname)
{

    FNameManager fnm;
//    std::cout << "Inside IntegrateSiStrips(TString,TString,TString)" << std::endl; std::cout.flush();
    Int_t nstrips_per_det = 16;
    TString intdir = (fnm.GetResultsDir(true) + "/IntOutput/");
//    const Char_t *intdir  =

    TH1F *hp;

    TH1F *h  = 0;
    TFile *f = new TFile(fname,"UPDATE");

    TString fs(fname);

    std::cout << "fname=" << fname << std::endl;
    std::cout << "intdir=" << intdir << std::endl;

    TString ofstr;
    if (outfname.Length()==0)
    {
        ofstr = fname;
        ofstr.ReplaceAll((fnm.GetResultsDir(true)+"/"+fnm.GetHistFileBase(false)),intdir);
        ofstr.Remove(ofstr.Length()-5,5);
    }
    else
    {
        ofstr = outfname;
    }
    std::cout << "ofstr=" << ofstr << std::endl;
    std::ofstream of;
    of.open(ofstr.Data(), std::ofstream::out);
    if (of.fail())
    {
        of.clear();
	of.open(ofstr.Data(), std::ofstream::out);
        if (of.fail())
	{
                std::cerr << "Failure opening " << ofstr << std::endl;
        	return;
	}
    }

    if (!f->IsOpen())
    {
        std::cout << "Returning from function "
                << "PAnalysis::IntegrateSiStrips(const Char_t *fname, Int_t binlo, Int_t binhi)"
                << std::endl;
        return;
    }

    of << "Integrated data for "
            << fname
            << " Si strips"
            << std::endl;

    of << std::left
            << std::setw(4) << "#"
            << std::setw(8) << "Low"
            << std::setw(8) << "Hi"
            << std::right
            << std::setw(10) << "Intgrl"
            << std::setw(10) << "StatUnc"
            << std::endl;
    of
            << "--------------------------------------------"
            << std::endl;

    Int_t count     = 0;
    Int_t failcount = 0;
    Int_t maxfails  = 3;
    Int_t pcount    = 0;
    Float_t integral;


    TDirectory *dir = 0;
    if (dname.Length()==0)
        dir = f->GetDirectory(hist_prefix.Data());
    else
    {
        std::cout << "\tGetting directory : " << dname << std::endl;
        dir = f->GetDirectory(dname.Data());
    }
    if (dir==0)
    {
        std::cout << hist_prefix << " directory doesn't exist" << std::endl;
        return;
    }

    do
    {
        h = dynamic_cast<TH1F*>(dir->Get(Form("%s%i%s",hist_prefix.Data(),count,hist_suffix.Data())));
        if (h!=NULL)
        {
            //            integral = h->Integral();
            Double_t unc = 0;
            integral = h->IntegralAndError(0,h->GetNbinsX()+1,unc);
            of << std::left
                    << std::setw(4) << std::setprecision(3) << count
                    << std::setw(8) << h->GetXaxis()->GetXmin()
                    << std::setw(8) << h->GetXaxis()->GetXmax()
                    << std::right
                    << std::setw(10) << integral
                    << std::setw(10) << unc
//                    << std::setw(10) << TMath::Sqrt(integral)
                    << std::endl;
            pcount++;
        }
        else
        {
            failcount++;
            std::cout
                    << Form("%s%i%s",hist_prefix.Data(),count,hist_suffix.Data())
                    << " doesn't exist"
                    << std::endl;
        }
        count++;
    } while (failcount < maxfails);

    of.close();
    f->Close();

}

void PAnalysis::IntegrateSiStrips(const Char_t *fname, Int_t binlo, Int_t binhi,
                                  const Char_t *hist_prefix, const Char_t *hist_suffix)
{
    Int_t nstrips_per_det = 16;
    //  const Char_t *hist_prefix = "adc";
    const Char_t *ofname  = "IntegrateSiStrips.out";
    const Char_t *intdir  = "IntOutput";

    TH1F *h  = 0;

    TFile *f = new TFile(fname,"READ");

    std::ofstream of(ofname,std::ofstream::app | std::ofstream::out);
    if (of.fail())
    {
        std::cerr << "Failure opening " << ofname << std::endl;
        return;
    }

    if (!f->IsOpen())
    {
        std::cout
                << "Returning from function "
                << "PAnalysis::IntegrateSiStrips(const Char_t *fname, Int_t binlo, Int_t binhi)"
                << std::endl;
        return;
    }

    of
            << "Integrated data for "
            << fname
            << " Si strips"
            << std::endl;

    of
            << "Range of integration::"
            << " lowbin=" << std::setw(6) << binlo
            << " hibin="  << std::setw(6) << binhi
            << std::endl;

    Int_t count     = 0;
    Int_t failcount = 0;
    Int_t maxfails  = 3;
    Float_t integral;
    Double_t err;
    do {
        h = dynamic_cast<TH1F*>(f->FindObjectAny(Form("%s%i%s",hist_prefix,count,hist_suffix)));
        if (h)
        {
            if (binhi==-1 || binhi > h->GetNbinsX())
                //	if (binhi=-1)
                binhi = h->GetNbinsX();

            //      	integral = h->IntegralAndError(binlo,binhi,err);
            integral = h->Integral(binlo, binhi);
            err=0;

            of
                    << std::setiosflags(std::ios::fixed)
                    << std::setw(4) << std::setprecision(2) << std::left << count
                    << std::setw(8) << integral << " +- " << std::setw(8) << err
                    << std::endl;
        }
        else
        {
            failcount++;
            std::cout
                    << Form("%s%i%s",hist_prefix,count,hist_suffix)
                    << " doesn't exist"
                    << std::endl;
        }
        count++;
    } while (failcount < maxfails);

    of.close();
    f->Close();

}

void PAnalysis::IntegrateSiStrips(const Char_t *fname, const Char_t *boundsfile,
                                  const Char_t *hist_prefix, const Char_t *hist_suffix)
{
    Int_t nstrips_per_det = 16;
    //  const Char_t *hist_prefix = "adc";
    const Char_t *ofname  = "IntegrateSiStrips.out";
    const Char_t *intdir  = "IntOutput";

    std::vector<Int_t> binlo;
    std::vector<Int_t> binhi;

    TH1F *hp;
    TObjArray *hints = new TObjArray(nstrips_per_det);
    TObjArray *hlows = new TObjArray(nstrips_per_det);
    TObjArray *hhis = new TObjArray(nstrips_per_det);

    std::ifstream bfile(boundsfile,std::ifstream::in);
    if (bfile.fail())
    {
        std::cerr << "Failure opening " << boundsfile << std::endl;
        return;
    }

    Int_t dumlo, dumhi;
    Int_t j=0;
    while (1)
    {
        bfile >> dumlo >> dumhi;
        if (bfile.eof())
            break;

        // Allow comments for each line
        bfile.ignore(400,'\n');

        binlo.push_back(dumlo);
        binhi.push_back(dumhi);

        if (j%nstrips_per_det==0)
	{
            hp = new TH1F(Form("hint%i",j/nstrips_per_det),
                          Form("Integral Det%i;Strip Number;Integral (cts);",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det); // Originally had a max of nstrips_per_det-1
            hints->Add(hp);
            hp = new TH1F(Form("hlow%i",j/nstrips_per_det),
                          Form("Lowerbound for Integral Det%i;Strip Number;Channel",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det); // Originally had a max of nstrips_per_det-1
            hlows->Add(hp);
            hp = new TH1F(Form("hhi%i",j/nstrips_per_det),
                          Form("Upperbound for Integral Det%i;Strip Number;Channel",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det); // Originally had a max of nstrips_per_det-1
            hhis->Add(hp);
	}
        j++;
    }
    bfile.close();

    TH1F *h  = 0;
    TFile *f = new TFile(fname,"UPDATE");

    TString fs(fname);

    TString ofstr(intdir);
    ofstr += "/";
    ofstr += fs(16,fs.Length()-16);
    ofstr.Remove(ofstr.Length()-5,5);

    std::ofstream of(ofstr.Data(),std::ofstream::out);
    //  std::ofstream of(ofstr.Data(),std::ofstream::app | std::ofstream::out);

    if (of.fail())
    {
        std::cerr << "Failure opening " << ofname << std::endl;
        return;
    }

    if (!f->IsOpen())
    {
        std::cout
                << "Returning from function "
                << "PAnalysis::IntegrateSiStrips(const Char_t *fname, Int_t binlo, Int_t binhi)"
                << std::endl;
        return;
    }

    of
            << "Integrated data for "
            << fname
            << " Si strips"
            << std::endl;
    of
            << "Integration boundary data found in "
            << boundsfile
            << std::endl;

    of
            << std::setw(4) << std::left << "#"
            << std::setw(6) << "Low"
            << std::setw(6) << "Hi"
            << std::setw(8) << std::right << "Intgrl"
            << std::setw(8) << std::right << "StatUnc"
            << std::setw(8) << std::right << "RelUnc"
            << std::endl;
    of
            << "--------------------------------------------"
            << std::endl;

    Int_t count     = 0;
    Int_t failcount = 0;
    Int_t maxfails  = 3;
    Int_t pcount    = 0;
    Float_t integral, tot_integral=0;
    Double_t err;
    do {
        h = dynamic_cast<TH1F*>(f->FindObjectAny(Form("%s%i%s",hist_prefix,count,hist_suffix)));
        if (h)
        {
            if (binlo.size()==pcount || binhi.size()==pcount)
            {
                of
                        << "Terminating function: no more bin settings"
                        << std::endl;
                break;
            }

            if (binhi[pcount]==-1 || binhi[pcount]>h->GetNbinsX())
                binhi[pcount] = h->GetNbinsX();


            //      	integral = h->IntegralAndError(binlo[pcount],binhi[pcount],err);
            integral = h->Integral(binlo[pcount],binhi[pcount]);
            err = TMath::Sqrt(integral);

            (dynamic_cast<TH1F*>(hints->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,integral);
            (dynamic_cast<TH1F*>(hints->At(pcount/nstrips_per_det)))->SetBinError(pcount%nstrips_per_det+1,err);
            (dynamic_cast<TH1F*>(hlows->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,binlo[pcount]);
            (dynamic_cast<TH1F*>(hhis->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,binhi[pcount]);

            of
                    << std::setiosflags(std::ios::fixed)
                    << std::setw(4) << std::setprecision(3) << std::left << count
                    << std::setw(6) << binlo[pcount]
                    << std::setw(6) << binhi[pcount]
                    << std::setw(8) << std::right << integral
                    << std::setw(4) << " +- " << std::setw(8) << std::right << err
                    << std::setw(8) << std::right << err/integral
                    << std::endl;
            pcount++;

            tot_integral += integral;
        }
        else
        {
            failcount++;
            std::cout
                    << Form("%s%i%s",hist_prefix,count,hist_suffix)
                    << " doesn't exist"
                    << std::endl;
        }
        count++;
    } while (failcount < maxfails);

    of
            << std::setw(10) << std::left  << "Total = "
            << std::setw(10) << std::right << tot_integral
            << std::endl;

    // Write the hists to root file
    for (Int_t i=0; i<hints->GetEntries(); i++)
    {
        (dynamic_cast<TH1*>(hints->At(i)))->Write("",TObject::kOverwrite);
        (dynamic_cast<TH1*>(hlows->At(i)))->Write("",TObject::kOverwrite);
        (dynamic_cast<TH1*>(hhis->At(i)))->Write("",TObject::kOverwrite);
    }

    of.close();
    f->Close();

    delete hints;
    delete hlows;
    delete hhis;

}

void PAnalysis::IntegrateCombineSiStrips(const Char_t *fname, const Char_t *boundsfile,
                                         const Char_t *hist_prefix, const Char_t *hist_suffix)
{
    Int_t nstrips_per_det = 16;
    // const Char_t *hist_prefix = "adc";
    const Char_t *ofname  = "IntegrateSiStrips.out";
    const Char_t *intdir  = "IntOutput";

    std::vector<Int_t> binlo;
    std::vector<Int_t> binhi;

    TH1F *hp;
    TObjArray *hints = new TObjArray(nstrips_per_det);
    TObjArray *hlows = new TObjArray(nstrips_per_det);
    TObjArray *hhis = new TObjArray(nstrips_per_det);

    std::ifstream bfile(boundsfile,std::ifstream::in);
    if (bfile.fail())
    {
        std::cerr << "Failure opening " << boundsfile << std::endl;
        return;
    }

    Int_t dumlo, dumhi;
    Int_t j=0;
    while (1)
    {
        bfile >> dumlo >> dumhi;
        if (bfile.eof())
            break;

        // Allow comments for each line
        bfile.ignore(400,'\n');

        binlo.push_back(dumlo);
        binhi.push_back(dumhi);

        if (j%nstrips_per_det==0)
	{
            hp = new TH1F(Form("hint%i",j/nstrips_per_det),
                          Form("adjadc Integral Det%i",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det); // Originally had a max of nstrips_per_det-1
            hints->Add(hp);
            hp = new TH1F(Form("hlow%i",j/nstrips_per_det),
                          Form("Lowerbound for adjadc Integral Det%i",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det); // Originally had a max of nstrips_per_det-1
            hlows->Add(hp);
            hp = new TH1F(Form("hhi%i",j/nstrips_per_det),
                          Form("Upperbound for adjadc Integral Det%i",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det); // Originally had a max of nstrips_per_det-1
            hhis->Add(hp);
	}
        j++;
    }
    bfile.close();

    TH1F *h  = 0;
    TFile *f = new TFile(fname,"UPDATE");

    TString fs(fname);

    TString ofstr(intdir);
    ofstr += "/";
    ofstr += fs(16,fs.Length()-16);
    ofstr.Remove(ofstr.Length()-5,5);

    std::ofstream of(TString(ofstr(0,ofstr.Length()-5)).Data(),std::ofstream::app | std::ofstream::out);
    if (of.fail())
    {
        std::cerr << "Failure opening " << ofname << std::endl;
        return;
    }

    if (!f->IsOpen())
    {
        std::cout
                << "Returning from function "
                << "PAnalysis::IntegrateSiStrips(const Char_t *fname, Int_t binlo, Int_t binhi)"
                << std::endl;
        return;
    }

    of
            << "Integrated data for "
            << fname
            << " Si strips"
            << std::endl;
    of
            << "Integration boundary data found in "
            << boundsfile
            << std::endl;

    of
            << std::setw(4) << std::left << "#"
            << std::setw(6) << "Low"
            << std::setw(6) << "Hi"
            << std::setw(8) << std::right << "Intgrl"
            << std::setw(8) << std::right << "StatUnc"
            << std::endl;
    of
            << "------------------------------------"
            << std::endl;

    Int_t count     = 0;
    Int_t failcount = 0;
    Int_t maxfails  = 3;
    Int_t pcount    = 0;
    Float_t integral;
    do {
        h = dynamic_cast<TH1F*>(f->FindObjectAny(Form("%s%i%s",hist_prefix,count,hist_suffix)));
        if (h)
        {
            if (binlo.size()==pcount || binhi.size()==pcount)
            {
                of
                        << "Terminating function: no more bin settings"
                        << std::endl;
                break;
            }

            if (binhi[pcount]==-1 || binhi[pcount]>h->GetNbinsX())
                binhi[pcount] = h->GetNbinsX();

            integral = h->Integral(binlo[pcount],binhi[pcount]);

            (dynamic_cast<TH1F*>(hints->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,integral);
            (dynamic_cast<TH1F*>(hlows->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,binlo[pcount]);
            (dynamic_cast<TH1F*>(hhis->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,binhi[pcount]);

            of
                    << std::setw(4) << std::setprecision(3) << std::left << count
                    << std::setw(6) << binlo[pcount]
                    << std::setw(6) << binhi[pcount]
                    << std::setw(8) << std::right << integral
                    << std::setw(8) << std::right << 1./TMath::Sqrt(integral)
                    << std::endl;
            pcount++;
        }
        else
        {
            failcount++;
            std::cout
                    << Form("%s%i%s",hist_prefix,count,hist_suffix)
                    << " doesn't exist"
                    << std::endl;
        }
        count++;
    } while (failcount < maxfails);

    // Write the hists to root file
    for (Int_t i=0; i<hints->GetEntries(); i++)
    {
        (dynamic_cast<TH1*>(hints->At(i)))->Write();
        (dynamic_cast<TH1*>(hlows->At(i)))->Write();
        (dynamic_cast<TH1*>(hhis->At(i)))->Write();
    }

    of.close();
    f->Close();

    delete hints;
    delete hlows;
    delete hhis;

}


void PAnalysis::IntegrateAndExtrapolateSiStrips(const Char_t *fname, const Char_t *boundsfile)
{
    Int_t nstrips_per_det = 16;
    const Char_t *objname = "adjadc";
    const Char_t *ofname  = "IntegrateAndExtrapolateSiStrips.out";

    std::vector<Int_t> binlo;
    std::vector<Int_t> binhi;
    std::vector<Int_t> binmin;

    TH1F *hp;
    TObjArray *hints = new TObjArray(nstrips_per_det);
    TObjArray *hlows = new TObjArray(nstrips_per_det);
    TObjArray *hhis = new TObjArray(nstrips_per_det);

    std::ifstream bfile(boundsfile,std::ifstream::in);
    if (bfile.fail())
    {
        std::cerr << "Failure opening " << boundsfile << std::endl;
        return;
    }

    Int_t dumlo, dumhi, dummin;
    Int_t j=0;
    while (1)
    {
        bfile >> dumlo >> dumhi >> dummin;
        if (bfile.eof())
            break;

        // Allow comments for each line
        bfile.ignore(400,'\n');

        binlo.push_back(dumlo);
        binhi.push_back(dumhi);
        binmin.push_back(dummin);

        if (j%nstrips_per_det==0)
	{
            hp = new TH1F(Form("hintextr%i",j/nstrips_per_det),
                          Form("adjadc Extrap. Integral Det%i",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det);
            hints->Add(hp);
            hp = new TH1F(Form("hlowextr%i",j/nstrips_per_det),
                          Form("Lowerbound for adjadc Extrap. Integral Det%i",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det);
            hlows->Add(hp);
            hp = new TH1F(Form("hhiextr%i",j/nstrips_per_det),
                          Form("Upperbound for adjadc Extrap. Integral Det%i",j/nstrips_per_det),
                          nstrips_per_det,0,nstrips_per_det);
            hhis->Add(hp);
	}
        j++;
    }
    bfile.close();

    TH1F *h  = 0;
    TFile *f = new TFile(fname,"UPDATE");

    std::ofstream of(ofname,std::ofstream::app | std::ofstream::out);
    if (of.fail())
    {
        std::cerr << "Failure opening " << ofname << std::endl;
        return;
    }

    if (!f->IsOpen())
    {
        std::cout
                << "Returning from function "
                << "PAnalysis::IntegrateAndExtrapolateSiStrips(const Char_t *fname, Int_t binlo, Int_t binhi)"
                << std::endl;
        return;
    }

    of
            << "Integrated And Extrapolated data for "
            << fname
            << " Si strips"
            << std::endl;
    of
            << "Integration boundary data found in "
            << boundsfile
            << std::endl;

    of
            << std::setw(4)  << std::left << "#"
            << std::setw(6)  << "Low"
            << std::setw(6)  << "Hi"
            << std::setw(6)  << "min"
            << std::setw(8)  << "Intgrl"
            << std::setw(15) << "Extrap. Intgrl"
            << std::endl;
    of
            << "-----------------------------------------------------------------"
            << std::endl;

    Int_t count     = 0;
    Int_t failcount = 0;
    Int_t maxfails  = 3;
    Int_t pcount    = 0;
    Float_t integral=0, ave_int_per_bin=0, intcorrection=0;
    do {
        h = dynamic_cast<TH1F*>(f->FindObjectAny(Form("%s%i",objname,count)));
        if (h)
        {
            if (binlo.size()==pcount || binhi.size()==pcount)
            {
                of
                        << "Terminating function: no more bin settings"
                        << std::endl;
                break;
            }

            if (binhi[pcount]==-1 || binhi[pcount]>h->GetNbinsX())
                binhi[pcount] = h->GetNbinsX();

            integral = h->Integral(binlo[pcount],binhi[pcount]);

            ave_int_per_bin = integral/(binhi[pcount]-binlo[pcount]);
            intcorrection   = (binlo[pcount]-binmin[pcount])*ave_int_per_bin;

            (dynamic_cast<TH1F*>(hints->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,integral+intcorrection);
            (dynamic_cast<TH1F*>(hlows->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,binlo[pcount]);
            (dynamic_cast<TH1F*>(hhis->At(pcount/nstrips_per_det)))->SetBinContent(pcount%nstrips_per_det+1,binhi[pcount]);

            of
                    << std::setw(4)  << std::setprecision(2) << std::left << count
                    << std::setw(6)  << binlo[pcount]
                    << std::setw(6)  << binhi[pcount]
                    << std::setw(6)  << binmin[pcount]
                    << std::setw(8)  << integral
                    << std::setw(16) << integral + intcorrection
                    << std::endl;
            pcount++;
        }
        else
        {
            failcount++;
            std::cout
                    << Form("%s%i",objname,count)
                    << " doesn't exist"
                    << std::endl;
        }
        count++;
    } while (failcount < maxfails);

    // Write the hists to root file
    for (Int_t i=0; i<hints->GetEntries(); i++)
    {
        (dynamic_cast<TH1*>(hints->At(i)))->Write();
        (dynamic_cast<TH1*>(hlows->At(i)))->Write();
        (dynamic_cast<TH1*>(hhis->At(i)))->Write();
    }

    of.close();
    f->Close();

    delete hints;
    delete hlows;
    delete hhis;

}


void PAnalysis::CreateRateHists(const Char_t *fname, Float_t seconds)
{
    TPaveText *pt;
    TFile *f = new TFile(fname,"UPDATE");
    if (f->IsZombie())
        return;

    TH1F *h = 0;
    TH1F *hrate;
    Int_t count=0;
    do {
        h = 0;
        h =(dynamic_cast<TH1F*>(f->FindObjectAny(Form("hint%i",count))));
        if (h)
        {
            hrate = dynamic_cast<TH1F*>(h->Clone(Form("hrate%i",count)));
            hrate->SetTitle(Form("Rate in each strip: Det#%i",count));
            hrate->Scale(1./seconds);
            hrate->GetYaxis()->SetTitle("Counts/s");
            hrate->GetXaxis()->SetTitle("Strip Number");

            pt = new TPaveText(0.8,0.7,1.0,0.9,"NDC ARC");
            pt->SetName(Form("PT_totrate%i",count));
            pt->AddText(Form("Time = %f s",seconds));
            pt->AddText(Form("Rate_{tot}=%f s^{-1}",h->Integral()/seconds));
            pt->Write();
            hrate->Write();
        }

        count++;

    } while (h);

    f->Close();


}

void PAnalysis::CreateExtrapolatedRateHists(const Char_t *fname, Float_t seconds)
{
    TPaveText *pt;
    TFile *f = new TFile(fname,"UPDATE");
    if (f->IsZombie())
        return;

    TH1F *h = 0;
    TH1F *hrate;
    Int_t count=0;
    do {
        h = 0;
        h =(dynamic_cast<TH1F*>(f->FindObjectAny(Form("hintextr%i",count))));
        if (h)
        {
            hrate = dynamic_cast<TH1F*>(h->Clone(Form("hrateextr%i",count)));
            hrate->SetTitle(Form("Rate in each strip: Det#%i",count));
            hrate->Scale(1./seconds);
            hrate->GetYaxis()->SetTitle("Counts/s");
            hrate->GetXaxis()->SetTitle("Strip Number");

            pt = new TPaveText(0.8,0.7,1.0,0.9,"NDC ARC");
            pt->SetName(Form("PT_totextrrate%i",count));
            pt->AddText(Form("Time = %f s",seconds));
            pt->AddText(Form("Rate_{tot}=%f s^{-1}",h->Integral()/seconds));
            pt->Write();
            hrate->Write();
        }

        count++;

    } while (h);

    f->Close();


}


Int_t PAnalysis::GuessRunNumberFromFileName(const Char_t* filename)
{
    TRegexp re("[0-9]+.root$");
    TString fname(filename);
    fname.Strip();
    Int_t index = fname.Index(re);
    std::cout << index << std::endl;
    return TString(fname(index,fname.Length()-5-index)).Atoi();

}


std::vector<TH1*> PAnalysis::GetAllTH1(TFile& rootfile)
{
    TH1 *h=0;
    TKey *obj=0;
    TListIter li(rootfile.GetListOfKeys());
    std::vector<TH1*> histvec;

    while (obj = static_cast<TKey*>(li.Next()))
    {
        h = dynamic_cast<TH1*>(obj->ReadObj());
        if (h!=NULL && h->InheritsFrom("TH1"))
            histvec.push_back(h);
    }
    std::cout << histvec.size() << std::endl;
    return histvec;
}
