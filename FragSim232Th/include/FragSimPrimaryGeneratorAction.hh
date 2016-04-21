
#include <map>
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RunManager.hh"
#include "VSource.hh"
#include "G4VPhysicalVolume.hh"
#include "AngDist.h"
#include "TFoam.h"
#include "MassDistribution.hh"
#include "FragSimPrimGenActionMessenger.hh"

#ifndef FragSimPrimaryGeneratorAction_h
#define FragSimPrimaryGeneratorAction_h 1

class G4ParticleGun;
class G4VSolid;
class PrimarySupervisor;
class TRandom3;

enum DISTR {kIsotropic, kUserDistribution };

class FragSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
private:
    G4ParticleGun*     fParticleGun;
    G4String          fDistribution;
    G4ThreeVector           fOrigin;
    G4ThreeVector        fVertexPos;
    G4ThreeVector        fVertexDir;
    G4Transform3D        fTransform;
    VSource*                fSource;
    DISTR                fDistState;
    std::map<DISTR, G4String> fDistNameMap;
    PrimarySupervisor*  fSupervisor;
    TFoam*                    fFoam;
    TRandom3*               fRandom;
    MassDistribution*    fMassDistr;
	UInt_t           fTotalVertices;
	UInt_t           fUsedVertices;

    FragSimPrimGenActionMessenger* messenger;

public:
    FragSimPrimaryGeneratorAction();
    ~FragSimPrimaryGeneratorAction(void);

public:
    void          SetUserDistribution(const AngDist& func);
    void          SetUserDistribution(G4String fname);
    void          SetupMassDistribution();
    void          SetSource(VSource& source);
    void          SetTransformation(const G4Transform3D& trans);

    const G4ParticleGun* GetParticleGun(void) const;
    G4ParticleGun* GetParticleGun(void);

    G4ParticleTable* GetParticleTable();

    void          GeneratePrimaries(G4Event* anEvent);
    G4ThreeVector SampleIsotropicDistribution() const;
    G4ThreeVector SampleUserDistribution();

    G4ThreeVector GetRandomDirection(void) const;
    G4ThreeVector GenerateVertexPosition(void);
    G4ThreeVector GenerateVertexDirection(void);

    void SampleMassAndEnergyDistribution();
    void SetPrimarySupervisor(const PrimarySupervisor* ps);
    PrimarySupervisor* GetPrimarySupervisor(void) const;

private:

    void SetupNameMap(void);

};

#endif


