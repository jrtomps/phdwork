// code iS modified from the BeamTestScoreParameterisation class
// from the HandsOn3 tutorial. Originally written by T. Aso

#ifndef SPHEREPARAMETERISATION_HH
#define SPHEREPARAMETERISATION_HH

#include "G4VPVParameterisation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>

class G4Box;
class G4Cons;
class G4Hype;
class G4LogicalVolume;
class G4Orb;
class G4Para;
class G4Polycone;
class G4Polyhedra;
class G4Sphere;
class G4Trap;
class G4Trd;
class G4Tubs;
class G4Torus;

class SphereParameterisation : public G4VPVParameterisation {

public:

  // Constructor
    SphereParameterisation(const G4double minRadius, const G4double maxRadius,
                           const G4double deltaTheta,
                           const G4double deltaPhi);

  // Destructor
  virtual ~SphereParameterisation();

  // Method
  virtual void ComputeDimensions(G4Sphere& sphere,
                                 const G4int copyNo,
                                 const G4VPhysicalVolume*) const;

private:

  // Override to get rid of warning messages
  void ComputeTransformation(const G4int, G4VPhysicalVolume*)const {};
  void ComputeDimensions (G4Tubs&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Trd&,const G4int,const G4VPhysicalVolume*)  const {}
  void ComputeDimensions (G4Trap&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Cons&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Orb&,const G4int,const G4VPhysicalVolume*)  const {}
  void ComputeDimensions (G4Torus&,const G4int,const G4VPhysicalVolume*)const {}
  void ComputeDimensions (G4Para&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Hype&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Box&,const G4int,const G4VPhysicalVolume*)  const {}
  void ComputeDimensions (G4Polycone&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Polyhedra&,const G4int,const G4VPhysicalVolume*) const {}

private:

  // Data members
  G4double fMinRadius;  // Inner radius of scoring sphere.
  G4double fMaxRadius;  // Outer radius of scoring sphere.
  G4double fDeltaTheta; // Delta theta angle
  G4double fDeltaPhi; // Delta phi angle
};

#endif // SPHEREPARAMETERISATION_HH
