#include "StripInfo.h"
#include "ColumnFileLoader.h"
#include <stdexcept>
#include "TMath.h"

StripInfo::StripInfo()
    : fGlobalIndex(0),
    fPairGlobalIndex(0),
    fParentDetIndex(0),
    fLocalIndex(0),
    fTheta(0),
    fUTheta(0),
    fPhi(0),
    fUPhi(0)
{}

StripInfo::StripInfo(UInt_t global_index, UInt_t partner_index,
                     Double_t theta, Double_t utheta,
                     Double_t phi, Double_t uphi)
                         : fGlobalIndex(global_index),
                         fPairGlobalIndex(partner_index),
                         fParentDetIndex(global_index/16),
                         fLocalIndex(global_index%16),
                         fTheta(theta),
                         fUTheta(utheta),
                         fPhi(phi),
                         fUPhi(uphi)
{
}

UInt_t StripInfo::GetGlobalIndex()const { return fGlobalIndex;}
UInt_t StripInfo::GetPairGlobalIndex()const { return fPairGlobalIndex;}
UInt_t StripInfo::GetDetectorIndex()const {return fParentDetIndex;}
UInt_t StripInfo::GetLocalIndex()const {return fLocalIndex;}

Double_t StripInfo::GetTheta() const {return fTheta;}
Double_t StripInfo::GetThetaError()const {return fUTheta;}
std::pair<Double_t, Double_t> StripInfo::GetThetaWithError() const { return std::make_pair(fTheta, fUTheta);}
Double_t StripInfo::GetPhi()const { return fPhi;}
Double_t StripInfo::GetPhiError()const {return fUPhi;}
std::pair<Double_t, Double_t> StripInfo::GetPhiWithError() const { return std::make_pair(fPhi, fUPhi); }


StripDb::StripDb(const std::string &angle_fname)
    : fStrips(),
    fPairIndices()
{
    SetUpPairMap();
    PopulateDatabase(angle_fname);
}

void StripDb::SetUpPairMap()
{
    fPairIndices.resize(32);
    for (UInt_t i=0; i<16; i++)
    {
        fPairIndices[i] = std::make_pair(16+i,48+i);
        fPairIndices[16+i] = std::make_pair(47-i,15-i);
    }

}

void StripDb::PopulateDatabase(const std::string &angle_fname)
{

    ColumnFileLoader cfl;
    cfl.LoadData(angle_fname);

    ColumnData theta = cfl.GetColumnForDet(0,0);
    ColumnData utheta = cfl.GetColumnForDet(0,1);
    ColumnData phi = cfl.GetColumnForDet(0,2);
    ColumnData uphi = cfl.GetColumnForDet(0,3);

    if (fPairIndices.size()!=32)
        throw std::runtime_error ("in StripDb::PopulateDatabase(...) insufficient pair indices loaded");

    fStrips.resize(64);
    UInt_t fi, pi;
    for (UInt_t i=0; i<fPairIndices.size(); i++)
    {
        fi = fPairIndices[i].first;
        pi = fPairIndices[i].second;
        fStrips[fi] = StripInfo(fi,  pi,theta[fi], utheta[fi], phi[fi], uphi[fi]);
        fStrips[pi] = StripInfo(pi, fi, theta[pi], utheta[pi], phi[pi], uphi[pi]);

    }
}

StripInfo StripDb::GetStrip(Double_t theta, Double_t phi)
{
    StripPair pair = GetStripPair(theta);

    StripInfo strip;
    Double_t tolerance = 3.0;
    if (TMath::Abs(phi-pair.first.GetPhi())<tolerance)
        strip = pair.first;
    else if ((phi-pair.second.GetPhi())<tolerance)
        strip = pair.second;
    else
        std::cerr << "Failed to find the desired strip, returning bad StripInfo" << std::endl;

    return strip;
}

StripPair StripDb::GetStripPair(Double_t theta)
{
    StripInfo strip;
    UInt_t i=0;
    Double_t tolerance = 1.0;
    while (TMath::Abs(strip.GetTheta()-theta)>tolerance)
    {
        strip = fStrips[i];
        i++;
    }

    return std::make_pair(fStrips[strip.GetGlobalIndex()], fStrips[strip.GetPairGlobalIndex()]);
}

UInt_t StripDb::GetThetaIndex(const StripInfo &strip)
{
   UInt_t id = strip.GetGlobalIndex();
   UInt_t th_id = 0;
   for (UInt_t i=0; i<fPairIndices.size(); i++)
   {
       if (id==fPairIndices[i].first || id==fPairIndices[i].second)
       {
           th_id = i;
           break;
       }
   }

    return th_id;
}

void StripDb::PrintThetaIndices()
{
    std::ios::fmtflags flags = std::cout.flags(std::ios::scientific);
    std::streamsize prec = std::cout.precision(2);
    for (UInt_t i=0; i<fPairIndices.size(); i++)
    {
        std::cout << "\n" << std::setw(3) << i
                << std::setw(12) << fPairIndices[i].first
                << std::setw(12) << fPairIndices[i].second;
    }
    std::cout.flags(flags);
    std::cout.precision(prec);

    std::cout << std::endl;
}

void StripDb::WriteTo(std::ostream &stream)
{
    for (UInt_t i=0; i<fStrips.size(); i++)
    {
        stream << "\n" << fStrips[i];
    }
    stream << std::endl;
}
