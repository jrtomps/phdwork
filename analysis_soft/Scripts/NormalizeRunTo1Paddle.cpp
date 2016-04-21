// NormalizeRunTo5Paddle.cpp
//
// Jeromy Tompkins
// 7/14/2011
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "TObject.h"
#include "TKey.h"
#include "TFile.h"
#include "TList.h"
#include "ScalerTool.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "FNameManager.h"
#include "TH1.h"
#include "ROOTUtils.h"
using namespace std;

FNameManager fnm("/home/jrtomps/pffrag2/analysis/fnameconfig.dat");

string expbase     = fnm.GetBaseDir();
string sclr_dir    = fnm.GetSclrDir(true);
string normed_file = fnm.GetHistFileDir(true) + "/normed_hist_run";
string configfile  = fnm.GetConfigFileName(true);

void NormalizeRunTo1Paddle(const Int_t run, const TString& histname)
{

    cout << fnm.GetSclrFileName(true) << endl;

    ScalerTool st(fnm.GetSclrFileName(true).data());

    stringstream ss;
    ss << fnm.GetHistFileBase(true) << run << ".root";
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
    vector<TH1*> vec = ROOTUtils::GetAllTH1InFile(dir);
    // the order is not important
    //ROOTUtils::EnforceProperOrdering(vec);
    cout << "Found " << vec.size() << " histograms" << endl;

    stringstream ss1;
    ss1 << normed_file << run << ".root";
    TFile normf(ss1.str().data(),"UPDATE");
    TDirectory *normdir = normf.GetDirectory(histname.Data());
    if (normdir==NULL)
    {
        normdir = normf.mkdir(histname.Data());
    }
    normdir->cd();

    vector<TH1*>::iterator it;
    Int_t val = st.GetValue(run,14);
    cout << endl;
    cout << "Normalization by 1Pad/100 = " << val << endl;
    for (it=vec.begin(); it!=vec.end(); it++)
    {
        if (val!=0)
            (*it)->Scale(0.01/val);
        else
            continue;

        string name = (*it)->GetName();
        name.append("_norm_1pad");
        (*it)->SetName(name.data());
        name = (*it)->GetTitle();
        name.append(" Normed to 1Pad");
        (*it)->SetTitle(name.data());
    //    normf.cd();
        normdir->cd();
        (*it)->Write();
    }
    cout << "Normalized histograms saved in " << normed_file << endl;

    //  return 0;

}


void NormalizeAllRunsTo1Paddle(const TString&  histname)
{

    ConfigManager cm;
    cm.LoadDbFromFile(configfile.data());
    std::vector<Int_t> runlist = cm.GetListOfRuns();
    //  if (cm.GetRun(run)==NULL) {cerr << "no configdb data" << endl; return;}

    std::vector<Int_t>::iterator it;
    for (it = runlist.begin(); it!= runlist.end(); it++)
    {
        NormalizeRunTo1Paddle(*it, histname);
    }

}
