#ifndef PARAMETERIZEDSPHEREBUILDER_HH
#define PARAMETERIZEDSPHEREBUILDER_HH

#include <algorithm>
#include "G4LogicalVolume.hh"
#include "G4PVParameterised.hh"
#include "G4AssemblyVolume.hh"
#include "G4Material.hh"
#include "G4Sphere.hh"
#include "G4VSensitiveDetector.hh"
#include "SphereParameterisation.hh"

class ParameterisedSphereBuilder
{
private:
    G4Sphere                 *fSolid;
    G4Material            *fMaterial;
    G4LogicalVolume      *fMotherLog;
    G4LogicalVolume       *fParamLog;
    SphereParameterisation *fParameterisation;
    G4PVParameterised *fParameterised;
    G4VSensitiveDetector *fSensitivity;
    G4AssemblyVolume      *fAssembly;

public:
    ParameterisedSphereBuilder();
    ParameterisedSphereBuilder(G4double iradius, G4double oradius,
                               G4double deltaTheta, G4double deltaPhi, G4Material* mat);
    ParameterisedSphereBuilder(const ParameterisedSphereBuilder& obj);
    ParameterisedSphereBuilder& operator=(ParameterisedSphereBuilder obj);
    friend void swap (ParameterisedSphereBuilder& lhs, ParameterisedSphereBuilder& rhs);

    virtual ~ParameterisedSphereBuilder();
    G4AssemblyVolume* GetADetector();

    void SetSensitivity(G4VSensitiveDetector* sensitivity);
private:
    void Assemble();
};

inline void swap(ParameterisedSphereBuilder &lhs, ParameterisedSphereBuilder &rhs)
{
    std::swap(lhs.fSolid, rhs.fSolid);
    std::swap(lhs.fMaterial, rhs.fMaterial);
    std::swap(lhs.fMotherLog, rhs.fMotherLog);
    std::swap(lhs.fParamLog, rhs.fParamLog);
    std::swap(lhs.fParameterisation, rhs.fParameterisation);
    std::swap(lhs.fSensitivity, rhs.fSensitivity);
    std::swap(lhs.fAssembly, rhs.fAssembly);
}

#endif // PARAMETERIZEDSPHEREBUILDER_HH
