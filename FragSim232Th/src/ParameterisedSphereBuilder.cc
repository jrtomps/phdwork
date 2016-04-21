#include <sstream>
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVParameterised.hh"
#include "G4AssemblyVolume.hh"
#include "G4Material.hh"
#include "G4Sphere.hh"
#include "G4String.hh"
#include "SphereParameterisation.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4VSensitiveDetector.hh"
#include "ParameterisedSphereBuilder.hh"


ParameterisedSphereBuilder::ParameterisedSphereBuilder()
    : fSolid(0),
    fMaterial(0),
    fMotherLog(0),
    fParamLog(0),
    fParameterisation(0),
    fParameterised(0),
    fAssembly(0)
{
}

ParameterisedSphereBuilder::ParameterisedSphereBuilder(G4double iradius,
                                                       G4double oradius, G4double deltaTheta,
                                                       G4double deltaPhi, G4Material* mat)
    : fSolid(new G4Sphere("fMotherSphereSolid",iradius,oradius,0*deg, 360*deg, 0*deg, 180*deg)),
    fMaterial(mat),
    fMotherLog(new G4LogicalVolume(fSolid,fMaterial,"fMotherLog")),
    fParamLog(new G4LogicalVolume(fSolid, fMaterial, "fParamLog")),
    fParameterisation(new SphereParameterisation(iradius, oradius, deltaTheta, deltaPhi)),
    fParameterised(new G4PVParameterised("fParameterised",   // Name
                                         fParamLog, // Logical volume
                                         fMotherLog, // Mother volume
                                         kZAxis,        // Axis
                                         180*deg/deltaTheta*360*deg/deltaPhi,            // Number of replicas
                                         fParameterisation)),
    fAssembly(0)
{
}

ParameterisedSphereBuilder::ParameterisedSphereBuilder(const ParameterisedSphereBuilder &obj)
    : fSolid(obj.fSolid),
    fMaterial(obj.fMaterial), // static object
    fMotherLog(obj.fMotherLog),
    fParamLog(obj.fParamLog),
    fParameterisation(obj.fParameterisation),
    fParameterised(obj.fParameterised),
    fAssembly(obj.fAssembly)
{
// this doesn't own any objects
}

ParameterisedSphereBuilder& ParameterisedSphereBuilder::operator=(ParameterisedSphereBuilder obj)
{
    swap(*this, obj);
    return *this;
}

ParameterisedSphereBuilder::~ParameterisedSphereBuilder()
{
// Deletion of these might break the software
//    if (fSolid!=0) { delete fSolid; fSolid=0;}
//    if (fMotherLog!=0) { delete fMotherLog; fMotherLog=0;}
//    if (fParamLog!=0) { delete fParamLog; fParamLog=0;}
//    if (fParameterised!=0) { delete fParameterised; fParameterised=0;}
//    if (fParameterisation!=0) { delete fParameterisation; fParameterisation=0;}
//    if (fAssembly!=0) { delete fAssembly; fAssembly=0;}
}

G4AssemblyVolume* ParameterisedSphereBuilder::GetADetector()
{
    if (fAssembly==0) Assemble();

    return fAssembly;
}

void ParameterisedSphereBuilder::SetSensitivity(G4VSensitiveDetector *sensitivity)
{
    fSensitivity = sensitivity;
    if (fParamLog!=0)
        fParamLog->SetSensitiveDetector(fSensitivity);
}

void ParameterisedSphereBuilder::Assemble()
{

    if (fMotherLog==0)
    {
        std::ostringstream message;
        message << "Default initializations cannot be used in Assemble()";
        G4Exception("ParameterisedSphereBuilder::Assemble()",
                                   "PSBuilder0000",
                                   RunMustBeAborted,
                                   message);
    }

    fAssembly = new G4AssemblyVolume;
    G4ThreeVector vec;
    fAssembly->AddPlacedVolume(fMotherLog,vec,0);

    G4VisAttributes attr(true);
    attr.SetColor(50.0/256.0, 0, 256.0/256.0);
    fParamLog->SetVisAttributes(attr);

}
