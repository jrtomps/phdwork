

#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "Parameters.h"
#include "G4Transform3D.hh"
#include "Randomize.hh"
#include "AngDist.h"
#include "TFoam.h"
#include "TAngFoamIntegrandWrapper.h"
#include "TRandom3.h"
//#include <boost/shared_ptr.hpp>
#include <memory>

#include "G4Ions.hh"
#include "G4Event.hh"
#include "G4VSolid.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RandomDirection.hh"
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimarySupervisor.hh"
#include "G4Geantino.hh"
#include "G4ParticleDefinition.hh"
#include "FragSimSource.hh"
#include "TreeMaker.hh"
#include "DataBucket.h"
#include "FragSimPrimGenActionMessenger.hh"
#include "FragSimPrimaryGeneratorAction.hh"

FragSimPrimaryGeneratorAction::FragSimPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(new G4ParticleGun(1)),
      fDistribution(""),
      fOrigin(0.0*cm,0.0*cm,1.0*cm),
      fVertexPos(0, 0, 0),
      fVertexDir(0, 0, 0),
      fTransform(),
      fSource(0),
      fDistState(kIsotropic),
      fDistNameMap(),
      fSupervisor(NULL),
      fFoam(0),
      fRandom(0),
      fMassDistr(0),
      fTotalVertices(0),
      fUsedVertices(0),
      messenger(new FragSimPrimGenActionMessenger(this))
{
    std::cout << "Constructing FragSimPrimaryGeneratorAction" << std::endl;
    SetupNameMap();
    fDistribution = fDistNameMap[fDistState];

    fParticleGun->SetParticleDefinition(G4Geantino::Definition());
    fParticleGun->SetParticleEnergy(10.0*MeV);
    fParticleGun->SetParticlePosition(fOrigin);

    DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
    db.GetDoubleMap()["a"] = 0;
    db.GetDoubleMap()["b"] = 0;
    db.GetDoubleMap()["c"] = 0;
    db.GetDoubleMap()["d"] = 0;
    db.GetStringMap()["distribution"] = "isotropic";

}

FragSimPrimaryGeneratorAction::~FragSimPrimaryGeneratorAction()
{
  // Set the stream format but save the old format
  std::ios::fmtflags old_flags = std::cout.flags(std::ios::scientific);
  std::streamsize prec = std::cout.precision(3);
  
	std::cout << "\n-----------------------------------------------------";
	std::cout << "\nFragSimPrimaryGeneratorAction End of Life Information";
	std::cout << "\nTotal vertices = " << fTotalVertices;
	std::cout << "\nUsed vertices  = " << fUsedVertices;
	std::cout << "\nFrac beam-target intersection = " << (1.0*fUsedVertices)/fTotalVertices;
	std::cout << std::endl;
  // Reset the stream format to what was originally there.
  std::cout.flags(old_flags);
  std::cout.precision(prec);

    delete fParticleGun;
    if (fSupervisor!=0) { delete fSupervisor; fSupervisor = 0; }
    if (fFoam!=0) { delete fFoam; fFoam=0; }
    if (fRandom!=0) { delete fRandom; fRandom=0;}
    delete messenger; messenger = 0;
}

void FragSimPrimaryGeneratorAction::SetUserDistribution(G4String fname)
{
    std::ifstream pfile (fname.data());
    if (!pfile.is_open())
    {

        G4cerr << fname << " could not be found. User distribution not set" << G4endl;
    }
    Parameters params(pfile);

    G4cout << "Found the following parameters:" << G4endl;
    G4cout << params << G4endl;

    Double_t pol;
    G4cout << "\nEnter polarization in range [0, 1.0] : " << G4endl;
    G4cin >> pol;

    AngDist w(params, pol, ELECTRIC);
    SetUserDistribution(w);
}

void FragSimPrimaryGeneratorAction::SetUserDistribution(const AngDist& func)
{
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> func_ptr(func.Clone());
    TAngFoamIntegrandWrapper *wfunc = new TAngFoamIntegrandWrapper(func_ptr);
    if (fFoam==0) fFoam = new TFoam("FragSimFoam");

    fFoam->SetkDim(2);
    fFoam->SetRho(wfunc);
    fRandom = new TRandom3;
    fFoam->SetPseRan(fRandom);
    fFoam->SetnCells(2000);
    fFoam->SetOptDrive(1); // Varedu
    fFoam->Initialize();
    fDistState = kUserDistribution;

    const Double_t* pars = func.Parameters();
    DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
    db.GetDoubleMap()["a"] = pars[0];
    db.GetDoubleMap()["b"] = pars[1];
    db.GetDoubleMap()["c"] = pars[2];
    db.GetDoubleMap()["d"] = pars[3];
    db.GetStringMap()["distribution"] = "user";
}

void
FragSimPrimaryGeneratorAction::SetSource(VSource& source)
{
    fSource = &source;
}

void
FragSimPrimaryGeneratorAction::SetTransformation(const G4Transform3D& trans)
{
    fTransform = trans;
}

void FragSimPrimaryGeneratorAction::SetupMassDistribution()
{
    fMassDistr = new MassDistribution(90,232,7.0*MeV);
}

const G4ParticleGun*
FragSimPrimaryGeneratorAction::GetParticleGun(void) const
{
    return fParticleGun;
}

G4ParticleGun*
FragSimPrimaryGeneratorAction::GetParticleGun(void)
{
    return fParticleGun;
}

void 
FragSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

    fVertexPos = GenerateVertexPosition();
    fVertexDir = GenerateVertexDirection();

#ifdef DEBUG_PrimaryGeneratorAction
    std::cout << "x="   << std::setprecision(2) << std::setw(8) << std::left << fVertexPos.getX()/cm
              << " y="  << std::setprecision(2) << std::setw(8) << std::left << fVertexPos.getY()/cm
              << " z="  << std::setprecision(2) << std::setw(8) << std::left << fVertexPos.getZ()/cm
              << " px=" << std::setprecision(2) << std::setw(8) << std::left << fVertexDir.getX()
              << " py=" << std::setprecision(2) << std::setw(8) << std::left << fVertexDir.getY()
              << " pz=" << std::setprecision(2) << std::setw(8) << std::left << fVertexDir.getZ()
              << std::endl;
#endif

    fParticleGun->SetParticlePosition(fVertexPos);
    fParticleGun->SetParticleMomentumDirection(fVertexDir);

    if (fMassDistr!=0)
    {
        SampleMassAndEnergyDistribution();
    }

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

void FragSimPrimaryGeneratorAction::SampleMassAndEnergyDistribution()
{
    G4int A;
    G4int Z;
    G4double energy;
    G4ParticleTable* table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* fragment=nullptr;
    G4int count = 0;
    while (fragment==nullptr && count<30)
    {
      fMassDistr->SampleIsotopeAndEnergy(Z, A, energy);
      fragment = table->GetIon(Z,A,0*MeV);
      count++;
    }

    if (count==30)
    {
        std::ostringstream message;
        message << "Failed to find a suitable fragment";
        G4Exception("FragSimPrimaryGeneratorAction::SampleMassAndEnergyDistribution()",
                               "PrimGen0004",RunMustBeAborted, message);
    }
    fParticleGun->SetParticleDefinition(fragment);
    fParticleGun->SetParticleEnergy(energy);
}

G4ThreeVector 
FragSimPrimaryGeneratorAction::SampleIsotropicDistribution(void) const
{
    return GetRandomDirection();
}

G4ThreeVector FragSimPrimaryGeneratorAction::SampleUserDistribution()
{
    if (fFoam==0)
    {
        std::ostringstream message;
        message << "TFoam has not been setup";
        G4Exception("FragSimPrimaryGeneratorAction::SampleUserDistribution()",
                    "PrimGen0002", RunMustBeAborted, message);
    }

    G4double x[2];
    fFoam->MakeEvent();
    // The next function copies the values of TFoam::fMCvect
    // into x. fMCvect is the array that is passed to the
    // TFoamIntegrand::Density function as the parameter and
    // thus whatever transformations occur in the Density funcion
    // are copied into x.
    fFoam->GetMCvect(x);

    // because we use the TAngFoamIntegrand, the x values are
    // x[0] --> theta (radians) and 0 < x[0] < pi
    // x[1] --> phi (radians)   and -pi < x[1] < pi
    G4double cosTheta = ::cos(x[0]);
    G4double sinTheta = ::sqrt(1.0-cosTheta*cosTheta); // b/c sintheta is always positive b/t 0 and pi

    G4double phi = x[1];
    G4double ux = sinTheta*std::cos(phi),
            uy = sinTheta*std::sin(phi),
            uz = cosTheta;

    return G4ThreeVector(ux,uy,uz);
}

G4ThreeVector
FragSimPrimaryGeneratorAction::GetRandomDirection(void) const
{
    // This code is copied from TestEm4.
    //this function is called at the begining of event
    //
    //distribution uniform in solid angle
    //
    G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double ux = sinTheta*std::cos(phi),
            uy = sinTheta*std::sin(phi),
            uz = cosTheta;

    return G4ThreeVector(ux,uy,uz);

}


void
FragSimPrimaryGeneratorAction::SetupNameMap(void)
{
    fDistNameMap[kIsotropic]     = "Isotropic";
    fDistNameMap[kUserDistribution]  = "UserDistribution";
}

G4ThreeVector
FragSimPrimaryGeneratorAction::GenerateVertexPosition(void)
{
    G4ThreeVector vec;
    if (fSource!=NULL)
    {
        do {
            vec = fSource->GetRandomPosition();
			++fTotalVertices;
        } while (fSupervisor!=NULL && !fSupervisor->IsGoodVertex(vec));
    }
    else
	{
        vec = fOrigin;
		++fTotalVertices;
	}	
	++fUsedVertices;

    return vec;
}

G4ThreeVector
FragSimPrimaryGeneratorAction::GenerateVertexDirection(void)
{
    G4ThreeVector dir;
    switch (fDistState)
    {
    case kIsotropic:
        dir = SampleIsotropicDistribution();
        break;
    case kUserDistribution:
        dir = SampleUserDistribution();
        break;
    default:
        dir = SampleIsotropicDistribution();
        break;
    }
    return dir;
}

void
FragSimPrimaryGeneratorAction::SetPrimarySupervisor(const PrimarySupervisor* ps)
{
    // If supervisor passed into the function is good,
    // copy construct and transfer ownership to this
    if (ps!=NULL)
    {
        fSupervisor = new PrimarySupervisor(*ps);
    }
}

PrimarySupervisor*
FragSimPrimaryGeneratorAction::GetPrimarySupervisor(void) const
{ return fSupervisor; }
