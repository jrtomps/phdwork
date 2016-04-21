#ifndef MASSDISTRIBUTION_HH
#define MASSDISTRIBUTION_HH

#include "TH1.h"
#include "TGraph.h"
#include "Rtypes.h"
#include "globals.hh"

class MassDistribution
{
private:
    std::string fDataDir;
    G4int fZ;
    G4int fA;
    G4double fEn;
    TH1*     fMassDistr;
    TGraph *fTKE;

public:
    MassDistribution(G4int Z, G4int A, G4double energy);
    void SampleIsotopeAndEnergy(G4int& z, G4int& a, G4double& energy) const;

private:
    std::string FindNearestEnergy(Double_t e);
    void LoadDataFromFile(const std::string& db_fname);
    G4int ComputeZFromA(G4int a) const;

};

#endif // MASSDISTRIBUTION_HH
