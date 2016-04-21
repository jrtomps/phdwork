#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include "TH1.h"
#include "TDirectory.h"
#include "ROOTUtils.h"


std::ostream& _stream = std::cout;

enum ThetaGroup { UPSTREAM, DOWNSTREAM };



void PrintHeader(const Int_t start0, const Int_t start1, const Double_t xlo, const Double_t xhi)
{
    _stream << std::setiosflags(std::ios::fixed)
            << std::setprecision(0);

    _stream << "\nComparison Between Detector Integrals"
            << "\n"
            << "\nIntegration Region = [" << xlo << "," << xhi << "]"
            << "\nInitial strip index for det0 = " << start0
            << "\nInitial strip index for det1 = " << start1
            << "\n"
            << "\n" << std::setw(4) << "Off"
            << std::setw(10) << "Intgrl0"
            << std::setw(10) << "Intgrl1"
            << std::setw(10) << "(I0/I1)"
            << "\n" << std::setfill('=') << std::setw(34) << '='
            << std::setfill(' ') << std::flush;

}

void PrintLine (const Int_t offset, const Double_t val0, const Double_t val1)
{
    _stream << std::setiosflags(std::ios::fixed)
            << std::setprecision(1);
    _stream << std::setw(4) << offset
            << std::setw(10) << val0
            << std::setw(10) << val1;
    _stream << std::resetiosflags(std::ios::fixed);

    _stream << std::setiosflags(std::ios::scientific)
            << std::setprecision(2);
    if (val1!=0) _stream << std::setw(10) << val0/val1;
    else         _stream << std::setw(10) << 0;
    _stream << std::resetiosflags(std::ios::scientific);

}


void CompareIntegrals(TDirectory* dir,
                      ThetaGroup group,
                      const Double_t xlo,
                      const Double_t xhi)
{
    UInt_t start_index0 = 0;
    UInt_t start_index1 = 0;
    std::vector<TH1*> h = ROOTUtils::GetAllTH1InDirectory(dir);
    ROOTUtils::EnforceProperOrdering(h);

    const UInt_t nstrips_per_det = 16;
    if (group == UPSTREAM)
    {
        start_index0 = 0;
        start_index1 = 2;
    }
    else // DOWNSTREAM
    {
        start_index0 = 1;
        start_index1 = 3;
    }
    start_index0 *= nstrips_per_det;
    start_index1 *= nstrips_per_det;

    Int_t binlo = 0;
    Int_t binhi = 0;
    Double_t cont0 = 0;
    Double_t cont1 = 0;

    PrintHeader(start_index0, start_index1, xlo, xhi);
    _stream << std::endl;
    for (UInt_t i=0; i<nstrips_per_det; i++)
    {
        binlo = h[start_index0 + i]->GetXaxis()->FindBin(xlo);
        binhi = h[start_index0 + i]->GetXaxis()->FindBin(xhi);
        cont0 = h[start_index0+i]->Integral(binlo, binhi);

        binlo = h[start_index1 + i]->GetXaxis()->FindBin(xlo);
        binhi = h[start_index1 + i]->GetXaxis()->FindBin(xhi);
        cont1 = h[start_index1+i]->Integral(binlo, binhi);

        PrintLine(i,cont0, cont1);
        _stream << std::endl;
    }

}
