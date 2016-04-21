// TargetHit.cc
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
// data by the TargetSensitivity class.
// 

#include <cstring>
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "TargetHit.hh"

TargetHit::TargetHit()
  : G4VHit(), 
    fphi(0), 
    ftheta(0), 
    finitWorldPos(), 
    fdeltaWorldPos(),
    finitMomentum(),
    fdeltaMomentum(),
    finitKE(0),
    fdeltaKE(0),
    ftotDepE(0),
    fnonIonizingDepE(0),
    fparticleName(""),
    fmass(0),
    fcharge(0),
    fdeltaCharge(0),
    ftrackLength(0),
    flocalPos()
{}


TargetHit::TargetHit(G4Step *aStep)
  : G4VHit(),
    fphi(aStep->GetPostStepPoint()->GetMomentumDirection().phi()),
    ftheta(aStep->GetPostStepPoint()->GetMomentumDirection().theta()),
    finitWorldPos(aStep->GetPreStepPoint()->GetPosition()),
    fdeltaWorldPos(aStep->GetDeltaPosition()),
    finitMomentum(aStep->GetPreStepPoint()->GetMomentum()),
    fdeltaMomentum(aStep->GetPostStepPoint()->GetMomentum()-finitMomentum),
    finitKE(aStep->GetPreStepPoint()->GetKineticEnergy()),
    fdeltaKE(aStep->GetPostStepPoint()->GetKineticEnergy() - finitKE),
    ftotDepE(aStep->GetTotalEnergyDeposit()),
    fnonIonizingDepE(aStep->GetNonIonizingEnergyDeposit()),
    fparticleName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()),
    fmass(aStep->GetTrack()->GetParticleDefinition()->GetPDGMass()),
    fcharge(aStep->GetPreStepPoint()->GetCharge()),
    fdeltaCharge(aStep->GetPostStepPoint()->GetCharge()-aStep->GetPreStepPoint()->GetCharge()),
    ftrackLength(aStep->GetTrack()->GetTrackLength()),
    flocalPos()
{
}

TargetHit::TargetHit(const TargetHit& obj)
    : G4VHit(obj),
    fphi(obj.fphi),
    ftheta(obj.ftheta),
    finitWorldPos(obj.finitWorldPos),
    fdeltaWorldPos(obj.fdeltaWorldPos),
    finitMomentum(obj.finitMomentum),
    fdeltaMomentum(obj.fdeltaMomentum),
    finitKE(obj.finitKE),
    fdeltaKE(obj.fdeltaKE),
    ftotDepE(obj.ftotDepE),
    fnonIonizingDepE(obj.fnonIonizingDepE),
    fparticleName(obj.fparticleName),
    fmass(obj.fmass),
    fcharge(obj.fcharge),
    fdeltaCharge(obj.fdeltaCharge),
    ftrackLength(obj.ftrackLength),
    flocalPos(obj.flocalPos)
{
}

TargetHit& TargetHit::operator=(TargetHit obj)
{
    swap(*this, obj);
    return *this;
}

TargetHit::~TargetHit(){;}

TargetHit& TargetHit::operator+=(TargetHit& obj)
{
   // sum up the summable objects...
   ftotDepE += obj.ftotDepE;
   fnonIonizingDepE += obj.fnonIonizingDepE;
   fdeltaWorldPos += obj.fdeltaWorldPos;
   fdeltaMomentum += obj.fdeltaMomentum;
   fdeltaCharge += obj.fdeltaCharge;

   ftrackLength += obj.ftrackLength;

   return *this;
}

void TargetHit::SetLocalPosition(const G4ThreeVector &vec)
{
    flocalPos = vec;
}
