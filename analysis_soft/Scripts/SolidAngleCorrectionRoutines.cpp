#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include "TGraphErrors.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "Rtypes.h"
#include "TrimTheZeroes.cpp"
#include "TableMap.hpp"
#include "ColumnFileLoader.h"
#include "VectorSorter.h"
#include "VectorWriter.h"

typedef std::pair<Double_t, Double_t> Point;
typedef std::vector<Point> DataVector;
typedef std::vector<DataVector> DetData;

class SolidAngleLoader
{
public:
    SolidAngleLoader(std::string fname)
        : fNPerDet(16),
        fDetData()
    {
        LoadCorrections(fname);
    }

    SolidAngleLoader(const SolidAngleLoader& that)
        : fNPerDet(that.fNPerDet), fDetData()
    {
        if (this != &that)
        {
            fDetData = that.fDetData;
        }
    }

    SolidAngleLoader& operator=(SolidAngleLoader& that )
                               {
        if (this != &that)
        {
            std::swap(fDetData, that.fDetData);
        }
        return *this;
    }

    ~SolidAngleLoader() throw() {};

    DataVector GetSAForDet(UInt_t i)
    {
        if (i<fDetData.size())
            return fDetData.at(i);
        else
            return DataVector();
    }

    DataVector GetSAForDets(std::string phi_orientation)
    {
        DataVector vec;
        if (phi_orientation.find("perp") != std::string::npos
            || phi_orientation.find("90") != std::string::npos)
        {
            vec = fDetData.at(3);
            vec.insert(vec.end(),fDetData.at(0).begin(),fDetData.at(0).end());
        }
        else
        {
            vec = fDetData.at(1);
            vec.insert(vec.end(),fDetData.at(2).begin(),fDetData.at(2).end());
        }
        return vec;
    }


private:
    const UInt_t fNPerDet;
    DetData fDetData;
    Bool_t FileIsNotOkay(std::ifstream& f);
    Bool_t LineShouldBeSkipped(std::string &line);
    void LoadCorrections(std::string fname);
};

Bool_t SolidAngleLoader::FileIsNotOkay(std::ifstream& f)
{
    Bool_t notokay = false;
    std::ios::iostate state = f.rdstate();

    if (f.eof())
    {
        notokay = true;
    }
    else if (f.fail())
    {
        std::cerr << std::hex << std::showbase
                << "fstream is corrupted (badbit=" << (state&std::ios_base::badbit)
                << ",failbit=" << (state&std::ios_base::failbit) << ")"
                << std::dec << std::noshowbase
                << std::endl;
        notokay = true;
    }

    return notokay;
}

Bool_t SolidAngleLoader::LineShouldBeSkipped(std::string& line)
{
    if ( (line.length()==0) || (line.find_first_not_of(" \t") == std::string::npos) )
        return true;
    else
        return false;
}


void SolidAngleLoader::LoadCorrections(std::string fname)
{
    std::ifstream file(fname.data(), std::ifstream::in);
    if (file.fail())
    {
        std::cerr << "Failed to open " << fname << std::endl;
        return;
    }

    DataVector data;

    Double_t sa, usa;
    std::string aline;
    std::stringstream ss;
    while(1)
    {
        getline(file,aline);
        std::cout << aline << std::endl;
        if ( FileIsNotOkay(file) )
            break;
        else if ( LineShouldBeSkipped(aline) )
            continue;
        else
        {
            ss.clear(); ss.str("");

            ss << aline;
            ss >> sa >> usa;

            if (!ss.fail())
            {
                if (fDetData.size()==0)
                    fDetData.push_back(data);
                else if (fDetData.back().size()%fNPerDet==0)
                    fDetData.push_back(data);

                fDetData.back().push_back(std::make_pair(sa,usa));
            }
        }
    }

    file.close();
}

Double_t Mult(Double_t x1, Double_t x2) { return x1*x2; }

TGraphErrors* CreateCorrectedGraph(TGraphErrors* gr, DataVector corr)
{
    using TMath::Sqrt;
    using TMath::Power;

    TGraphErrors *gr_ret = 0;

    if (gr->GetN() != corr.size())
    {
        std::cout << "Error! TGraphErrors have different number of entries" << std::endl;
        return gr_ret;
    }
    else
        gr_ret = new TGraphErrors(gr->GetN());

    Double_t *x  = gr->GetX();
    Double_t *ex = gr->GetEX();
    Double_t *y  = gr->GetY();
    Double_t *ey = gr->GetEY();
    Double_t val, uval;

    for (Int_t i=0; i<gr->GetN(); i++)
    {
        if (corr[i].first!=0)
        {
            val = y[i]/corr[i].first;
            uval = val*Sqrt(Power(ey[i]/y[i],2.0)
                            + Power(corr[i].second/corr[i].first,2.0));
        }
        else
        {
            val = 0;
            uval = 0;
        }
        gr_ret->SetPoint(i,x[i], val);
        gr_ret->SetPointError(i,ex[i],uval);
    }

    return gr_ret;
}

Bool_t FileIsBad(TFile* f)
{
    Bool_t flag = false;
    if (f==NULL)
    {
        std::cout << "null file argument" << std::endl;
        flag = true;
    }
    else if ( !f->IsOpen() )
    {
        std::cout << "file " << f->GetName() << " doesn't exist" << std::endl;
        flag = true;
    }
    return flag;
}

void ApplySolidAngleCorrections(TFile *f, std::string corrections_fname)
{

    if (FileIsBad(f)) return;

    SolidAngleLoader sal(corrections_fname);
    std::cout << sal.GetSAForDets("parallel").size() << std::endl;
    std::cout << sal.GetSAForDets("perpendicular").size() << std::endl;

    TCanvas* c=0;
    TDirectory *dir=0;
    TObject *obj=0;
    TGraphErrors *gr0=0, *gr1=0;
    TGraphErrors *gr_new0=0, *gr_new1=0;
    TGraphErrors *gr_new0_trim=0, *gr_new1_trim=0;

    std::string dirname("uncorrected");
    if ( (dir = f->GetDirectory(dirname.data())) )
    {
        obj = dir->Get((dirname + "_phi0_yield").data());
        if (obj!=NULL) gr0 = static_cast<TGraphErrors*>(obj);

        obj = dir->Get((dirname + "_phi90_yield").data());
        if (obj!=NULL) gr1 = static_cast<TGraphErrors*>(obj);
    }
    else
    {
        std::cout << "uncorrected graphs are not in the uncorrected directory"
                << std::endl;
    }

    gr_new0 = CreateCorrectedGraph(gr0, sal.GetSAForDets("parallel"));
    gr_new0->SetName("sa_corrected_phi0_yield");
    gr_new0->SetLineColor(kRed);
    gr_new0->SetMarkerColor(kRed);
    gr_new0->SetMarkerStyle(20);

    gr_new0_trim = TrimTheZeroes(gr_new0);
    gr_new0_trim->SetName("sa_corrected_phi0_yield_trimmed");
    gr_new0_trim->SetLineColor(kRed+2);
    gr_new0_trim->SetMarkerColor(kRed+2);
    gr_new0_trim->SetMarkerStyle(20);

    gr_new1 = CreateCorrectedGraph(gr1, sal.GetSAForDets("perpendicular"));
    gr_new1->SetName("sa_corrected_phi90_yield");
    gr_new1->SetLineColor(kGreen);
    gr_new1->SetMarkerColor(kGreen);
    gr_new1->SetMarkerStyle(20);

    gr_new1_trim = TrimTheZeroes(gr_new1);
    gr_new1_trim->SetName("sa_corrected_phi90_yield_trimmed");
    gr_new1_trim->SetLineColor(kGreen+2);
    gr_new1_trim->SetMarkerColor(kGreen+2);
    gr_new1_trim->SetMarkerStyle(20);

    dir = f->GetDirectory("corrected");
    if (dir==NULL)
    {
       dir = f->mkdir("corrected");
    }
    dir->cd();

    std::cout << "In " << f->GetName()
            << "/" << dir->GetName() << std::endl;
    std::cout << "\t" << gr_new0->GetName() << std::endl;
    gr_new0->Write("",TObject::kOverwrite);
    std::cout << "\t" << gr_new1->GetName() << std::endl;
    gr_new1->Write("",TObject::kOverwrite);

    std::cout << "\t" << gr_new0_trim->GetName() << std::endl;
    gr_new0_trim->Write("",TObject::kOverwrite);
    std::cout << "\t" << gr_new1_trim->GetName() << std::endl;
    gr_new1_trim->Write("",TObject::kOverwrite);

    f->cd();

}


void PrintToStream(std::ostream& stream,
                   std::vector<Double_t> vec0,
                   std::vector<Double_t> vec1,
                   std::vector<Double_t> vec2,
                   std::vector<Double_t> vec3)
{
    stream << std::setiosflags(std::ios::scientific)
            << std::right
            << std::setprecision(2);
    stream << std::setw(13) << "yield"
            << std::setw(13) << "uyield"
            << std::setw(13) << "corr_yield"
            << std::setw(13) << "ucorr_yield"
            << "\n"
            << std::setfill('-')
            << std::setw(52) << '-'
            << std::setfill(' ');

    for (UInt_t i=0; i<vec2.size(); i++)
    {

        stream << "\n"
                << std::setw(13) << vec0[i]
                << std::setw(13) << vec1[i]
                << std::setw(13) << vec2[i]
                << std::setw(13) << vec3[i];
    }

    stream << std::resetiosflags(std::ios::scientific)
            << std::right << std::setprecision(6)
            << std::endl;

}

void PrintUncorrectedAndCorrectedResultsToFile(std::string results_file,
                                               std::string sa_corr_file,
                                               std::string out_fname)
{

    ColumnFileLoader resfile; resfile.LoadData(results_file);
    ColumnFileLoader safile; safile.LoadData(sa_corr_file);

    ColumnData theta = resfile.GetColumnForDet(0,0);
    ColumnData yield = resfile.GetColumnForDet(0,1);
    ColumnData utheta = resfile.GetColumnForDet(0,2);
    ColumnData uyield = resfile.GetColumnForDet(0,3);
    ColumnData phi = resfile.GetColumnForDet(0,4);
    ColumnData uphi = resfile.GetColumnForDet(0,5);

    ColumnData corr = safile.GetColumnForDet(0,0);
    ColumnData ucorr = safile.GetColumnForDet(0,1);

    ColumnData res(yield.size(),0);
    ColumnData ures(yield.size(),0);

//    std::vector<Double_t> in(res.size(),0);
//    for (UInt_t i=0; i<in.size(); i++) in[i] = i;

    using TMath::Power;
    using TMath::Sqrt;
    for (UInt_t i=0; i<yield.size(); i++)
    {
        if (corr[i]>1.0e-10)
        {
            res[i] = yield[i]/corr[i];
            if (yield[i]>1.0e-10)
                ures[i] = res[i]*Sqrt(Power(uyield[i]/yield[i],2.0)+Power(ucorr[i]/corr[i],2.0));
            else
                ures[i] = 0;
        }
        else
        {
            res[i]=0;
            ures[i]=0;
        };
    }

    VectorWriter vp;
    std::ofstream ofile(out_fname.data(), std::ios::out);

    vp.RegisterVector("theta",theta);
    vp.RegisterVector("yield",yield);
    vp.RegisterVector("utheta",utheta);
    vp.RegisterVector("uyield",uyield);
    vp.RegisterVector("phi",phi);
    vp.RegisterVector("uphi",uphi);
    vp.RegisterVector("sa_corr",corr);
    vp.RegisterVector("usa_corr",ucorr);
    vp.RegisterVector("corr_yield", res);
    vp.RegisterVector("ucorr_yield",ures);
    vp.Write(ofile);

    ofile.close();
}

void PrintSortedCorrectedAndUncorrectedResultsToFile(std::string results_file,
                                                     std::string sorted_fname,
                                                     std::string ang_fname="angle_data/angles")
{
    ColumnFileLoader resfile;
    resfile.SetNPerDet(16);
    resfile.LoadData(results_file);

    ColumnFileLoader angfile;
    angfile.SetNPerDet(16);
    angfile.LoadData(ang_fname);

    ColumnData para_yield = resfile.GetColumnForDets("parallel",1);
    ColumnData perp_yield = resfile.GetColumnForDets("perpendicular",1);
    ColumnData para_uyield = resfile.GetColumnForDets("parallel",2);
    ColumnData perp_uyield = resfile.GetColumnForDets("perpendicular",2);


    ColumnData para_corr = resfile.GetColumnForDets("parallel",3);
    ColumnData perp_corr = resfile.GetColumnForDets("perpendicular",3);
    ColumnData para_ucorr = resfile.GetColumnForDets("parallel",4);
    ColumnData perp_ucorr = resfile.GetColumnForDets("perpendicular",4);

    ColumnData para_theta = angfile.GetColumnForDets("parallel",0);
    ColumnData para_utheta = angfile.GetColumnForDets("parallel",1);
    ColumnData para_phi = angfile.GetColumnForDets("parallel",2);
    ColumnData para_uphi = angfile.GetColumnForDets("parallel",3);

    ColumnData perp_theta = angfile.GetColumnForDets("perpendicular",0);
    ColumnData perp_utheta = angfile.GetColumnForDets("perpendicular",1);
    ColumnData perp_phi = angfile.GetColumnForDets("perpendicular",2);
    ColumnData perp_uphi = angfile.GetColumnForDets("perpendicular",3);

    ofstream ofile;
    ofile.open(sorted_fname.data(), std::ios::out);
    if (ofile.fail())
    {
        std::cerr << sorted_fname << " doesn't exist" << std::endl;
        return;
    }

    VectorSorter vs;
    vs.RegisterVector(para_theta);
    vs.RegisterVector(para_utheta);
    vs.RegisterVector(para_phi);
    vs.RegisterVector(para_uphi);
    vs.RegisterVector(para_yield);
    vs.RegisterVector(para_uyield);
    vs.RegisterVector(para_corr);
    vs.RegisterVector(para_ucorr);
    vs.Sort(0);

    VectorSorter vs2;
    vs2.RegisterVector(perp_theta);
    vs2.RegisterVector(perp_utheta);
    vs2.RegisterVector(perp_phi);
    vs2.RegisterVector(perp_uphi);
    vs2.RegisterVector(perp_yield);
    vs2.RegisterVector(perp_uyield);
    vs2.RegisterVector(perp_corr);
    vs2.RegisterVector(perp_ucorr);
    vs2.Sort(0);


    VectorWriter vp;
    ofile << "Begin parallel data" << endl;
    ofile << setfill('-');
    ofile << setw(60) << '-'
            << setfill(' ')
            << endl;

    vp.RegisterVector("theta",vs.GetSortedVector(0));
    vp.RegisterVector("utheta",vs.GetSortedVector(1));
    vp.RegisterVector("phi",vs.GetSortedVector(2));
    vp.RegisterVector("uphi",vs.GetSortedVector(3));
    vp.RegisterVector("yield",vs.GetSortedVector(4));
    vp.RegisterVector("uyield",vs.GetSortedVector(5));
    vp.RegisterVector("corr",vs.GetSortedVector(6));
    vp.RegisterVector("ucorr",vs.GetSortedVector(7));
    vp.Write(ofile);

    ofile << setfill('-');
    ofile << setw(60) << '-'
            << setfill(' ')
            << endl;
    ofile << "Begin perpendicular data" << endl;
    ofile << setfill('-');
    ofile << setw(60) << '-'
            << setfill(' ')
            << endl;

    vp.Clear();
    vp.RegisterVector("theta", vs2.GetSortedVector(0));
    vp.RegisterVector("utheta",vs.GetSortedVector(1));
    vp.RegisterVector("phi",vs.GetSortedVector(2));
    vp.RegisterVector("uphi",vs.GetSortedVector(3));
    vp.RegisterVector("yield",vs2.GetSortedVector(4));
    vp.RegisterVector("uyield",vs2.GetSortedVector(5));
    vp.RegisterVector("corr",vs2.GetSortedVector(6));
    vp.RegisterVector("ucorr",vs2.GetSortedVector(7));
    vp.Write(ofile);
    ofile.close();

}
