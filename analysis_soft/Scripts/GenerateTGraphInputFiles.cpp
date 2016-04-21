#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "TGraphErrors.h"
#include "ColumnFileLoader.h"
#include "TFile.h"
#include "TObject.h"
#include "TClass.h"
using namespace std;

bool verbose = true;

struct PointData_t {
    Double_t x;
    Double_t y;
    Double_t ex;
    Double_t ey;
};


bool PointDataSorter(PointData_t p1, PointData_t p2)
{
    return (p1.x < p2.x ? true : false);
}

ColumnData ReadInBlackListedChannels(const string& fname)
{
    ColumnFileLoader cfl;
    cfl.LoadData(fname);
    return cfl.GetColumnForDet(0,0);
}

Bool_t IsBlacklisted(Int_t index, ColumnData& blacklist)
{
    Double_t val = index;
    return std::binary_search(blacklist.begin(), blacklist.end(), val);
}

void GenerateTGraphInputFiles(const string& yield_fname,
                              const string& ang_fname,
                              const string& output_fname,
                              const string& blacklist_fname="")
{

    ColumnData blacked_ch;
    if ( !blacklist_fname.empty() )
        blacked_ch = ReadInBlackListedChannels(blacklist_fname);


    stringstream ssyield, ssang;
    string yield_line, ang_line;

    ifstream yield_file(yield_fname.data(), ios::in);
    if (!yield_file.is_open())
    {
        cout << yield_fname << " doesn't exist!" << endl;
        return;
    }

    // skip over file header in yield file
    unsigned int n_yield_lines_to_skip = 4;
    //    for (unsigned int i=0; i<n_yield_lines_to_skip; i++)
    std::string aline;
    do {
        std::getline(yield_file,aline);
    } while ( aline.find("-------")==std::string::npos );
    //  std::getline(yield_file, aline);
    //    yield_file.ignore(1000,'\n');
    std::cout << aline << std::endl;

    ifstream ang_file(ang_fname.data(), ios::in);
    if (!ang_file.is_open())
    {
        cout << ang_fname << " doesn't exist!" << endl;
        return;
    }

    // skip over file header in ang file
    unsigned int n_ang_lines_to_skip = 3;
    for (unsigned int i=0; i<n_ang_lines_to_skip; i++)
        ang_file.ignore(1000,'\n');

    ofstream out_file(output_fname.data(), ios::out);
    ios::fmtflags outfmt = ios::left | ios::scientific;
    out_file << setiosflags(outfmt);
    out_file.precision(2);
    out_file.width(10);

    out_file << setw(20) << "#Yield data from "
            << yield_fname << endl;
    out_file << setw(20) << "#Angle data from "
            << ang_fname << endl;
    out_file << "#"
            << setw(10) << "theta"
            << setw(10) << "intgrl"
            << setw(10) << "utheta"
            << setw(10) << "uintgrl"
            << setw(10) << "phi"
            << setw(10) << "uphi"
            << endl;
    //            << setw(39) << setfill('-')
//            << "-" << endl;
//    out_file << setfill(' ');

    int strip_index, nlines_read=0, index=0;
    double low, high, integral, uintegral, theta, utheta, phi, uphi;
    char plusminus[3];


    TGraphErrors *gr[2], *grp;
    for (int i=0; i<2; i++)
    {
        gr[i] = new TGraphErrors(32);
        gr[i]->SetMarkerColor(i+2);
        gr[i]->SetMarkerStyle(20);
        gr[i]->SetLineColor(i+2);
        gr[i]->SetName(Form("uncorrected_phi%i_yield",i*90));
    }

    vector<vector<PointData_t> > data_points;
    vector<PointData_t> agr0, agr1;
    data_points.push_back(agr0);
    data_points.push_back(agr1);

    int theta_index_map[4] = {16,0,16,0};
    int phi_index_map[4]   = {1,0,0,1};
    int n [4] = {0,0,0,0};

    while (yield_file.good() && ang_file.good())
    {
        getline(yield_file, yield_line);
        getline(ang_file, ang_line);


        if (yield_file.rdstate()!=0)
	{
            if (!yield_file.eof())
                cout << yield_fname
                        << " has nonzero state bit. Aborting" << endl;
            break;
	}
        if (ang_file.rdstate()!=0)
	{
            if (!ang_file.eof())
                cout << ang_fname
                        << " has nonzero state bit. Aborting" << endl;
            break;
	}
        if (yield_line.find("Total") != string::npos)
            break;

        ssyield.clear();

        // Set the yield file stringstream to the current line
        ssyield.str(yield_line);

        size_t in = ssyield.str().find("inf");
        if (in!=string::npos)
	{
            cout << "found inf on line " << nlines_read << endl;
            string cstr = ssyield.str().replace(in,3," 0");
            ssyield.str(cstr);
	}

        in = ssyield.str().find("nan");
        if (in!=string::npos)
        {
            cout << "found nan on line " << nlines_read << endl;
            string cstr = ssyield.str().replace(in,3," 0");
            ssyield.str(cstr);
        }

        //      cout << ssyield.str() << endl;
        in = ssyield.str().find("+-");
        if (in!=string::npos)
	{
            ssyield >> strip_index >> low >> high
                    >> integral >> plusminus >> uintegral;
	}
        else
	{
            ssyield >> strip_index >> low >> high
                    >> integral >> uintegral;
	}
        //      cout << endl; cout << "\tbad=" << ssyield.bad() << " fail=" << ssyield.fail() << " eof=" << ssyield.eof() << endl; cout.flush();

        //      cout << setw(4) << strip_index
        //       	   << setw(10) << low
        //       	   << setw(10) << high
        //       	   << setw(10) << integral
        //       	   << setw(10) << uintegral;

        ssang.clear();
        ssang.str(ang_line);
        //      cout << ang_line << " : " << ssang.str() << endl;
        ssang >> theta >> utheta >> phi >> uphi;
        // cout << setw(10) << theta
        // 	   << setw(10) << utheta
        // 	   << endl;

        //      out_file.clear();
        out_file << setw(10) << theta
                << setw(10) << integral
                << setw(10) << utheta
                << setw(10) << uintegral
                << setw(10) << phi
                << setw(10) << uphi
                << endl;

        PointData_t apoint;
        apoint.x = theta;
        apoint.y = integral;
        apoint.ex = utheta;
        apoint.ey = uintegral;

        if (! IsBlacklisted(nlines_read, blacked_ch) && integral>0)
        {
            index = nlines_read/16;
            data_points[phi_index_map[index]].push_back(apoint);
        }
        nlines_read++;
    } // end while

    stable_sort(data_points[0].begin(), data_points[0].end(), PointDataSorter);
    stable_sort(data_points[1].begin(), data_points[1].end(), PointDataSorter);

    index = 0;
    std::vector<PointData_t>::iterator it;
    gr[0]->Set(data_points[0].size());
    for (it=data_points[0].begin(); it!=data_points[0].end(); it++, index++)
    {
        gr[0]->SetPoint(index, it->x, it->y);
        gr[0]->SetPointError(index, it->ex, it->ey);
    }

    index = 0;
    gr[1]->Set(data_points[1].size());
    for (it=data_points[1].begin(); it!=data_points[1].end(); it++, index++)
    {
        gr[1]->SetPoint(index, it->x, it->y);
        gr[1]->SetPointError(index, it->ex, it->ey);
    }

    cout << "Read " << nlines_read << " lines " << endl;
    out_file.close();
    yield_file.close();
    ang_file.close();

    string root_fname = output_fname;
    size_t is = root_fname.rfind(".");
    root_fname.replace(is, output_fname.length()-is, ".root");
    cout << "Outputting corrected data to file " << root_fname << std::endl;
    TFile *froot = new TFile(root_fname.data(),"UPDATE");
    froot->cd();
    TDirectory *obj = froot->GetDirectory("uncorrected");
    if (obj==NULL)
        obj = froot->mkdir("uncorrected","uncorrected");

    obj->Write();
    obj->cd();

    for (int i=0; i<2; i++) gr[i]->Write("",TObject::kOverwrite);
    froot->Close();
    //  return 0;
};
