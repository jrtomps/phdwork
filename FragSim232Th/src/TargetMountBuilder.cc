#include "G4TessellatedSolid.hh"
#include "G4TriangularFacet.hh"
#include "G4QuadrangularFacet.hh"
#include "G4AssemblyVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4ReflectedSolid.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "TargetMountBuilder.hh"

TargetMountBuilder::TargetMountBuilder()
    : fMount(0),
      fMountLV(0),
      fShelf(0),
      fShelfLV(0),
      fAssembly(0)
{
}

G4AssemblyVolume* TargetMountBuilder::GetAssembly()
{
    if (fAssembly==0) Assemble();

    return fAssembly;
}

void TargetMountBuilder::Assemble()
{
    ConstructMount();
    ConstructShelf();

    fAssembly = new G4AssemblyVolume;

    G4ThreeVector mount_tr;
    G4RotationMatrix *mount_rot = new G4RotationMatrix();
    fAssembly->AddPlacedVolume(fMountLV,mount_tr,mount_rot);

    G4ThreeVector shelf_tr;
    G4RotationMatrix *shelf_rot = new G4RotationMatrix();
    fAssembly->AddPlacedVolume(fShelfLV,shelf_tr,shelf_rot);

}


void TargetMountBuilder::ConstructMount()
{
    // this method will construct the geometric shape
    // that is the 45 deg target mount. The various shapes
    // are taken straight from the model that was used to
    // generate the shop drawing. It is not perfect but very
    // close.

    // The points taken from the model are stored in the
    // file called FragSim02/notes/45DegTargetHolderGeoPoint.ods
    // The faces are generated using the data points it contains
    // but by turning them into triangles.

    fMount = new G4TessellatedSolid("mount_solid");

    //face0 --> three triangles
    G4TriangularFacet *f0a = new G4TriangularFacet(G4ThreeVector(-0.462*cm, 2.286*cm, -0.0762*cm),
                                                 G4ThreeVector(-0.46228*cm, 2.032*cm,-0.0762*cm),
                                                 G4ThreeVector(-1.36144*cm, 1.397*cm,-0.0762*cm),
                                                   ABSOLUTE);

    G4TriangularFacet *f0b = new G4TriangularFacet(G4ThreeVector(-0.46228*cm, 2.286*cm, -0.0762*cm),
                                                   G4ThreeVector(-1.36144*cm, 1.397*cm, -0.0762*cm),
                                                   G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                   ABSOLUTE);

    G4TriangularFacet *f0c = new G4TriangularFacet(G4ThreeVector(-0.46228*cm, 2.286*cm, -0.0762*cm),
                                                   G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                   G4ThreeVector(-0.82296*cm, 2.286*cm, -0.0762*cm),
                                                   ABSOLUTE);
    fMount->AddFacet(f0a);
    fMount->AddFacet(f0b);
    fMount->AddFacet(f0c);

    // face1 --> a quadrangle
    G4QuadrangularFacet *f1 = new G4QuadrangularFacet(G4ThreeVector(-0.46228*cm, 2.286*cm, -0.0762*cm),
                                                       G4ThreeVector(0.43434*cm, 2.286*cm, 0.82042*cm),
                                                       G4ThreeVector(0.43434*cm, 2.032*cm, 0.82042*cm),
                                                       G4ThreeVector(-0.46228*cm, 2.032*cm, -0.0762*cm),
                                                       ABSOLUTE);
    fMount->AddFacet(f1);

    // face2 --> three triangles
    G4TriangularFacet *f2a = new G4TriangularFacet(G4ThreeVector(0.43434*cm, 2.286*cm, 0.82042*cm),
                                                   G4ThreeVector(0.43434*cm, 2.286*cm, 1.1811*cm),
                                                   G4ThreeVector(0.43434*cm, 1.905*cm, 1.71958*cm),
                                                   ABSOLUTE);

    G4TriangularFacet *f2b = new G4TriangularFacet(G4ThreeVector(0.43434*cm, 2.286*cm, 0.82042*cm),
                                                   G4ThreeVector(0.43434*cm, 1.905*cm, 1.71958*cm),
                                                   G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                   ABSOLUTE);

    G4TriangularFacet *f2c = new G4TriangularFacet(G4ThreeVector(0.43434*cm, 2.286*cm, 0.82042*cm),
                                                   G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                   G4ThreeVector(0.43434*cm, 2.032*cm, 0.82042*cm),
                                                   ABSOLUTE);
    fMount->AddFacet(f2a);
    fMount->AddFacet(f2b);
    fMount->AddFacet(f2c);

    // face3 --> quadrangle
    G4QuadrangularFacet *f3 = new G4QuadrangularFacet(G4ThreeVector(0.43434*cm, 2.032*cm, 0.82042*cm),
                                                      G4ThreeVector(-0.46228*cm, 2.032*cm, -0.0762*cm),
                                                      G4ThreeVector(-1.36144*cm, 1.397*cm, -0.0762*cm),
                                                      G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                      ABSOLUTE);
    fMount->AddFacet(f3);

    // face4 --> quadrangle
    G4QuadrangularFacet *f4 = new G4QuadrangularFacet(G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                      G4ThreeVector(-0.07366*cm, 0.381*cm, 1.21158*cm),
                                                      G4ThreeVector(-0.85344*cm, 0.381*cm, 0.4318*cm),
                                                      G4ThreeVector(-1.36144*cm, 1.397*cm, -0.0762*cm),
                                                      ABSOLUTE);
    fMount->AddFacet(f4);

    // face5 --> a triangle
    G4TriangularFacet *f5 = new G4TriangularFacet(G4ThreeVector(-1.36144*cm, 1.397*cm, -0.0762*cm),
                                                  G4ThreeVector(-0.85344*cm, 0.381*cm, 0.4318*cm),
                                                  G4ThreeVector(-2.08026*cm, 0.381*cm, 0.64008*cm),
                                                  ABSOLUTE);
    fMount->AddFacet(f5);

    // face6 --> a triangle
    G4TriangularFacet *f6 = new G4TriangularFacet(G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                  G4ThreeVector(-0.28448*cm, 0.381*cm, 2.4384*cm),
                                                  G4ThreeVector(-0.07366*cm, 0.381*cm, 1.21158*cm),
                                                  ABSOLUTE);
    fMount->AddFacet(f6);

    // face7 --> a quadrangle
    G4QuadrangularFacet *f7 = new G4QuadrangularFacet(G4ThreeVector(-0.82296*cm, 2.286*cm, -0.0762*cm),
                                                      G4ThreeVector(0.43434*cm, 2.286*cm, 1.1811*cm),
                                                      G4ThreeVector(0.43434*cm, 2.286*cm, 0.82042*cm),
                                                      G4ThreeVector(-0.46228*cm, 2.286*cm, -0.0762*cm),
                                                      ABSOLUTE);
    fMount->AddFacet(f7);

    // face8 --> 4 triangles
    // this is the interior surface that will be set at position zero.
    G4TriangularFacet *f8a = new G4TriangularFacet(G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                   G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                   G4ThreeVector(-0.46228*cm, 0.635*cm, 2.6162*cm),
                                                   ABSOLUTE);

     G4TriangularFacet *f8b = new G4TriangularFacet(G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                    G4ThreeVector(-0.46228*cm, 0.635*cm, 2.6162*cm),
                                                    G4ThreeVector(0.43434*cm, 1.905*cm, 1.71958*cm),
                                                    ABSOLUTE);

     G4TriangularFacet *f8c = new G4TriangularFacet(G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                    G4ThreeVector(0.43434*cm, 1.905*cm, 1.71958*cm),
                                                    G4ThreeVector(0.43434*cm, 2.286*cm, 1.1811*cm),
                                                    ABSOLUTE);

     G4TriangularFacet *f8d = new G4TriangularFacet(G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                   G4ThreeVector(0.43434*cm, 2.286*cm, 1.1811*cm),
                                                   G4ThreeVector(-0.82296*cm, 2.286*cm, -0.0762*cm),
                                                   ABSOLUTE);
     fMount->AddFacet(f8a);
     fMount->AddFacet(f8b);
     fMount->AddFacet(f8c);
     fMount->AddFacet(f8d);

     // face9 --> 4 triangles
     G4TriangularFacet *f9a = new G4TriangularFacet(G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                    G4ThreeVector(-2.48412*cm, 0.9525*cm, 1.04648*cm),
                                                    G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                    ABSOLUTE);

     G4TriangularFacet *f9b = new G4TriangularFacet(G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                    G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                    G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                    ABSOLUTE);

     G4TriangularFacet *f9c = new G4TriangularFacet(G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                    G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                    G4ThreeVector(-0.68834*cm, 0.9525*cm, 2.84226*cm),
                                                    ABSOLUTE);

     G4TriangularFacet *f9d = new G4TriangularFacet(G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                    G4ThreeVector(-0.68834*cm, 0.9525*cm, 2.84226*cm),
                                                    G4ThreeVector(-0.46228*cm, 0.635*cm, 2.6162*cm),
                                                    ABSOLUTE);
     fMount->AddFacet(f9a);
     fMount->AddFacet(f9b);
     fMount->AddFacet(f9c);
     fMount->AddFacet(f9d);


     // face10 -->  4 triangles
     G4TriangularFacet *f10a = new G4TriangularFacet(G4ThreeVector(-1.7272*cm, 2.286*cm, 0.28956*cm),
                                                     G4ThreeVector(0.06858*cm, 2.286*cm, 2.08534*cm),
                                                     G4ThreeVector(-0.68834*cm, 1.21666*cm, 2.84226*cm),
                                                     ABSOLUTE);

     G4TriangularFacet *f10b = new G4TriangularFacet(G4ThreeVector(-1.7272*cm, 2.286*cm, 0.28956*cm),
                                                     G4ThreeVector(-0.68834*cm, 1.21666*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                     ABSOLUTE);

     G4TriangularFacet *f10c = new G4TriangularFacet(G4ThreeVector(-1.7272*cm, 2.286*cm, 0.28956*cm),
                                                     G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                     ABSOLUTE);

     G4TriangularFacet *f10d = new G4TriangularFacet(G4ThreeVector(-1.7272*cm, 2.286*cm, 0.28956*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.21666*cm, 1.04648*cm),
                                                     ABSOLUTE);
     fMount->AddFacet(f10a);
     fMount->AddFacet(f10b);
     fMount->AddFacet(f10c);
     fMount->AddFacet(f10d);


     // face11 --> quadrangle
     G4QuadrangularFacet *f11 = new G4QuadrangularFacet(G4ThreeVector(-2.32664*cm, 2.286*cm, 0.889*cm),
                                                        G4ThreeVector(-1.7272*cm, 2.286*cm, 0.28956*cm),
                                                        G4ThreeVector(-2.48412*cm, 1.21666*cm, 1.04648*cm),
                                                        G4ThreeVector(-2.48412*cm, 2.06502*cm, 1.04648*cm),
                                                        ABSOLUTE);
     fMount->AddFacet(f11);

     // face12 --> 5 triangles
     // this is more complicated and will be done as follows:
     // 012, 026, 234, 245, 256
     G4TriangularFacet *f12a = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 2.06502*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.21666*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f12b = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 2.06502*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f12c = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                     G4ThreeVector(-2.48412*cm, 0.9525*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.48412*cm, 0.381*cm, 1.04648*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f12d = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                     G4ThreeVector(-2.48412*cm, 0.381*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.48412*cm, 0.381*cm, 1.49352*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f12e = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 1.08458*cm, 1.2319*cm),
                                                     G4ThreeVector(-2.48412*cm, 0.381*cm, 1.49352*cm),
                                                     G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                     ABSOLUTE);
     fMount->AddFacet(f12a);
     fMount->AddFacet(f12b);
     fMount->AddFacet(f12c);
     fMount->AddFacet(f12d);
     fMount->AddFacet(f12e);

     // face13 --> 4 triangles
     G4TriangularFacet *f13a = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                     G4ThreeVector(-1.13792*cm, 1.74752*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 2.06502*cm, 2.84226*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f13b = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                     G4ThreeVector(-0.68834*cm, 2.06502*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.53086*cm, 2.286*cm, 2.68478*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f13c = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                     G4ThreeVector(-0.53086*cm, 2.286*cm, 2.68478*cm),
                                                     G4ThreeVector(-2.32664*cm, 2.286*cm, 0.889*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f13d = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                     G4ThreeVector(-2.32664*cm, 2.286*cm, 0.889*cm),
                                                     G4ThreeVector(-2.48412*cm, 2.06502*cm, 1.04648*cm),
                                                     ABSOLUTE);
     fMount->AddFacet(f13a);
     fMount->AddFacet(f13b);
     fMount->AddFacet(f13c);
     fMount->AddFacet(f13d);

     // face14 --> a quadrangle; the back of the mount
     G4QuadrangularFacet *f14 = new G4QuadrangularFacet(G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                        G4ThreeVector(-2.48412*cm, 0.381*cm, 1.49352*cm),
                                                        G4ThreeVector(-1.13792*cm, 0.381*cm, 2.84226*cm),
                                                        G4ThreeVector(-1.13792*cm, 1.74752*cm, 2.84226*cm),
                                                        ABSOLUTE);
     fMount->AddFacet(f14);

     // face15 --> 5 triangles
     // this one is also more complicated and is laid out as follows:
     // 012, 023, 036, 345, 356
     G4TriangularFacet *f15a = new G4TriangularFacet(G4ThreeVector(-1.13792*cm, 0.381*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 0.381*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 0.9525*cm, 2.84226*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f15b = new G4TriangularFacet(G4ThreeVector(-1.13792*cm, 0.381*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 0.9525*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f15c = new G4TriangularFacet(G4ThreeVector(-1.13792*cm, 0.381*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                     G4ThreeVector(-1.13792*cm, 1.74752*cm, 2.84226*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f15d = new G4TriangularFacet(G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 1.21666*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 2.06502*cm, 2.84226*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f15e = new G4TriangularFacet(G4ThreeVector(-0.87376*cm, 1.08458*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 2.06502*cm, 2.84226*cm),
                                                     G4ThreeVector(-1.13792*cm, 1.74752*cm, 2.84226*cm),
                                                     ABSOLUTE);
     fMount->AddFacet(f15a);
     fMount->AddFacet(f15b);
     fMount->AddFacet(f15c);
     fMount->AddFacet(f15d);
     fMount->AddFacet(f15e);


     // face16 --> quadrangle
     G4QuadrangularFacet *f16 = new G4QuadrangularFacet(G4ThreeVector(-0.68834*cm, 1.21666*cm, 2.84226*cm),
                                                        G4ThreeVector(0.06858*cm, 2.286*cm, 2.08534*cm),
                                                        G4ThreeVector(-0.53086*cm, 2.286*cm, 2.68478*cm),
                                                        G4ThreeVector(-0.68834*cm, 2.06502*cm, 2.84226*cm),
                                                        ABSOLUTE);
     fMount->AddFacet(f16);

     // face17 --> 3 quadrangles
     G4QuadrangularFacet *f17a = new G4QuadrangularFacet(G4ThreeVector(-0.07366*cm, 0.381*cm, 1.21158*cm),
                                                         G4ThreeVector(-0.28448*cm, 0.381*cm, 2.4384*cm),
                                                         G4ThreeVector(-0.68834*cm, 0.381*cm, 2.84226*cm),
                                                         G4ThreeVector(-1.13792*cm, 0.381*cm, 2.84226*cm),
                                                         ABSOLUTE);
     G4QuadrangularFacet *f17b = new G4QuadrangularFacet(G4ThreeVector(-0.07366*cm, 0.381*cm, 1.21158*cm),
                                                         G4ThreeVector(-1.13792*cm, 0.381*cm, 2.84226*cm),
                                                         G4ThreeVector(-2.48412*cm, 0.381*cm, 1.49352*cm),
                                                         G4ThreeVector(-0.85344*cm, 0.381*cm, 0.4318*cm),
                                                         ABSOLUTE);
     G4QuadrangularFacet *f17c = new G4QuadrangularFacet(G4ThreeVector(-2.48412*cm, 0.381*cm, 1.49352*cm),
                                                         G4ThreeVector(-2.48412*cm, 0.381*cm, 1.04648*cm),
                                                         G4ThreeVector(-2.08026*cm, 0.381*cm, 0.64008*cm),
                                                         G4ThreeVector(-0.85344*cm, 0.381*cm, 0.4318*cm),
                                                         ABSOLUTE);
     fMount->AddFacet(f17a);
     fMount->AddFacet(f17b);
     fMount->AddFacet(f17c);

     // face18 --> quadrangle
     G4QuadrangularFacet *f18 = new G4QuadrangularFacet(G4ThreeVector(-2.32664*cm, 2.286*cm, 0.889*cm),
                                                        G4ThreeVector(-0.53086*cm, 2.286*cm, 2.68478*cm),
                                                        G4ThreeVector(0.06858*cm, 2.286*cm, 2.08534*cm),
                                                        G4ThreeVector(-1.7272*cm, 2.286*cm, 0.28956*cm),
                                                        ABSOLUTE);
     fMount->AddFacet(f18);

     // face19 --> quadrangle
     G4QuadrangularFacet *f19 = new G4QuadrangularFacet(G4ThreeVector(-1.13792*cm, 1.74752*cm, 2.84226*cm),
                                                        G4ThreeVector(-2.48412*cm, 1.74752*cm, 1.49352*cm),
                                                        G4ThreeVector(-2.48412*cm, 0.381*cm, 1.49352*cm),
                                                        G4ThreeVector(-1.13792*cm, 0.381*cm, 2.84226*cm),
                                                        ABSOLUTE);
     fMount->AddFacet(f19);

     // face20 --> 4 triangles
     // more complicated... will be done as in the following combos
     // 015, 123, 134, 145

     G4TriangularFacet *f20a = new G4TriangularFacet(G4ThreeVector(0.43434*cm, 1.905*cm, 1.71958*cm),
                                                     G4ThreeVector(-0.46228*cm, 0.635*cm, 2.6162*cm),
                                                     G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f20b = new G4TriangularFacet(G4ThreeVector(-0.46228*cm, 0.635*cm, 2.6162*cm),
                                                     G4ThreeVector(-0.68834*cm, 0.9525*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.68834*cm, 0.381*cm, 2.84226*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f20c = new G4TriangularFacet(G4ThreeVector(-0.46228*cm, 0.635*cm, 2.6162*cm),
                                                     G4ThreeVector(-0.68834*cm, 0.381*cm, 2.84226*cm),
                                                     G4ThreeVector(-0.28448*cm, 0.381*cm, 2.4384*cm),
                                                     ABSOLUTE);
     G4TriangularFacet *f20d = new G4TriangularFacet(G4ThreeVector(-0.46228*cm, 0.635*cm, 2.6162*cm),
                                                     G4ThreeVector(-0.28448*cm, 0.381*cm, 2.4384*cm),
                                                     G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                     ABSOLUTE);
     fMount->AddFacet(f20a);
     fMount->AddFacet(f20b);
     fMount->AddFacet(f20c);
     fMount->AddFacet(f20d);

     // face21 --> 4 triangles
     // these are like the face 20
     // 012, 025, 235, 345
     G4TriangularFacet *f21a = new G4TriangularFacet(G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                     G4ThreeVector(-1.36144*cm, 1.397*cm, -0.0762*cm),
                                                     G4ThreeVector(-2.08026*cm, 0.381*cm, 0.64008*cm),
                                                     ABSOLUTE);
     G4TriangularFacet* f21b = new G4TriangularFacet(G4ThreeVector(-1.36144*cm, 1.905*cm, -0.0762*cm),
                                                     G4ThreeVector(-2.08026*cm, 0.381*cm, 0.64008*cm),
                                                     G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                     ABSOLUTE);
     G4TriangularFacet* f21c = new G4TriangularFacet(G4ThreeVector(-2.08026*cm, 0.381*cm, 0.64008*cm),
                                                     G4ThreeVector(-2.48412*cm, 0.381*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                     ABSOLUTE);
     G4TriangularFacet* f21d = new G4TriangularFacet(G4ThreeVector(-2.48412*cm, 0.381*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.48412*cm, 0.9525*cm, 1.04648*cm),
                                                     G4ThreeVector(-2.2606*cm, 0.635*cm, 0.82042*cm),
                                                     ABSOLUTE);
     fMount->AddFacet(f21a);
     fMount->AddFacet(f21b);
     fMount->AddFacet(f21c);
     fMount->AddFacet(f21d);

     // face22 --> quadrangle
     G4QuadrangularFacet *f22 = new G4QuadrangularFacet(G4ThreeVector(0.43434*cm, 2.032*cm, 0.82042*cm),
                                                        G4ThreeVector(0.43434*cm, 1.397*cm, 1.71958*cm),
                                                        G4ThreeVector(-1.36144*cm, 1.397*cm, -0.0762*cm),
                                                        G4ThreeVector(-0.46228*cm, 2.032*cm, -0.0762*cm),
                                                        ABSOLUTE);
     fMount->AddFacet(f22);

     fMount->SetSolidClosed(true);

     G4RotationMatrix rot;
     G4ThreeVector transl(1.36144*cm, -0.635*cm, -1.7184*cm);
     G4Transform3D tr(rot, transl);
     G4ReflectedSolid *reflMount = new G4ReflectedSolid("reflMount", fMount, tr);

     //     G4ReflectZ3D reflectX;
//     G4ReflectedSolid *reflMount = new G4ReflectedSolid("reflMount", fMount, reflectX);


     G4Material *aluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

//     fMountLV = new G4LogicalVolume(reflMount,aluminum,"Mount_LV");
     fMountLV = new G4LogicalVolume(reflMount, aluminum, "Mount_LV");
     G4VisAttributes attr;
     attr.SetColor(0.80,0.80,0.80);
     attr.SetForceSolid(true);
     attr.SetForceAuxEdgeVisible(true);
     fMountLV->SetVisAttributes(attr);


}


void TargetMountBuilder::ConstructShelf()
{
    fShelf = new G4TessellatedSolid("shelf_solid");

    //face50 --> quadrangle
    G4QuadrangularFacet *f50 = new G4QuadrangularFacet(G4ThreeVector(-1.28016*cm, 0.381*cm, 0.635*cm),
                                                       G4ThreeVector(-1.28016*cm, 0*cm, 0.635*cm),
                                                       G4ThreeVector(-2.18186*cm, 0*cm, 2.19964*cm),
                                                       G4ThreeVector(-2.18186*cm, 0.381*cm, 2.19964*cm),
                                                       ABSOLUTE);

    fShelf->AddFacet(f50);

    //face51 --> quadrangle
    G4QuadrangularFacet *f51 = new G4QuadrangularFacet(G4ThreeVector(-0.5461*cm, 0.381*cm, 0.635*cm),
                                                       G4ThreeVector(-0.5461*cm, 0*cm, 0.635*cm),
                                                       G4ThreeVector(-1.28016*cm, 0*cm, 0.635*cm),
                                                       G4ThreeVector(-1.28016*cm, 0.381*cm, 0.635*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f51);

    // face52 --> quadrangle
    G4QuadrangularFacet *f52 = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 0.381*cm, 2.19964*cm),
                                                        G4ThreeVector(0.35814*cm, 0*cm, 2.19964*cm),
                                                        G4ThreeVector(-0.5461*cm, 0*cm, 0.635*cm),
                                                        G4ThreeVector(-0.5461*cm, 0.381*cm, 0.635*cm),
                                                        ABSOLUTE);

    fShelf->AddFacet(f52);

    //face53 --> more complicated...
    // 3 quadrangles...
    // 0123, 0349, 5678
    G4QuadrangularFacet *f53a = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 0.381*cm, 2.19964*cm),
                                                        G4ThreeVector(-0.5461*cm, 0.381*cm, 0.635*cm),
                                                        G4ThreeVector(-1.28016*cm, 0.381*cm, 0.635*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.381*cm, 2.19964*cm),
                                                        ABSOLUTE);

    G4QuadrangularFacet *f53b = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 0.381*cm, 2.19964*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.381*cm, 2.19964*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.381*cm, 2.54*cm),
                                                        G4ThreeVector(0.35814*cm, 0.381*cm, 2.54*cm),
                                                        ABSOLUTE);

    G4QuadrangularFacet* f53c = new G4QuadrangularFacet(G4ThreeVector(-1.43764*cm, 0.381*cm, 2.54*cm),
                                                        G4ThreeVector(-1.05664*cm, 0.381*cm, 2.921*cm),
                                                        G4ThreeVector(-0.76708*cm, 0.381*cm, 2.921*cm),
                                                        G4ThreeVector(-0.38608*cm, 0.381*cm, 2.54*cm),
                                                        ABSOLUTE);

    fShelf->AddFacet(f53a);
    fShelf->AddFacet(f53b);
    fShelf->AddFacet(f53c);

    // face54 --> 2 quadrangles
    // 0145, 1234
    G4QuadrangularFacet* f54a = new G4QuadrangularFacet(G4ThreeVector(-2.18186*cm, 0*cm, 4.445*cm),
                                                       G4ThreeVector(-2.18186*cm, 0*cm, 2.19964*cm),
                                                       G4ThreeVector(0.35814*cm, 0*cm, 2.19964*cm),
                                                       G4ThreeVector(0.35814*cm, 0*cm, 4.445*cm),
                                                       ABSOLUTE);
    G4QuadrangularFacet* f54b = new G4QuadrangularFacet(G4ThreeVector(-2.18186*cm, 0*cm, 2.19964*cm),
                                                        G4ThreeVector(-1.28016*cm, 0*cm, 0.635*cm),
                                                        G4ThreeVector(-0.5461*cm, 0*cm,	0.635*cm),
                                                        G4ThreeVector(0.35814*cm, 0*cm, 2.19964*cm),
                                                        ABSOLUTE);
    fShelf->AddFacet(f54a);
    fShelf->AddFacet(f54b);

    // face55 --> 3 quandrangles
    // 0167, 1456, 1234
    G4QuadrangularFacet* f55a = new G4QuadrangularFacet(G4ThreeVector(-2.18186*cm, 0*cm, 2.19964*cm),
                                                        G4ThreeVector(-2.18186*cm, 0*cm, 4.445*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.381*cm, 2.54*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.381*cm, 2.19964*cm),
                                                        ABSOLUTE);
    G4QuadrangularFacet* f55b = new G4QuadrangularFacet(G4ThreeVector(-2.18186*cm, 0*cm, 4.445*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.508*cm, 2.921*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.508*cm, 2.54*cm),
                                                        G4ThreeVector(-2.18186*cm, 0.381*cm, 2.54*cm),
                                                        ABSOLUTE);
    G4QuadrangularFacet* f55c = new G4QuadrangularFacet(G4ThreeVector(-2.18186*cm, 0*cm, 4.445*cm),
                                                         G4ThreeVector(-2.18186*cm, 2.54*cm, 4.445*cm),
                                                         G4ThreeVector(-2.18186*cm, 2.54*cm, 2.921*cm),
                                                         G4ThreeVector(-2.18186*cm, 0.508*cm, 2.921*cm),
                                                         ABSOLUTE);
    fShelf->AddFacet(f55a);
    fShelf->AddFacet(f55b);
    fShelf->AddFacet(f55c);

    // face56 --> quadrangle
    G4QuadrangularFacet *f56 = new G4QuadrangularFacet(G4ThreeVector(-2.18186*cm, 0*cm, 4.445*cm),
                                                        G4ThreeVector(0.35814*cm, 0*cm, 4.445*cm),
                                                        G4ThreeVector(0.35814*cm, 2.54*cm, 4.445*cm),
                                                        G4ThreeVector(-2.18186*cm, 2.54*cm, 4.445*cm),
                                                        ABSOLUTE);
    fShelf->AddFacet(f56);

    // face57 --> 3 quadrangles
    // 0167, 1234, 1456
    G4QuadrangularFacet *f57a = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 2.54*cm, 4.445*cm),
                                                        G4ThreeVector(0.35814*cm, 0*cm, 4.445*cm),
                                                        G4ThreeVector(0.35814*cm, 0.508*cm, 2.921*cm),
                                                        G4ThreeVector(0.35814*cm, 2.54*cm, 2.921*cm),
                                                        ABSOLUTE);
    G4QuadrangularFacet *f57b = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 0*cm, 4.445*cm),
                                                        G4ThreeVector(0.35814*cm, 0*cm, 2.19964*cm),
                                                        G4ThreeVector(0.35814*cm, 0.381*cm, 2.19964*cm),
                                                        G4ThreeVector(0.35814*cm, 0.381*cm, 2.54*cm),
                                                       ABSOLUTE);
    G4QuadrangularFacet *f57c = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 0*cm, 4.445*cm),
                                                        G4ThreeVector(0.35814*cm, 0.381*cm, 2.54*cm),
                                                        G4ThreeVector(0.35814*cm, 0.508*cm, 2.54*cm),
                                                        G4ThreeVector(0.35814*cm, 0.508*cm, 2.921*cm),
                                                        ABSOLUTE);
    fShelf->AddFacet(f57a);
    fShelf->AddFacet(f57b);
    fShelf->AddFacet(f57c);

    // face58 --> 2 quadrangles
    // 0167, 2345
    G4QuadrangularFacet *f58a = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 2.54*cm, 2.921*cm),
                                                       G4ThreeVector(0.35814*cm, 0.508*cm, 2.921*cm),
                                                       G4ThreeVector(-2.18186*cm, 0.508*cm, 2.921*cm),
                                                       G4ThreeVector(-2.18186*cm, 2.54*cm, 2.921*cm),
                                                       ABSOLUTE);
    G4QuadrangularFacet *f58b = new G4QuadrangularFacet(G4ThreeVector(-0.76708*cm, 0.508*cm, 2.921*cm),
                                                        G4ThreeVector(-0.76708*cm, 0.381*cm, 2.921*cm),
                                                        G4ThreeVector(-1.05664*cm, 0.381*cm, 2.921*cm),
                                                        G4ThreeVector(-1.05664*cm, 0.508*cm, 2.921*cm),
                                                        ABSOLUTE);
    fShelf->AddFacet(f58a);
    fShelf->AddFacet(f58b);

    // face59 --> quadrangle
    G4QuadrangularFacet *f59 = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 0.508*cm, 2.921*cm),
                                                       G4ThreeVector(0.35814*cm, 0.508*cm, 2.54*cm),
                                                       G4ThreeVector(-0.38608*cm, 0.508*cm, 2.54*cm),
                                                       G4ThreeVector(-0.76708*cm, 0.508*cm, 2.921*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f59);

    // face60 --> quadrangle
    G4QuadrangularFacet *f60 = new G4QuadrangularFacet(G4ThreeVector(-1.05664*cm, 0.508*cm, 2.921*cm),
                                                       G4ThreeVector(-1.43764*cm, 0.508*cm, 2.54*cm),
                                                       G4ThreeVector(-2.18186*cm, 0.508*cm, 2.54*cm),
                                                       G4ThreeVector(-2.18186*cm, 0.508*cm, 2.921*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f60);

    // face61 --> quadrangle
    G4QuadrangularFacet *f61 = new G4QuadrangularFacet(G4ThreeVector(-1.05664*cm, 0.508*cm, 2.921*cm),
                                                       G4ThreeVector(-1.05664*cm, 0.381*cm, 2.921*cm),
                                                       G4ThreeVector(-1.43764*cm, 0.381*cm, 2.54*cm),
                                                       G4ThreeVector(-1.43764*cm, 0.508*cm, 2.54*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f61);

    // face62 --> quadrangle
    G4QuadrangularFacet *f62 = new G4QuadrangularFacet(G4ThreeVector(-0.38608*cm, 0.508*cm, 2.54*cm),
                                                       G4ThreeVector(-0.38608*cm, 0.381*cm, 2.54*cm),
                                                       G4ThreeVector(-0.76708*cm, 0.381*cm, 2.921*cm),
                                                       G4ThreeVector(-0.76708*cm, 0.508*cm, 2.921*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f62);

    // face63 --> quadrangle
    G4QuadrangularFacet *f63 = new G4QuadrangularFacet(G4ThreeVector(0.35814*cm, 0.508*cm, 2.54*cm),
                                                       G4ThreeVector(0.35814*cm, 0.381*cm, 2.54*cm),
                                                       G4ThreeVector(-0.38608*cm, 0.381*cm, 2.54*cm),
                                                       G4ThreeVector(-0.38608*cm, 0.508*cm, 2.54*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f63);

    // face64 --> quadrangle
    G4QuadrangularFacet *f64 = new G4QuadrangularFacet(G4ThreeVector(-1.43764*cm, 0.508*cm, 2.54*cm),
                                                       G4ThreeVector(-1.43764*cm, 0.381*cm, 2.54*cm),
                                                       G4ThreeVector(-2.18186*cm, 0.381*cm, 2.54*cm),
                                                       G4ThreeVector(-2.18186*cm, 0.508*cm, 2.54*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f64);

    // face65 --> quadrangle
    G4QuadrangularFacet *f65 = new G4QuadrangularFacet(G4ThreeVector(-2.18186*cm, 2.54*cm, 4.445*cm),
                                                       G4ThreeVector(0.35814*cm, 2.54*cm, 4.445*cm),
                                                       G4ThreeVector(0.35814*cm, 2.54*cm, 2.921*cm),
                                                       G4ThreeVector(-2.18186*cm, 2.54*cm, 2.921*cm),
                                                       ABSOLUTE);
    fShelf->AddFacet(f65);

    fShelf->SetSolidClosed(true);

    G4RotationMatrix rot;
    G4ThreeVector transl(1.36144*cm, -0.635*cm, -1.7184*cm);
    G4Transform3D tr(rot, transl);
    G4ReflectedSolid *reflShelf = new G4ReflectedSolid("reflShelf", fShelf, tr);

    G4Material *aluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    fShelfLV = new G4LogicalVolume(reflShelf,aluminum,"Shelf_LV");
    G4VisAttributes attr;
    attr.SetColor(0.80,0.80,0.80);
    attr.SetForceSolid(true);
    fShelfLV->SetVisAttributes(attr);



}
