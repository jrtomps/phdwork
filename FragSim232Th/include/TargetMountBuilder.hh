#ifndef TARGETMOUNTBUILDER_H
#define TARGETMOUNTBUILDER_H

#include "G4TessellatedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

class TargetMountBuilder
{
private:
    G4TessellatedSolid *fMount;
    G4LogicalVolume*  fMountLV;
    G4TessellatedSolid *fShelf;
    G4LogicalVolume* fShelfLV;
    G4AssemblyVolume *fAssembly;

public:
    TargetMountBuilder();
    G4AssemblyVolume* GetAssembly();

private:
    void Assemble();
    void ConstructMount();
    void ConstructShelf();
};

#endif // TARGETMOUNTBUILDER_H
