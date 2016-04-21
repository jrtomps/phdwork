#include "TH2.h"
#include "AngMarginalEta2C.h"
#include "TCanvas.h"
#include "Rtypes.h"
#include "TFile.h"
#include <iostream>
#include <string>
#include <set>
#include <dirent.h>
#include "TApplication.h"

void ListAllFilesInDirOfType(std::string dirname, std::string fsuffix)
{

    std::set<std::string> mset;

    ::DIR* dp = ::opendir(dirname.data());

    ::dirent* dir = 0;
    while ( (dir = ::readdir(dp)) )
    {
        std::string name(dir->d_name);
        if (name.rfind(fsuffix)!=std::string::npos)
        {
            mset.insert(name);
        }
    }

    std::set<std::string>::iterator it;
    for (it=mset.begin(); it!=mset.end(); it++)
    {
        std::cout << "\n\t" << *it;
    }

    std::cout << std::endl;
}

std::string GetROOTFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".root");
    std::cout << "\nEnter location of root file : ";
    std::getline(std::cin, fname);
    return fname;
}


void CompareAngMarginalEta2C(UInt_t z0, UInt_t a0, UInt_t z1, UInt_t a1)
{
    std::string fname = GetROOTFile();
    TFile* f = new TFile(fname.data());
    Eta2C eta(f);

    AngMarginalEta2C m0eta(eta,z0,a0);
    AngMarginalEta2C m1eta(eta,z1,a1);

    f->Close();

    const TH2* h0 = m0eta.GetHist();
    const TH2* h1 = m1eta.GetHist();

    TH2* hdiff = dynamic_cast<TH2*>(h0->Clone("hdiff"));

    hdiff->Add(h1,-1.0);

    hdiff->Draw("surf1");

}


#ifndef __CINT__
Int_t main(Int_t argc, Char_t* argv[])
{
    UInt_t z0 = ::atoi(argv[1]);
    UInt_t a0 = ::atoi(argv[2]);
    UInt_t z1 = ::atoi(argv[3]);
    UInt_t a1 = ::atoi(argv[4]);

    TApplication theapp("theapp",&argc,argv);

    CompareAngMarginalEta2C(z0,a0, z1,a1);

    theapp.Run(true);

    return 0;


}

#endif
