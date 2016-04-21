#include <iostream>
#include <iomanip>
#include <cmath>
#include "G4VSensitiveDetector.hh"
#include "TargetHit.hh"
#include "G4VTouchable.hh"
#include "G4AffineTransform.hh"
#include "G4ThreeVector.hh"
#include "TargetSensitivity.hh"

TargetSensitivity::TargetSensitivity(G4String det_name, G4int collectionID)
    : G4VSensitiveDetector(det_name),
      fcollectionID(collectionID),
      fdetName(det_name)
{
    collectionName.insert(det_name+"_collection");
}

TargetSensitivity::~TargetSensitivity(){;}

G4bool 
TargetSensitivity::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

//    G4cout << "trackid=" << aStep->GetTrack()->GetTrackID() << G4endl;
//    G4cout << "  parentid=" << aStep->GetTrack()->GetParentID() << G4endl;

    TargetHit*  newHit = new TargetHit(aStep);

    // Compute the local position of the initial position
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    const G4AffineTransform& topTransform = touchable->GetHistory()->GetTopTransform();
    G4ThreeVector localPos = topTransform.TransformPoint(aStep->GetPreStepPoint()->GetPosition());
    newHit->SetLocalPosition(localPos);

    hitsCollection->insert( newHit );

    return true;
}

void 
TargetSensitivity::Initialize(G4HCofThisEvent* HCE)
{
    hitsCollection = new TargetHitsCollection(fdetName,
                                              G4String() = fdetName+"_collection");

    HCE->AddHitsCollection(fcollectionID,hitsCollection);
}

void 
TargetSensitivity::EndOfEvent(G4HCofThisEvent*)
{
}

