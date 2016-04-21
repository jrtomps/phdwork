// PrintFractionalMultiplicityPerStrip.cpp
//
// Jeromy Tompkins
// 2/22/2012
//

#include <iostream>
#include <iomanip>
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

using namespace std;

void
PrintFractionalMultiplicityPerStrip(TH2* h, Int_t nmult)
{

    Double_t bc, sbc;
    cout << setw(5) << ' '
            << setfill('.') << "|"
            << setw(14) << "Bin Content..." << "|"
            << setfill(' ');
    cout << "\n"
            << setw(5) << "Bin"
            << setw(8) << "M[ 1]"
            << setw(5) << "M[" << setw(2) << nmult << "]"
            << setw(9) << "M[ 1]/M[" << setw(2) << nmult << "]";
    cout << setfill('-')
            << "\n"
            << setw(5+8+8+12) << '-'
            << setfill(' ');

    for (Int_t bin=h->GetXaxis()->GetFirst(); bin<=h->GetXaxis()->GetLast(); bin++)
    {

        bc = h->Integral(bin,bin, nmult+1,-1);
        sbc = h->Integral(bin,bin, 2, -1);

        cout << "\n"
                << setiosflags(ios::fixed)
                << setprecision(0)
                << setw(5) << bin;

        cout << setw(8) << sbc
                << setw(8) << bc
                << resetiosflags(ios::fixed)
                << setiosflags(ios::scientific)
                << setprecision(2);
        if (sbc==0)
        {
            cout << setw(12) << 0;
        }
        else
        {
            cout << setw(12) << bc/sbc;
        }
        cout << resetiosflags(ios::scientific);
    }


    std::cout << std::endl;
}

void PrintFractionalMultiplicityPerStrip(TString fname,
                                         Int_t nmult=2,
                                         TString hname="coinc_map/coinc_map_mult_f")
{
    TFile *f = new TFile(fname.Data());
    if (!f->IsOpen())
    {
        cerr << "Failed to open file " << fname.Data() << endl;
        return;
    }

    TH2* h2 = dynamic_cast<TH2*>(f->Get(hname.Data()));
    if (h2==NULL)
    {
        cerr << hname.Data() << " doesn't exist" << endl;
        return;
    }
    else
    {
        PrintFractionalMultiplicityPerStrip(h2,nmult);
    }
    f->Close();
}
