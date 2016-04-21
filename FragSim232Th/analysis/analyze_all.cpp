
#include <iostream>
#include <string>
#include <cstdlib>
#include <set>
#include <exception>
#include <dirent.h>
#include <functional>
#include "TFile.h"
#include "TTree.h"
#include "Analyzer.h"
#include "AngleAverager.h"
#include "SolidAnglePDFGeneratorC.h"
#include "LambdaGenerator.h"
#include "EtaDbFiller2C.h"
#include "AngleAverager.h"
#include "SolidAngleComputer.h"
#include "EnergyHist.h"
#include "EnergyHistSorted.h"
#include "YieldGenerator.h"
#include "RatioVsOrigin.h"
#include <boost/regex.hpp>
using namespace std;

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

int
main ()
{
    string indir;

    if (getenv("SIM_OUT_DIR")==NULL)
    {
        std::cout << "SIM_OUT_DIR variable not set"
                  << "\n> source setup.sh "
                  << std::endl;
        return -1;
    }
    else
        indir = getenv("SIM_OUT_DIR");

    try
    {
       Char_t ans;
        TChain *tr = new TChain("Sim_Tree");

        std::cout << "Do you want to run multiple files? ";
        std::cin >> ans;

        std::string ifname;
        if (std::cin.peek()=='\n') std::cin.get();
        if (ans=='n')
        {
            cout << "\nIn directory = " << indir << endl;
            ListAllFilesInDirOfType(indir,".root");
            cout << "\nEnter file name containing TTree : ";
            getline(cin,ifname);
			      indir += "/";
            ifname.insert(0, indir);
            tr->AddFile(ifname.data());
        }
        else
        {
            Int_t first, last;
            cout << "\nEnter base name : ";
            getline(cin, ifname);
            cout << "Enter first index : ";
            cin >> first;
            cout << "Enter last index :  ";
            cin >> last;

            std::ostringstream fn;
            for (Int_t i=first; i<=last; i++)
            {
                fn.clear(); fn.str("");
                fn << ifname << i << ".root";
                tr->AddFile(fn.str().data());
            }
        }
        
        std::string file = "h_eta.root";
        TFile fout(file.data(), "UPDATE");
        
        string file2 = "lambda_eta.root";
        TFile fout2(file2.data(), "UPDATE");

        file = "test.root";
        TFile fout0(file.data(), "UPDATE");
        
		SolidAnglePDFGeneratorC  *sapg = new SolidAnglePDFGeneratorC(tr, &fout, 9.3);
        LambdaGenerator  *lg = new LambdaGenerator(tr, &fout2, 9.3);
        EtaDbFiller2C    *edbf = new EtaDbFiller2C(tr, 9.3);
        AngleAverager     *aa = new AngleAverager(tr, &fout0, tr->GetFile(), 7.1);
        SolidAngleComputer *sac = new SolidAngleComputer(tr, tr->GetFile());
        EnergyHist       *eh = new EnergyHist(tr, &fout0, 7.1, 9.0);
        RatioVsOrigin       *rvo = new RatioVsOrigin(tr, &fout0, 7.1, 9.0);
        YieldGenerator       *yg = new YieldGenerator(tr, &fout0);
        EnergyHistSorted       *ehs= new EnergyHistSorted(tr, &fout0);




        Analyzer *an = new Analyzer;
        an->AddFunction(sapg);
        an->AddFunction(edbf);
        an->AddFunction(aa);
        an->AddFunction(sac);
        an->AddFunction(eh);
		an->AddFunction(rvo);
        an->AddFunction(yg);
        an->AddFunction(ehs);

        tr->Process(an);
   //     fin.Close();
        eh->WriteIntegralsToFile("g4_sa_corr_232Th.dat",7.1, 9.0);
    
        fout.Close();
        fout0.Close();
        fout2.Close();
        delete an;
        std::cout << "\nFinished analyzing " << ifname << std::endl;

    }
    catch (exception& e)
    {

        std::cout << "\r" << e.what() << std::endl;
    }
    return 0;
}

