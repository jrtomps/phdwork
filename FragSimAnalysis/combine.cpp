#include <iostream>
#include "TFileMerger.h"
#include "DataBucket.h"
#include "ROOTUtils.h"
#include <string>
#include <vector>
#include <dirent.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
using namespace std;


int
main ()
{

    string base_pattern;
    cout << "What is the pattern of the file base? ";
    cin >> base_pattern;

    TFileMerger merger;
    vector<string> files;


    // base_name = /home/jrtomps/some/script
    //
    // dir = /home/jrtomps/some
    // base_name = script
    string dir(base_pattern);
    size_t in1 = dir.rfind('/');
    if (in1!=string::npos)
    {
        dir.erase(in1,base_pattern.length()-in1);
        base_pattern.erase(0,dir.length()+1);
    }

    if (dir==base_pattern) dir = ".";

    struct dirent *dirp;
    DIR *dp = opendir(dir.data());
    if (dp==NULL)
    {
        std::cerr << "Error opening present directory" << std::endl;
        return -1;
    }
    else
    {
        while((dirp=readdir(dp))!=NULL)
        {
            files.push_back(string(dirp->d_name));
        }
    }

    Double_t fcount =0;
    for (UInt_t i=0; i<files.size(); i++)
    {
        if (files[i].find(base_pattern)!=string::npos)
        {
            merger.AddFile(files[i].data());
            fcount++;
        }
    }

    std::string outfname = base_pattern + ".root";
    //merger.PrintFiles();
    merger.SetPrintLevel(3);
    merger.OutputFile(outfname.data(),"RECREATE");
    merger.Merge();

    TFile* f = new TFile(outfname.data(), "update");
    vector<TDirectory*> all_dirs = ROOTUtils::GetAllTDirectoriesInDirectory(f);
    all_dirs.push_back(f);

    for (UInt_t i=0; i<all_dirs.size(); i++)
    {
        all_dirs[i]->cd();

        std::cout << "Merging " << all_dirs[i]->GetName() << std::endl;
        vector<TH1*> all_h = ROOTUtils::GetAllTH1InDirectory(all_dirs[i]);
        for (UInt_t j=0; j<all_h.size(); j++)
        {
            all_h[j]->Scale(1.0/fcount);
            all_dirs[i]->cd();
            all_h[j]->Write("",TObject::kOverwrite);
        }

//        f->cd();
        vector<TH2*> all_h2 = ROOTUtils::GetAllTH2InDirectory(all_dirs[i]);
        for (UInt_t j=0; j<all_h2.size(); j++)
        {
            all_h2[j]->Scale(1.0/fcount);
//            all_dirs[i]->cd();
            all_h2[j]->Write("",TObject::kOverwrite);
        }

        f->cd();
        vector<TH3*> all_h3 = ROOTUtils::GetAllFromDirectory<TH3>(all_dirs[i]);
        for (UInt_t j=0; j<all_h3.size(); j++)
        {
            all_h3[j]->Scale(1.0/fcount);
  //          all_dirs[i]->cd();
            all_h3[j]->Write("",TObject::kOverwrite);
        }
    }
    f->Close();

    return 0;
}
