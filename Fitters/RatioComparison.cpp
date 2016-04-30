
#include "Rtypes.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <fstream>
#include <stdexcept>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "AngDistC.h"
#include "Parameters.h"


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

std::string GetParamFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".params");
    std::cout << "\nEnter location of params : ";
    std::getline(std::cin, fname);
    return fname;
}

AngDistC GetAngDist(std::string parfname)
{
    std::ifstream pfile(parfname.data(),std::ios::in);
    Parameters params(pfile);
    pfile.close();
    if (! params.KeysAreSensible()) throw std::runtime_error("Nonsensible parameters");
    AngDistC W(params);
    return W;
}


void RatioComparison()
{
    std::cout << "\nFor the parameters that created the pseudo-data...";
    std::string ofname  = GetParamFile();

    AngDistC Wd = GetAngDist(ofname);

    std::string nfname;
    std::cout << "\nFor the parameters resulting from the fit ...";
    nfname = GetParamFile();

    AngDistC Wf = GetAngDist(nfname);

    Double_t xpar[] = {90*CLHEP::deg,0};
    Double_t xperp[] = {90*CLHEP::deg, 90*CLHEP::deg};

    Double_t oratio = Wd(xpar)/Wd(xperp);
    Double_t oasymm = (Wd(xperp) - Wd(xpar))/(Wd(xperp) + Wd(xpar));
    Double_t nratio = Wf(xpar)/Wf(xperp);
    Double_t nasymm = (Wf(xperp) - Wf(xpar))/(Wf(xperp) + Wf(xpar));

    std::cout << std::setiosflags(std::ios::scientific | std::ios::right);
    std::cout << std::setprecision(2);


    std::cout << std::setw(30) << "Pseudo-data generator params" << " : " << ofname << std::endl;
    std::cout << std::setw(30) << "Fit result params" << " : " << nfname << std::endl;

    std::cout << "\nThe following results correspond to theta=90 deg" << std::endl;
    std::cout << "\n" << std::setw(10) << "Dist"
            << std::setw(10) << "Ratio"
            << std::setw(10) << "Asymm";


    std::cout << "\n" << std::setw(10) << "data" << std::setw(10) << oratio << std::setw(10) << oasymm;
    std::cout << "\n" << std::setw(10) << "fit" << std::setw(10) << nratio  << std::setw(10) << nasymm;

    std::cout << "\n\n" << std::setw(15) << "Ratio rel diff" << " = " << std::setw(10) << (oratio - nratio)*2.0/(oratio+nratio);
    std::cout << "\n" << std::setw(15) << "Asymm rel diff" << " = " << std::setw(10) << (oasymm - nasymm)*2.0/(oasymm+nasymm);

    std::cout << std::resetiosflags(std::ios::scientific | std::ios::right);
    std::cout << std::setprecision(6);
    std::cout << "\n" << std::endl;
}

#ifndef __CINT__
Int_t main()
{
    RatioComparison();
    return 0;
}
#endif
