

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include "TDirectory.h"
#include "TH1.h"
#include "TMath.h"
#include "ROOTUtils.h"

std::ostream& __stream = std::cout;
TFile* hist_file = 0;

struct data_point
{
    Double_t v;
    Double_t uv;
};

std::vector<data_point> ComputeAverageValueForHists(TDirectory* dir)
{
    std::vector<data_point> res;

    std::vector<TH1*> hists = ROOTUtils::GetAllTH1InDirectory(dir);
    ROOTUtils::EnforceProperOrdering(hists);

    Double_t integral=0;
    Double_t uintegral=0;
    for (UInt_t i=0; i<hists.size(); i++)
    {
        integral = hists[i]->IntegralAndError(1,hists[i]->GetNbinsX(),uintegral);

        Int_t nb=0;
        for (Int_t ii=1; ii<=hists[i]->GetNbinsX(); ii++)
        {
            if (hists[i]->GetBinContent(ii)!=0) nb++;
        }

        data_point dp;
        dp.v = integral/nb;
        dp.uv = uintegral/nb;

        res.push_back(dp);


    }

    return res;
}

Int_t CalcUncDigit(data_point& p)
{

    Int_t power;
    if (p.v<1)
        power = TMath::Log10(p.v)-1-2;
    else
        power = TMath::Log10(p.v)+2;

    return p.uv/TMath::Power(10,power);

}

void PrintMap(std::map<std::string, std::vector<data_point> >& mmap)
{
    std::map<std::string, std::vector<data_point> >::iterator it;
    std::vector<data_point>::iterator val_it;

    // Print out the header
    __stream << "\n" << std::setw(6) << ' ';
    for (it=mmap.begin(); it!=mmap.end(); it++)
    {
        __stream << std::setw(13) << it->first;
    }
    __stream << "\n"
            << std::setfill('=') << std::setw(6+13*mmap.size()) << '='
            << std::setfill(' ');

    // loop through the length of each vector
    for (UInt_t i=0; i<mmap.begin()->second.size(); i++)
    {

        // print the index
        __stream << "\n" << std::setw(6) << i;

        __stream << std::setiosflags(std::ios::scientific)
                << std::setprecision(2);
        // then print the values for each of the runs
        for (it=mmap.begin(); it!=mmap.end(); it++)
        {
            __stream << std::setw(10) << (it->second)[i].v;

            __stream << std::setw(10) << (it->second)[i].uv;

        }
        __stream << std::resetiosflags(std::ios::scientific)
                << std::setprecision(6);
    }

    __stream << std::endl;
}

void CompareDirectories(TDirectory* dir0, TDirectory* dir1=0, TDirectory* dir2=0)
{

    std::map<std::string, std::vector<data_point> > results;
    if (dir0!=0) results[dir0->GetName()] = ComputeAverageValueForHists(dir0);
    if (dir1!=0) results[dir1->GetName()] = ComputeAverageValueForHists(dir1);
    if (dir2!=0) results[dir2->GetName()] = ComputeAverageValueForHists(dir2);


    PrintMap(results);
}



