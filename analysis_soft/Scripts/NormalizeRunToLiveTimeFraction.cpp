// NormalizeRunToVetoClock.cpp
//
// Jeromy Tompkins
// 7/20/2011
//


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "TObject.h"
#include "TKey.h"
#include "TFile.h"
#include "TList.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "FNameManager.h"
#include "TH1.h"
#include "ROOTUtils.h"

using namespace std;
FNameManager fnm("/home/jrtomps/pffrag2/analysis/fnameconfig.dat");

string expbase     = fnm.GetBaseDir();//"/home/jrtomps/pffrag1";
string sclr_dir    = fnm.GetSclrDir();//expbase + "/scalers";
string normed_file = fnm.GetHistFileDir() + "/normed_hist_run";
string configfile  = fnm.GetConfigFileName();

ofstream logfile("normalization_logfile.txt",ifstream::out);


void NormalizeRunToLiveTimeFraction(const Int_t run, const TString& histname)
{
    clog.rdbuf(logfile.rdbuf());
    clog << "\nRun : " << run
            << " Skipped hists list" << endl;

    ConfigManager cm;
    cout << configfile << endl;
    cm.LoadDbFromFile(configfile.data());
    if (cm.GetRun(run)==NULL)
    {
        cerr << "Run doesn't exist in the config database"<< endl;
        return;
    }

    stringstream ss;
    ss << normed_file << run << ".root";

    TFile f(ss.str().data(),"UPDATE");
    cout << "Reading " << ss.str() << endl;
    TDirectory *dir = f.GetDirectory(histname.Data());
    if (dir==NULL)
    {
        cout << histname.Data() <<" is not a directory" << endl;
        return;
    }
    else
    {
        dir->cd();
    }
    TString pattern_name = histname;
    pattern_name += "[0-9]\+_norm_1pad$";
    pattern_name.Prepend('^');
    vector<TH1*> vec = ROOTUtils::GetAllTH1InFile(dir,pattern_name);
    // the order isn't important
    //ROOTUtils::EnforceProperOrdering(vec);

    cout << "Found " << vec.size() << " histograms" << endl;

    stringstream ss1;
    ss1 << normed_file << run << ".root";
    //TFile normf(ss1.str().data(),"UPDATE");

    Float_t val = cm.GetRun(run)->GetLiveTime();
    Float_t val2 = cm.GetRun(run)->GetRunDuration();

    if (val2!=0)
        val /= val2;
    else
        val = 0;

    cout << "\nNormalization by Live Time Fraction(" << val << ")" << endl;

    vector<TH1*>::iterator it;
    for (it=vec.begin(); it!=vec.end(); it++)
    {
        if (val!=0)
            (*it)->Scale(1.0/val);
        else
        {
            clog << (*it)->GetName() << ", ";
            continue;
        }

        string name = (*it)->GetName();
        if ((name.rfind("_norm_ltf")==string::npos) || (name.rfind("_norm_ltf")==name.length()-9))
            name.append("_norm_ltf");

        (*it)->SetName(name.data());
//        cout << "Writing " << name << " to file " << endl;
        name = (*it)->GetTitle();
        name.append(" Normed to LiveTimeFraction");
        (*it)->SetTitle(name.data());
 //       normf.cd();
        (*it)->Write();
    }
    cout << "Normalized histograms saved in " << ss1.str() << endl;

    //  return 0;
//    normf.Close();
}


void NormalizeAllRunsToLiveTimeFraction(const TString& histname)
{

    ConfigManager cm;
    cm.LoadDbFromFile(configfile.data());
    std::vector<Int_t> runlist = cm.GetListOfRuns();
    //  if (cm.GetRun(run)==NULL) {cerr << "no configdb data" << endl; return;}

    std::vector<Int_t>::iterator it;
    for (it = runlist.begin(); it!= runlist.end(); it++)
    {
        NormalizeRunToLiveTimeFraction(*it, histname);
    }

}
