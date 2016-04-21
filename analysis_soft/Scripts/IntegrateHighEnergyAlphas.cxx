

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include "FNameManager.h"
#include "ConfigManager.h"
#include "ExtensibleDb.h"
#include "TDirectory.h"
#include "TH1.h"
#include "ROOTUtils.h"

std::ostream& __stream = std::cout;
TFile* hist_file = 0;
FNameManager fnm("fnameconfig.dat");
ConfigManager cm;

TDirectory* GetDirectoryInRunFile(std::string dir_name, const UInt_t run)
{
    TDirectory *dir = 0;

    std::ostringstream hist_fname;
    hist_fname << fnm.GetHistFileBase(true) << run << ".root";

    hist_file = new TFile(hist_fname.str().data(), "update");
    if (! hist_file->IsOpen())
    {
        std::cerr << "Unable to open " << hist_fname.str() << std::endl;
        return dir;
    }

    dir = hist_file->GetDirectory(dir_name.data());

    // the responsibility of determining whether this is null
    // is left to the function caller.
    return dir;
}

void LoadConfigDb()
{
    std::cout << "Using Config Databased in : "
            << fnm.GetConfigFileName(true)
            << std::endl;
    cm.LoadDbFromFile(fnm.GetConfigFileName(true));
}

std::vector<Double_t> IntegrateHighEnergyAlphasForRun(const UInt_t run)
{
    std::vector<Double_t> res;

    if (cm.GetDb().size()==0) LoadConfigDb();

    ConfigEntry *ce = cm.GetRun(run);
    if (ce==0)
    {
        std::cerr << "No data stored in configdatabase for run "
                << run << std::endl;
        return res;
    }
    Double_t dur = ce->GetLiveTime();


    ExtensibleDb* edb = ce->GetExtensibleDb();
    if (edb==0)
    {
        std::cerr << "No ExtensibleDb found for run "
                << run << std::endl;
        return res;
    }

    std::vector<Float_t> xlo, xhi;
    edb->GetValues("AlphaPeak",xlo);
    edb->GetValues("Threshold",xhi);
    if (xlo.size()==0 || xhi.size()==0)
    {
        std::cerr << "Missing integration range information" << std::endl;
        std::cerr << std::setw(20) << "AlphaPeak.size()" << " = " << xlo.size() << std::endl;
        std::cerr << std::setw(20) << "Threshold.size()" << " = " << xhi.size() << std::endl;
        return res;
    }

    std::string dname("adc");
    TDirectory* dir = GetDirectoryInRunFile(dname,run);
    if (dir==0)
    {
        std::cerr << "Unable to find " << dname << std::endl;
        return res;
    }

    std::vector<TH1*> hists = ROOTUtils::GetAllTH1InDirectory(dir);
    ROOTUtils::EnforceProperOrdering(hists);

    if (hists.size()>xlo.size() || hists.size()>xhi.size())
    {
        std::cerr << "More hists have been found than there are integration regions" << std::endl;
        return res;
    }


    Int_t binlo = 0;
    Int_t binhi = 0;
    for (UInt_t i=0; i<hists.size(); i++)
    {
        binlo = hists[i]->GetXaxis()->FindBin(xlo[i]);
        binhi = hists[i]->GetXaxis()->FindBin(xhi[i]);

        if (dur!=0)
            res.push_back(hists[i]->Integral(binlo, binhi)/dur);
        else
            res.push_back(0);
    }


    if (hist_file->IsOpen()) hist_file->Close();

    return res;
}

void PrintMap(std::map<UInt_t, std::vector<Double_t> >& mmap)
{
    std::map<UInt_t, std::vector<Double_t> >::iterator it;
    std::vector<Double_t>::iterator val_it;

    // Print out the header
    __stream << "\n" << std::setw(6) << ' ';
    for (it=mmap.begin(); it!=mmap.end(); it++)
    {
        __stream << std::setw(10) << it->first;
    }
    __stream << "\n"
            << std::setfill('=') << std::setw(6+10*mmap.size()) << '='
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
            __stream << std::setw(10) << (it->second)[i];
        }
        __stream << std::resetiosflags(std::ios::scientific)
                << std::setprecision(6);
    }

    __stream << std::endl;
}


std::vector<Double_t> SumUpMap(std::map<UInt_t, std::vector<Double_t> >& mmap)
{
    UInt_t n = mmap.begin()->second.size();
    std::vector<Double_t> res(n,0);

    std::map<UInt_t, std::vector<Double_t> >::iterator it;
    for (UInt_t i=0; i<n; i++)
    {
        Double_t sum = 0;
        for (it=mmap.begin(); it!=mmap.end(); it++)
        {
            sum += (it->second)[i];
        }
        res[i] = sum;
    }
    return res;
}



void CompareAllRunsForTarget(std::string tar_name, const Double_t eg, std::string pol)
{
    if (cm.GetDb().size()==0) LoadConfigDb();

    std::vector<Int_t> rlist = cm.GetListOfTargetRunsWithEnergyAndPol(tar_name.data(), eg, pol.data());

    std::map<UInt_t, std::vector<Double_t> > results;
    for (UInt_t i=0; i<rlist.size(); i++)
    {
        results[rlist[i]] = IntegrateHighEnergyAlphasForRun(rlist[i]);
    }

    std::vector<Double_t> sum = SumUpMap(results);
    results[100000] = sum;
    PrintMap(results);
}



