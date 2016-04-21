#include <iomanip>
#include "ProgressBar.h"

ProgressBar::ProgressBar(UInt_t nmax, std::ostream& stream, std::string prestr)
    : fStream(stream),
    fMax(nmax),
    fCurr(0),
    fWidth(64),
    fPreString(prestr)
{
}

ProgressBar::ProgressBar(UInt_t nmax, UInt_t width, std::ostream& stream, std::string prestr)
    : fStream(stream),
    fMax(nmax),
    fCurr(0),
    fWidth(width),
    fPreString(prestr)
{
}


void ProgressBar::Reset()
{
    fCurr = 1;
}

void ProgressBar::AdvanceProgress()
{
    Print();
    fCurr++;
}


void ProgressBar::Print()
{
    std::ios::fmtflags flags = fStream.flags(std::ios::fixed);
    std::streamsize prec = fStream.precision(1);

    Double_t frac = (1.0*fCurr)/fMax;
    Int_t nmarks =  frac * fWidth;
    fStream << "\r"
            << fPreString
            << " |";

    if (nmarks>0)
        fStream << std::setfill('-') << std::setw(nmarks) << '-'
                << std::setfill(' ');

    if (fWidth-nmarks>0)
        fStream << std::setw(fWidth-nmarks) << ' ';

    fStream << "| "
            << std::setw(5) << 100.0*frac
            << "%";

    fStream.flags(flags);
    fStream.precision(prec);
}
