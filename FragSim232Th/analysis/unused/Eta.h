#ifndef ETA_H
#define ETA_H

#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "Math/IFunction.h"

class Eta : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    TFile* fFile; //!< root file pointer (object not owned by this class)

public:
    Eta(TFile* file);

    Eta*   Clone() const;
    UInt_t NDim() const {return 6;}

    TH2* Eta2DAtTheta(UInt_t strip, UInt_t Z, UInt_t A, Double_t theta);
    TH2* Eta2DAtPhi(UInt_t strip, UInt_t Z, UInt_t A, Double_t phi);
    TH2* Eta2DAtDepth(UInt_t strip, UInt_t Z, UInt_t A, Double_t depth);


private:
    TH3*  GetTH3(UInt_t strip, UInt_t Z, UInt_t A) const;
    Double_t DoEval(const double *x) const;

};

#endif // ETA_H
