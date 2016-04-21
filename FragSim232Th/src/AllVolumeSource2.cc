#include <iomanip>
#include "VSource.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "AllVolumeSource2.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "globals.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4RandomDirection.hh"
#include "TreeMaker.hh"

//AllVolumeSource2::AllVolumeSource2(G4VPhysicalVolume *world_vol)
//    : VSource(), fWorld(world_vol), fTarget(0)
//
//{
//    FindTargetAndInitialize();
//}

AllVolumeSource2::AllVolumeSource2(G4String worldName, G4String targetName, G4double thickness)
    : VSource(),
    fWorldName(worldName),
    fTargetName(targetName),
    fTarget(0),
    fThickness(thickness),
    fUseAllVolume(thickness==0),
    fFaceLimits(5,0),
    fFaceCounter(6,0)
{
    ::TreeMaker* tr = ::TreeMaker::GetTreeMakerPointer();
    DataBucket& db = tr->GetDataBucket();
    db.GetDoubleMap()["surf_thickness_um"]=fThickness/um;

}


AllVolumeSource2::~AllVolumeSource2()
{
//   if (fWorld!=0) delete fWorld; fWorld=0;

    G4int total = 0;
    for (unsigned int i=0; i<6; i++) total += fFaceCounter[i];

    G4cout << "\n==================================="
            << "\n AllVolumeSource2 - Face Counters  "
            << "\n-----------------------------------"
            << "\n 0  : " << std::setw(8) << fFaceCounter[0] << " --> " << 100.0*fFaceCounter[0]/total << " %"
            << "\n 1  : " << std::setw(8) << fFaceCounter[1] << " --> " << 100.0*fFaceCounter[1]/total << " %"
            << "\n 2  : " << std::setw(8) << fFaceCounter[2] << " --> " << 100.0*fFaceCounter[2]/total << " %"
            << "\n 3  : " << std::setw(8) << fFaceCounter[3] << " --> " << 100.0*fFaceCounter[3]/total << " %"
            << "\n 4  : " << std::setw(8) << fFaceCounter[4] << " --> " << 100.0*fFaceCounter[4]/total << " %"
            << "\n 5  : " << std::setw(8) << fFaceCounter[5] << " --> " << 100.0*fFaceCounter[5]/total << " %"
            << G4endl;
}

void AllVolumeSource2::Initialize()
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
        G4Exception("AllVolumeSource2::FindTargetAndInitialize()",
                    "AllVolumeSource2001",RunMustBeAborted,message,"");
    }

    fXHalfLength = box->GetXHalfLength();
    fYHalfLength = box->GetYHalfLength();
    fZHalfLength = box->GetZHalfLength();

    if (fThickness>fXHalfLength)
    {
        std::ostringstream message;
        message << "Thickness > half x length of target"
                << fXHalfLength/cm;
        G4Exception("AllVolumeSource2::FindTargetAndInitialize()",
                    "AllVolumeSource2002",RunMustBeAborted,message,"");
    }
    if (fThickness>fYHalfLength)
    {
        std::ostringstream message;
        message << "Thickness > half y length of target"
                << fYHalfLength/cm << G4endl;
        G4Exception("AllVolumeSource2::FindTargetAndInitialize()",
                    "AllVolumeSource2003",RunMustBeAborted,message,"");

    }
    if (fThickness>fZHalfLength)
    {
        std::ostringstream message;
        message << "Thickness > half z length of target"
                << fZHalfLength/cm << G4endl;
        G4Exception("AllVolumeSource2::FindTargetAndInitialize()",
                    "AllVolumeSource2004",RunMustBeAborted,message,"");

    }

    if (fThickness==0)
    {
        G4cout << "Thickness of zero found. Setting to half thickness = "
                << fThickness/cm << G4endl;
        fUseAllVolume = true;
    }

    SetupFaceLimits();
}

void AllVolumeSource2::GetTargetVolume()
{
    G4TransportationManager* tm = G4TransportationManager::GetTransportationManager();
    G4Navigator* nav = tm->GetNavigator(fWorldName);
    if (nav==0)
    {
        std::ostringstream message;
        message << "Unable to find the navigator for the world volume named " << fWorldName;
        G4Exception("AllVolumeSource2::GetTargetVolume()", "AllVolumeSource2002",
                    RunMustBeAborted, message,"");
    }
    G4VPhysicalVolume *wpv = 0;
    wpv = nav->GetWorldVolume();
    if (wpv==0)
    {
        std::ostringstream message;
        message << "Unable to find the world volume named " << fWorldName << " in the navigator";
        G4Exception("AllVolumeSource2::GetTargetVolume()", "AllVolumeSource2003",
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
        G4Exception("AllVolumeSource2::GetTargetVolume()", "AllVolumeSource2004",
                    RunMustBeAborted, message,"");
    }

    G4cout << *(fTarget->GetLogicalVolume()->GetSolid()) << G4endl;
}

G4ThreeVector AllVolumeSource2::GetRandomPosition()
{
    if (fTarget==0)
    {
        GetTargetVolume();
        Initialize();
    }

    if (fUseAllVolume)
    {
        return GetPositionFromWholeVolume();
    }
    else
    {
        return GetPositionFromSurfaceLayer();
    }
}
G4ThreeVector AllVolumeSource2::GetPositionFromWholeVolume()
{
    G4ThreeVector v = GetRandomPositionFromVolume(G4ThreeVector(0,0,0),
                                                  fXHalfLength,
                                                  fYHalfLength,
                                                  fZHalfLength);
    return fRotation*v+fTranslation;
}

G4ThreeVector AllVolumeSource2::GetPositionFromSurfaceLayer()
{
    G4ThreeVector v;
    G4int face_index = RandomlySelectFace();

    fFaceCounter[face_index]++;

    G4double xc=0, yc=0, zc=0;
    G4double xw=0, yw=0, zw=0;
    switch (face_index)
    {
    case 0 : // yz-plane @ x>0
        xc = fXHalfLength - 0.5*fThickness;
        yc = 0;
        zc = 0;

        xw = 0.5*fThickness;
        yw = fYHalfLength;
        zw = fZHalfLength - fThickness;
        break;
    case 1 : // xz-plane @ y<0
        xc = 0;
        yc = -1.0*(fYHalfLength - 0.5*fThickness);
        zc = 0;

        xw = fXHalfLength - fThickness;
        yw = 0.5*fThickness;
        zw = fZHalfLength - fThickness;
        break;
    case 2 : // yz-plane @ x<0
        xc = -1.0*(fXHalfLength - 0.5*fThickness);
        yc = 0;
        zc = 0;

        xw = 0.5*fThickness;
        yw = fYHalfLength;
        zw = fZHalfLength - fThickness;
        break;
    case 3 : // xz-plane @ y>0
        xc = 0;
        yc = fYHalfLength - 0.5*fThickness;
        zc = 0;

        xw = fXHalfLength - fThickness;
        yw = 0.5*fThickness;
        zw = fZHalfLength - fThickness;
        break;
    case 4 : // xy-plane @ z>0
        xc = 0;
        yc = 0;
        zc = fZHalfLength - 0.5*fThickness;

        xw = fXHalfLength;
        yw = fYHalfLength;
        zw = 0.5*fThickness;
        break;
    case 5 : // xy-place @ z<0
        xc = 0;
        yc = 0;
        zc = -1.0*(fZHalfLength - 0.5*fThickness);

        xw = fXHalfLength;
        yw = fYHalfLength;
        zw = 0.5*fThickness;
        break;
    default :
            std::ostringstream message;
            message << "Invalid side selected";
            G4Exception("AllVolumeSource2::GetPositionFromSurfaceLayer()",
                        "AllVolumeSource2006",
                        RunMustBeAborted, message,"");
            break;
    }
    v = GetRandomPositionFromVolume(G4ThreeVector(xc, yc, zc), xw, yw, zw);

    return fRotation*v+fTranslation;
}

G4ThreeVector AllVolumeSource2::GetRandomPositionFromVolume(const G4ThreeVector& center,
                                                            const G4double xw,
                                                            const G4double yw,
                                                            const G4double zw)
{
    G4double x,y,z;
    x = xw*(2.0*G4UniformRand()-1.0);
    y = yw*(2.0*G4UniformRand()-1.0);
    z = zw*(2.0*G4UniformRand()-1.0);

    G4ThreeVector pos(x,y,z);
    pos += center;
    return pos;

}

G4int AllVolumeSource2::RandomlySelectFace()
{
    G4int index = 0;
    G4double r = G4UniformRand();
    if      (r <= fFaceLimits[0])   index = 0;
    else if (r <= fFaceLimits[1])   index = 1;
    else if (r <= fFaceLimits[2])   index = 2;
    else if (r <= fFaceLimits[3])   index = 3;
    else if (r <= fFaceLimits[4])   index = 4;
    else                            index = 5;

    return index;
}

void AllVolumeSource2::SetupFaceLimits()
{
    std::vector<G4double> side_volumes(6,0);

    G4double xw, yw, zw;
    // side 0 volume
    xw = 0.5*fThickness;
    yw = fYHalfLength;
    zw = fZHalfLength - fThickness;
    side_volumes[0] = xw*yw*zw;

    // side 1 volume
    xw = fXHalfLength - fThickness;
    yw = 0.5*fThickness;
    zw = fZHalfLength - fThickness;
    side_volumes[1] = xw*yw*zw;

    // side 2 volume
    xw = 0.5*fThickness;
    yw = fYHalfLength;
    zw = fZHalfLength - fThickness;
    side_volumes[2] = xw*yw*zw;

    // side 3 volume
    xw = fXHalfLength - fThickness;
    yw = 0.5*fThickness;
    zw = fZHalfLength - fThickness;
    side_volumes[3] = xw*yw*zw;

    // side 4 volume
    xw = fXHalfLength;
    yw = fYHalfLength;
    zw = 0.5*fThickness;
    side_volumes[4] = xw*yw*zw;

    // side 5 volume
    xw = fXHalfLength;
    yw = fYHalfLength;
    zw = 0.5*fThickness;
    side_volumes[5] = xw*yw*zw;

    G4double total_volume = side_volumes[0]
                            + side_volumes[1]
                            + side_volumes[2]
                            + side_volumes[3]
                            + side_volumes[4]
                            + side_volumes[5];

    side_volumes[0] /= total_volume;
    side_volumes[1] /= total_volume;
    side_volumes[2] /= total_volume;
    side_volumes[3] /= total_volume;
    side_volumes[4] /= total_volume;
    side_volumes[5] /= total_volume;

    fFaceLimits[0] = side_volumes[0] + 0;
    fFaceLimits[1] = side_volumes[1] + fFaceLimits[0];
    fFaceLimits[2] = side_volumes[2] + fFaceLimits[1];
    fFaceLimits[3] = side_volumes[3] + fFaceLimits[2];
    fFaceLimits[4] = side_volumes[4] + fFaceLimits[3];
 //   fFaceLimits[5] = side_volumes[5] + fFaceLimits[4];

    std::streamsize prec = G4cout.precision(4);
    std::ios::fmtflags flags = G4cout.flags(std::ios::fixed);
    G4cout << "\n        ALL VOLUME SOURCE SETUP       "
            << "\n======================================================"
            << "\n Face :                Selection Criteria"
            << "\n------------------------------------------------------"
            << "\n 0    :   " << std::setw(8) << side_volumes[0] << "%  [" << std::setw(8) << 0  << ","<< std::setw(8) << fFaceLimits[0] << " )"
            << "\n 1    :   " << std::setw(8) << side_volumes[1] << "%  [" << std::setw(8) << fFaceLimits[0] << "," << std::setw(8) << fFaceLimits[1] << " )"
            << "\n 2    :   " << std::setw(8) << side_volumes[2] << "%  [" << std::setw(8) << fFaceLimits[1] << "," << std::setw(8) << fFaceLimits[2] << " )"
            << "\n 3    :   " << std::setw(8) << side_volumes[3] << "%  [" << std::setw(8) << fFaceLimits[2] << "," << std::setw(8) << fFaceLimits[3] << " )"
            << "\n 4    :   " << std::setw(8) << side_volumes[4] << "%  [" << std::setw(8) << fFaceLimits[3] << "," << std::setw(8) << fFaceLimits[4] << " )"
            << "\n 5    :   " << std::setw(8) << side_volumes[5] << "%  [" << std::setw(8) << fFaceLimits[4] << "," << std::setw(8) << 1 << " ]"
            << G4endl;

    G4cout.precision(prec);
    G4cout.flags(flags);
}
