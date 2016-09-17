
#include <iostream>
#include <string>
#include <cstdlib>
#include <set>
#include <stdexcept>
#include <dirent.h>
#include <functional>
#include "TFile.h"
#include "TTree.h"
#include "Analyzer.h"
#include "AngleAverager.h"
#include "SolidAnglePDFGeneratorC.h"
#include "LambdaGenerator.h"
#include "EtaDbFiller2C.h"
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

std::string getEnv(std::string varName) {
    if (getenv(varName.c_str())==NULL)
    {
        string msg = varName + " variable not set";
        throw std::runtime_error(msg);
    }
    else
        return getenv(varName.c_str());
}

int
main ()
{
    string indir = getEnv("SIM_OUT_DIR");
    string outdir = getEnv("PHD_ANALYSIS_DIR");

    try
    {
        string file = outdir + "/h_eta.root";
        TFile fout(file.data(), "UPDATE");
        
        string file2 = outdir + "/lambda_eta.root";
        TFile fout2(file2.data(), "UPDATE");

        string file3 = outdir + "/angles_eta.root";
        TFile fout3(file3.data(), "UPDATE");
  
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
                fn << indir << "/" << ifname << i << ".root";
                tr->AddFile(fn.str().data());
            }
        }


//        AngleAverager     *aa = new AngleAverager(tr, &fout3, &fin, 7.9);
        SolidAnglePDFGeneratorC  *sapg = new SolidAnglePDFGeneratorC(tr, &fout, 9.3);
        LambdaGenerator  *lg = new LambdaGenerator(tr, &fout2, 9.3);
        EtaDbFiller2C    *edbf = new EtaDbFiller2C(tr, 9.3);

        Analyzer *an = new Analyzer;
//        an->AddFunction(aa);
        an->AddFunction(sapg);
        an->AddFunction(lg);
        an->AddFunction(edbf);


        tr->Process(an);
   //     fin.Close();
        fout.Close();
        fout2.Close();
        delete an;

    }
    catch (exception& e)
    {

        std::cout << "\r" << e.what() << std::endl;
    }
    return 0;
}

