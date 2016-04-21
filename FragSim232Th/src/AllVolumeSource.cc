#include "VSource.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "AllVolumeSource.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "globals.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4RandomDirection.hh"
#include "TreeMaker.hh"

//AllVolumeSource::AllVolumeSource(G4VPhysicalVolume *world_vol)
//    : VSource(), fWorld(world_vol), fTarget(0)
//
//{
//    FindTargetAndInitialize();
//}

AllVolumeSource::AllVolumeSource(G4String worldName, G4String targetName, G4double thickness)
    : VSource(),
    fWorldName(worldName),
    fTargetName(targetName),
    fTarget(0),
    fThickness(thickness)
{
    ::TreeMaker* tr = ::TreeMaker::GetTreeMakerPointer();
    DataBucket& db = tr->GetDataBucket();
    db.GetDoubleMap()["surf_thickness_um"]=fThickness/um;
}


AllVolumeSource::~AllVolumeSource()
{
//   if (fWorld!=0) delete fWorld; fWorld=0;
}

void AllVolumeSource::Initialize()
{

    fRotation = *fTarget->GetObjectRotation();
    fTranslation = fTarget->GetObjectTranslation();

    fPlaneNorm = fRotation.colX().cross(fRotation.colY());
    fTransform = G4Transform3D(fRotation, fTranslation);

    G4VSolid *solid = fTarget->GetLogicalVolume()->GetSolid();
    G4Box* box = dynamic_cast<G4Box*>(solid);
    if (box==0)
    {
        std::ostringstream message;
        message << "Found a non G4Box object as the target which is not supported";
        G4Exception("AllVolumeSource::FindTargetAndInitialize()",
                    "AllVolumeSource001",RunMustBeAborted,message,"");
    }

    fXHalfLength = box->GetXHalfLength();
    fYHalfLength = box->GetYHalfLength();
    fZHalfLength = box->GetZHalfLength();

    if (fThickness>fZHalfLength)
    {
        fThickness = fZHalfLength;
        G4cout << "Thickness > half thickness of target, thickness set to half thickness = "
                << fThickness/cm << G4endl;
    }
    if (fThickness==0)
    {
        G4cout << "Thickness of zero found. Setting to half thickness = "
                << fThickness/cm << G4endl;
        fThickness = fZHalfLength;
    }

}

void AllVolumeSource::GetTargetVolume()
{
    G4TransportationManager* tm = G4TransportationManager::GetTransportationManager();
    G4Navigator* nav = tm->GetNavigator(fWorldName);
    if (nav==0)
    {
        std::ostringstream message;
        message << "Unable to find the navigator for the world volume named " << fWorldName;
        G4Exception("AllVolumeSource::GetTargetVolume()", "AllVolumeSource002",
                    RunMustBeAborted, message,"");
    }
    G4VPhysicalVolume *wpv = 0;
    wpv = nav->GetWorldVolume();
    if (wpv==0)
    {
        std::ostringstream message;
        message << "Unable to find the world volume named " << fWorldName << " in the navigator";
        G4Exception("AllVolumeSource::GetTargetVolume()", "AllVolumeSource003",
                    RunMustBeAborted, message,"");
    }
    G4LogicalVolume *wlv = wpv->GetLogicalVolume();
    G4VPhysicalVolume*      dpv;

    G4int i=0;
    while (i < wlv->GetNoDaughters())
    {
        dpv = wlv->GetDaughter(i);
        if (dpv->GetName() == fTargetName)
        {
            fTarget = dpv;
            break;
        }
        i++;
    }
    if (fTarget==0)
    {
        std::ostringstream message;
        message << "Unable to find the target volume named " << fTargetName;
        G4Exception("AllVolumeSource::GetTargetVolume()", "AllVolumeSource004",
                    RunMustBeAborted, message,"");
    }

    G4cout << *(fTarget->GetLogicalVolume()->GetSolid()) << G4endl;
}

G4ThreeVector AllVolumeSource::GetRandomPosition()
{
    if (fTarget==0)
    {
        GetTargetVolume();
        Initialize();
    }

    G4double x=0,y=0,z=0;
    if (fXHalfLength>0 && fYHalfLength>0 && fZHalfLength>0)
    {
        x = fXHalfLength*(2.0*G4UniformRand()-1.0);
        y = fYHalfLength*(2.0*G4UniformRand()-1.0);
    }

    // Randomly choose the side of the target
    G4double i = G4UniformRand();
    if (i<0.5)
        z = -1.0*fZHalfLength;
    else
        z = fZHalfLength;

    // Randomly select depth from the target surface to
    // generate event up to the maximum depth set by fThickness
    G4double surf_depth = fThickness*G4UniformRand();
    if ( z < 0 )
        z += surf_depth;
    else
        z -= surf_depth;


    G4ThreeVector v(x,y,z);
    return fRotation*v+fTranslation;
}

