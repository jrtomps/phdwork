#include "MassDistribution.hh"
#include <dirent.h>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include "TMath.h"
#include "TFile.h"
#include "TGraph.h"
#include "TSpline.h"
#include "NuclearChart.h"
#include <boost/algorithm/string.hpp>

MassDistribution::MassDistribution(G4int Z, G4int A, G4double energy)
    : fDataDir("data"),
    fZ(Z),
    fA(A),
    fEn(energy),
    fMassDistr(0),
    fTKE(0)
{
    std::string fname;

    fDataDir.append("/");

    std::string zname = NuclearChart::GetInstance()->GetName(Z);
    boost::to_lower(zname);

    std::ostringstream dname;
    dname << fA << zname;
    fDataDir += (dname.str() + "/");

    try
    {
       fname = FindNearestEnergy(energy);
    }
    catch (std::exception& exc)
    {
        std::cout << exc.what() << std::endl;

        std::cout << "Manually enter data file name : ";
        getline(std::cin, fname);

    }

    std::string filename = (fDataDir + fname);
    G4cout << "Using fragment mass energy distribution defined in " << filename << G4endl;
    LoadDataFromFile(filename);
}

std::string MassDistribution::FindNearestEnergy(Double_t e)
{
    ::DIR* dir = ::opendir(fDataDir.data());
    if (dir==0) throw std::runtime_error("Couldn't find data directory");

    std::map<Double_t, std::string> energies;

    ::dirent* d;
    Double_t en;
    while ( (d = ::readdir(dir)) )
    {
        en = ::atof(d->d_name);
        energies.insert(std::make_pair(en,std::string(d->d_name)));
    }

    std::map<Double_t, std::string>::iterator it = std::lower_bound(energies.begin(), energies.end(),
                                                                    std::make_pair(e,""),
                                                                    std::less<std::map<Double_t, std::string>::value_type >());

    std::map<Double_t,std::string>::iterator itfound = it;

    it--;

    std::string str;
    if ( (itfound->first-e) < (e-it->first) )
        str = itfound->second;
    else
        str = it->second;


    return str;
}

void MassDistribution::LoadDataFromFile(const std::string &db_fname)
{
    TGraph *gr = new TGraph(db_fname.data());
    gr->Print();
    TFile *f = new TFile("asdf.root","UPDATE");
    gr->Write("",TObject::kOverwrite);
    f->Close();

    std::cout << "Found " << gr->GetN() << " masses" << std::endl;
    fMassDistr = new TH1D("fMassDistr","",238,0.5,238.5);
    Double_t mass;

    // Compute a quadratic interpolation b/t first and second point
    Double_t x0 = gr->GetX()[0];
    Double_t dd = gr->GetX()[1]-gr->GetX()[0];
    Double_t y0 = gr->GetY()[0];
    Double_t y1 = gr->GetY()[1];

    Double_t a = (dd*dd*y0 - x0*x0*y0 + x0*x0*y1)/(dd*dd);
    Double_t b = 2.0*x0 * (y0-y1)/(dd*dd);
    Double_t c = (-y0+y1)/(dd*dd);

    TAxis *ax = fMassDistr->GetXaxis();
    for (Int_t i=1; i<=fMassDistr->GetNbinsX(); i++)
//    for (UInt_t i=0; i<gr->GetN(); i++)
    {
//        mass = ax->FindBin(gr->GetX()[i]);
        mass = ax->GetBinCenter(i);
        if (mass>=gr->GetX()[1] && mass<=gr->GetX()[gr->GetN()-1]) {
          double val = gr->Eval(mass,0,"S");
          // if the spline results in a negative value, we set it to zero
          if (val < 0) val = 0;
          fMassDistr->SetBinContent(i, val);
        } else if (mass>=gr->GetX()[0] && mass<gr->GetX()[1])
        {
          double val = a + b*mass + c*mass*mass;
          fMassDistr->SetBinContent(i, val);
//            fMassDistr->SetBinContent(i, 1.12816059451781147e-02*mass+(gr->GetY()[0]-gr->GetX()[0]*1.12816059451781147e-02) );
        }
        else
            fMassDistr->SetBinContent(i,0);
    }
    delete gr;

    size_t in = db_fname.rfind("/");
    std::string enstr = db_fname.substr(in+1, db_fname.size()-in-1);
    fTKE = new TGraph((db_fname.substr(0,in)+"/TKE/" + enstr + "_TKE").data());
    std::cout << "Found " << fTKE->GetN() << " energies " << std::endl;

}

void MassDistribution::SampleIsotopeAndEnergy(G4int& z, G4int& a, G4double& energy) const
{
    a = fMassDistr->GetRandom();
    z = ComputeZFromA(a);

    if (a>=fTKE->GetX()[0] && a <= fTKE->GetX()[fTKE->GetN()-1])
    {
        energy = fTKE->Eval(a,0,"S")*MeV;
//        std::cout << "a = " << a << "\t energy = " << energy << std::endl;
    }
    else
        energy = 0;
}


G4int MassDistribution::ComputeZFromA(G4int a) const
{

    return static_cast<G4int>(a*90.0/232.0);
//    Double_t m, b;
//    if (neven < 19)
//    {
//        m = 1;
//        b = 0;
//    }
//    else if (neven < 57)
//    {
//        m = 0.674;
//        b = 6.20;
//    }
//    else if (neven < 60)
//    {
//        m = 0;
//        b = 50;
//    }
//    else if (neven < 103)
//    {
//        m = 0.544;
//        b = 13.47;
//    }
//    else if (neven < 107)
//    {
//        m = 0;
//        b = 82;
//    }
//    else
//    {
//        m = 0.708;
//        b = -9.79;
//    }
//
//    return (2.0*neven-b)/(1+m);
}
