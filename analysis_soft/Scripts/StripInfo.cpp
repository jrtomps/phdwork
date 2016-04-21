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
    UInt_t pi;
    for (UInt_t i=0; i<fPairIndices.size(); i++)
    {
        pi = fPairIndices[i];
        fStrips[i] = StripInfo(i,pi,theta[i], utheta[i], phi[i], uphi[i]);
        fStrips[pi] = StripInfo(pi,i, theta[pi], utheta[pi], phi[pi], uphi[pi]);
    }
}

StripInfo StripDb::GetStrip(Double_t theta, Double_t phi)
{
    StripPair pair = GetStripPair(theta);

    StripInfo strip;
    if (phi == pair.first.GetPhi())
        strip = pair.first;
    else if (phi == pair.second.GetPhi())
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
        strip = fStrips[i]
    }

    return std::make_pair(fStrips[strip.GetGlobalIndex()], fStrips[strip.GetPairGlobalIndex()]);
}

void StripDb::WriteTo(std::ostream &stream)
{
    for (UInt_t i=0; i<fStrips.size(); i++)
    {
        stream << "\n" << fStrips[i];
    }
    stream << std::endl;
}
