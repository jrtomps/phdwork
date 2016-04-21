// This is code taken from the HandsOn3 tutorial by  T. Aso


#include "SphereParameterisation.hh"
#include "G4Sphere.hh"
#include "G4VPhysicalVolume.hh"

SphereParameterisation::SphereParameterisation(const G4double minRadius,
                                               const G4double maxRadius,
                                               const G4double deltaTheta,
                                               const G4double deltaPhi)
                                                   :G4VPVParameterisation()
                                                   ,fMinRadius(minRadius)
                                                   ,fMaxRadius(maxRadius)
                                                   ,fDeltaTheta(deltaTheta)
                                                   ,fDeltaPhi(deltaPhi)
{}

SphereParameterisation::~SphereParameterisation() {}

void SphereParameterisation::ComputeDimensions(G4Sphere& sphere,
                                               const G4int copyNo,
                                               const G4VPhysicalVolume*) const
{
    G4int nphi = static_cast<G4int>(360.0*deg/fDeltaPhi);

    G4int itheta = copyNo/nphi;
    G4int iphi = copyNo%nphi;

    G4double startTheta = itheta*fDeltaTheta;
    G4double startPhi = iphi*fDeltaPhi;
    sphere.SetInsideRadius(fMinRadius);
    sphere.SetOuterRadius(fMaxRadius);
    sphere.SetStartPhiAngle(startPhi);
    sphere.SetDeltaPhiAngle(fDeltaPhi);
    sphere.SetStartThetaAngle(startTheta);
    sphere.SetDeltaThetaAngle(fDeltaTheta);
}
