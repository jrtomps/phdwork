#ifndef AllVolumeSource2_HH
#define AllVolumeSource2_HH

#include "G4String.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "VSource.hh"
#include <vector>

class AllVolumeSource2 : public VSource
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
    bool     fUseAllVolume;
    std::vector<G4double> fFaceLimits;
    std::vector<G4int> fFaceCounter;

public:
//    AllVolumeSource2(G4VPhysicalVolume *world_vol);
    AllVolumeSource2(G4String worldName, G4String targetName, G4double thickness=0);
    virtual ~AllVolumeSource2();

    virtual G4ThreeVector GetRandomPosition();

private:
    void Initialize();
    void GetTargetVolume();
    G4ThreeVector GetPositionFromSurfaceLayer();
    G4ThreeVector GetPositionFromWholeVolume();
    G4ThreeVector GetRandomPositionFromVolume(const G4ThreeVector& center,
                                              const G4double xw,
                                              const G4double yw,
                                              const G4double zw);
    G4int RandomlySelectFace();
    void SetupFaceLimits();
};

#endif // AllVolumeSource2_HH
