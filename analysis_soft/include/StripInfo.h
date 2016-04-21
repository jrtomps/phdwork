#ifndef STRIPINFO_H
#define STRIPINFO_H

#include "Rtypes.h"
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>

class StripInfo
{
private:
    UInt_t   fGlobalIndex;
    UInt_t   fPairGlobalIndex;
    UInt_t   fParentDetIndex;
    UInt_t   fLocalIndex;
    Double_t fTheta;
    Double_t fUTheta;
    Double_t fPhi;
    Double_t fUPhi;

public:
    StripInfo();
    StripInfo(UInt_t global_index, UInt_t partner_index,
              Double_t theta, Double_t utheta,
              Double_t phi, Double_t uphi);

    UInt_t GetGlobalIndex()const;
    UInt_t GetPairGlobalIndex()const;
    UInt_t GetDetectorIndex()const;
    UInt_t GetLocalIndex()const;

    Double_t GetTheta() const;
    Double_t GetThetaError()const;
    std::pair<Double_t, Double_t> GetThetaWithError() const;
    Double_t GetPhi()const;
    Double_t GetPhiError()const;
    std::pair<Double_t, Double_t> GetPhiWithError() const;
};

inline Bool_t operator==(StripInfo& lhs, StripInfo& rhs)
{
    return (lhs.GetGlobalIndex()==rhs.GetGlobalIndex());
}

inline std::ostream& operator<<(std::ostream& stream, StripInfo& strip)
{

    stream << std::setw(3) << strip.GetGlobalIndex()
            << std::setw(3) << strip.GetLocalIndex()
            << std::setw(3) << strip.GetDetectorIndex()
            << std::setw(3) << strip.GetPairGlobalIndex();
    std::ios::fmtflags flags = stream.flags(std::ios::scientific);
    std::streamsize prec = stream.precision(2);
    stream << std::setw(10) << strip.GetTheta()
            << std::setw(10) << strip.GetThetaError()
            << std::setw(10) << strip.GetPhi()
            << std::setw(10) << strip.GetPhiError();
    stream.flags(flags);
    stream.precision(prec);

    return stream;
}

typedef std::pair<StripInfo, StripInfo> StripPair;

class StripDb
{
private:
    std::vector<StripInfo> fStrips;
    std::vector<std::pair<UInt_t, UInt_t> > fPairIndices;

public:
    StripDb(const std::string& angle_fname);

    StripInfo GetStrip(Double_t theta, Double_t phi);
    StripPair GetStripPair(Double_t theta);
    UInt_t    GetThetaIndex(const StripInfo& strip);

    void PrintThetaIndices(void);
    void WriteTo(std::ostream& stream=std::cout);

private:
    void SetUpPairMap();
    void PopulateDatabase(const std::string& angle_fname="angle_data/angles");

};


#endif // STRIPINFO_H
