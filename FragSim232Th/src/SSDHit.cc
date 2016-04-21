// SSDHit.cc
//
// Jeromy Tompkins
// 9/16/2011
//
// This was originally the file AsymHit.cc, written by John Hardin.
// I have modified the structure of these files and changed the way
// the constructors assign values to the data members by make liberal
// use of the initialization list. This should improve the 
// functionality of the class.
//
// This class is the "package" that will be implemented for storing
// data by the SSDSensitivity class.
// 

#include <cstring>
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "SSDHit.hh"

SSDHit::SSDHit()
  : G4VHit(), 
    //    fmsg(""),
    fdetID(-1), 
    fsiStrip(-1), 
    feffPhi(0), 
    feffTheta(0), 
    fworldPos(),
    fdeltaWorldPos(),
    flocalPos(),
    fmomentum(),
    fdeltaMomentum(),
    finitKE(0),
    fdeltaKE(0),
    ftotDepE(0),
    fnonIonizingDepE(0),
    fdetName(""),
    fparticleName(""),
    fmass(0),
    fcharge(0),
    fdetLocationMap()
{
  SetupDetLocationMap();
  fdetName = fdetLocationMap[fdetID];
}

SSDHit::SSDHit(G4String detName, G4Step *aStep, G4int idetector, G4int iSistrip)
  : G4VHit(),
    //    fmsg(detName),
    fdetID(idetector), 
    fsiStrip(iSistrip), 
    feffPhi(0),
    feffTheta(0),
    fworldPos(aStep->GetPreStepPoint()->GetPosition()),
    fdeltaWorldPos(aStep->GetDeltaPosition()),
    flocalPos(),
    fmomentum(aStep->GetPreStepPoint()->GetMomentum()),
    fdeltaMomentum(aStep->GetPostStepPoint()->GetMomentum()-fmomentum),
    finitKE(aStep->GetPreStepPoint()->GetKineticEnergy()),
    fdeltaKE(aStep->GetPostStepPoint()->GetKineticEnergy() - finitKE),
    ftotDepE(aStep->GetTotalEnergyDeposit()),
    fnonIonizingDepE(aStep->GetNonIonizingEnergyDeposit()),
    fdetName(detName),
    fparticleName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()),
    fmass(aStep->GetTrack()->GetParticleDefinition()->GetPDGMass()),
    fcharge(aStep->GetTrack()->GetDynamicParticle()->GetCharge()),
    fdetLocationMap()
{
  SetupDetLocationMap();
  SetEffPhiTheta(aStep);
}

SSDHit::SSDHit(const SSDHit& obj)
    : G4VHit(obj),
    fdetID(obj.fdetID),
    fsiStrip(obj.fsiStrip),
    feffPhi(obj.feffPhi),
    feffTheta(obj.feffTheta),
    fworldPos(obj.fworldPos),
    fdeltaWorldPos(obj.fdeltaWorldPos),
    flocalPos(obj.flocalPos),
    fmomentum(obj.fmomentum),
    fdeltaMomentum(obj.fdeltaMomentum),
    finitKE(obj.finitKE),
    fdeltaKE(obj.fdeltaKE),
    ftotDepE(obj.ftotDepE),
    fnonIonizingDepE(obj.fnonIonizingDepE),
    fdetName(obj.fdetName),
    fparticleName(obj.fparticleName),
    fmass(obj.fmass),
    fcharge(obj.fcharge),
    fdetLocationMap(obj.fdetLocationMap)
{
}

SSDHit& SSDHit::operator=(SSDHit obj)
{
    swap(*this, obj);
    return *this;
}

SSDHit::~SSDHit(){;}

SSDHit& SSDHit::operator+=(SSDHit& obj)
{
   // sum up the summable objects...
   fdeltaKE += obj.GetDeltaKineticEnergy();
   ftotDepE += obj.GetTotalEnergyDeposit();
   fnonIonizingDepE += obj.GetNonIonizingEnergyDeposit();

   fdeltaWorldPos += obj.GetDeltaPosition();
   fdeltaMomentum += obj.GetDeltaMomentum();

   return *this;
}

void
SSDHit::SetupDetLocationMap(void)
{
  fdetLocationMap[0] = "UpStreamTop";
  fdetLocationMap[1] = "DownStreamLeft";
  fdetLocationMap[2] = "UpStreamRight";
  fdetLocationMap[3] = "DownStreamBottom";  
}


void 
SSDHit::SetDetName(G4String name)
{
  fdetName = name;
}


void
SSDHit::SetEffPhiTheta(G4double iphi,G4double ithe)
{
  feffPhi = iphi;
  feffTheta = ithe;
}

void 
SSDHit::SetEffPhiTheta(G4Step *aStep)
{
  G4ThreeVector vec = aStep->GetTrack()->GetVertexPosition();

  G4ThreeVector finalWorldPos = fworldPos + fdeltaWorldPos;
  // theta() returns atan2(perp(),dz) where perp() is sqrt(dx*dx+dy*dy)
  // and dz is the z component. This is the proper definition of the 
  // polar angle
  feffTheta = (finalWorldPos - vec).theta();
  // Phi = atan2(dy,dx) where dx and dy are the components
  // this is the proper definition of the azimuthal angle
  feffPhi   = (finalWorldPos - vec).phi();
}

void
SSDHit::SetLocalPos(const G4ThreeVector& avec)
{
  flocalPos = avec;
}
