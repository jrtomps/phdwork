#ifndef ALLVOLUMESOURCE_HH
#define ALLVOLUMESOURCE_HH

#include "G4String.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

class AllVolumeSource : public VSource
{
private:
//    G4VPhysicalVolume *fWorld; //!< this class owns this and must delete it.
    G4String fWorldName;
    G4String fTargetName;
    G4VPhysicalVolume *fTarget;
    G4double fXHalfLength;
    G4double fYHalfLength;
    G4double fZHalfLength;
    G4double fThickness;

public:
//    AllVolumeSource(G4VPhysicalVolume *world_vol);
    AllVolumeSource(G4String worldName, G4String targetName, G4double thickness=0);
    virtual ~AllVolumeSource();

    virtual G4ThreeVector GetRandomPosition();

private:
    void Initialize();
    void GetTargetVolume();
};

#endif // ALLVOLUMESOURCE_HH
