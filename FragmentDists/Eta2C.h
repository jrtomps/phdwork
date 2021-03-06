#ifndef Eta2C_H
#define Eta2C_H

#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "Math/IFunction.h"

class Eta2C : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    TFile*   fFile; //!< root file pointer (object not owned by this class)
    Double_t fNorm;

public:
    Eta2C(TFile* file);

    Eta2C*   Clone() const;
    UInt_t NDim() const;

    TH2* Eta2CDAtTheta(UInt_t Z, UInt_t A, Double_t theta);
    TH2* Eta2CDAtPhi(UInt_t Z, UInt_t A, Double_t phi);
    TH2* Eta2CDAtDepth(UInt_t Z, UInt_t A, Double_t depth);

    TH3*  GetAngleVsDepth(UInt_t Z, UInt_t A) const;

    UInt_t GetNIsotopes() const;

    TH1* GetMassDistribution() const;
    TH1* GetTKEDistribution() const;

    Double_t GetNormalization() const;

    Double_t GetEnergyWeight(Double_t E) const;
    Double_t ComputeIntegral(TH3* h);

private:
    TH3* GetTH3(UInt_t Z, UInt_t A) const;
    void ComputeNormalization();
    void EnforceProbabilities();
    Bool_t IsIsotopeFolder(TDirectory* dir) const;
    Double_t DoEval(const double *x) const;

};

#endif // Eta2C_H
