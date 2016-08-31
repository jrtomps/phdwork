
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <set>
#include <exception>
#include <dirent.h>
#include <functional>
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "Analyzer.h"
#include "SiStripGenerator.h"
#include "AngleAverager.h"
#include "SolidAngleComputer.h"
#include "EnergyHist.h"
#include "EnergyHistSorted.h"
#include "YieldGenerator.h"
#include "BoundaryCreator.h"
#include <boost/regex.hpp>
#include "RatioVsOrigin.h"
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
        string file = "test.root";
        TFile fout(file.data(), "UPDATE");

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
			std::ostringstream fn;
			fn << indir << "/" << ifname;
            tr->AddFile(fn.str().data());
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

            if (ifname.at(ifname.length()-1)!='_')  ifname.append("_");
            std::ostringstream fn;
            for (Int_t i=first; i<=last; i++)
            {
                fn.clear(); fn.str("");
                fn << ifname << i << ".root";
                tr->AddFile(fn.str().data());
            }
        }

        Int_t entries=TChain::kBigNumber;
        std::cout << "Do you want to process the entire tree? (y/n) : ";
        std::cin >> ans;
        if (ans!='y' && ans!='Y')
        {
            std::cout << "Enter number to process : ";
            std::cin >> entries;
        }
//        TTree *tr   = dynamic_cast<TTree*>(fin.Get("Sim_Tree"));

        AngleAverager     *aa = new AngleAverager(tr, &fout, tr->GetFile(), 7.1);
        SolidAngleComputer *sac = new SolidAngleComputer(tr, tr->GetFile());
        EnergyHist       *eh = new EnergyHist(tr, &fout, 7.1, 9.0);
        RatioVsOrigin       *rvo = new RatioVsOrigin(tr, &fout, 7.1, 9.0);
        YieldGenerator       *yg = new YieldGenerator(tr, &fout);
        EnergyHistSorted       *ehs= new EnergyHistSorted(tr, &fout);

        Analyzer *an = new Analyzer;
        an->AddFunction(aa);
        an->AddFunction(sac);
//        an->AddFunction(eh);
//		an->AddFunction(rvo);
//        an->AddFunction(yg);
//        an->AddFunction(ehs);

        tr->Process(an,"",entries);

		eh->WriteIntegralsToFile("g4_sa_corr_232Th.dat",7.1, 9.0);

        fout.Close();
        delete an;

		std::cout << "\nFinished analyzing " << ifname << std::endl;
    }
    catch (exception& e)
    {

        std::cout << "\r" << e.what() << std::endl;
    }
    return 0;
}

