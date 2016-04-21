// FragSimSource.cc
//
// Jeromy Tompkins
// 10/19/2011
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "G4RandomDirection.hh"
#include "FragSimSource.hh"
#include "FragSimSourceMessenger.hh"
#include "VSource.hh"
#include "TreeMaker.hh"

FragSimSource::FragSimSource(const G4double r, 
                             const G4Transform3D& trans)
    : VSource(trans),
      fRadius(r),
      fRotThickness(0),
      fTar(),
      fSType(kVolume),
      fSurfaceThickness(1.0*um),
      messenger(new FragSimSourceMessenger(this))
{
    std::cout << "Constructing FragSimSource" << std::endl;
    SetTransform(trans);
    fRotThickness = ComputeRotThickness();
    SetSurfaceThickness(1.0*um);
    SetType("volume");
}

FragSimSource::FragSimSource(const G4double r, 
                             const G4Transform3D& trans,
                             const FSSType type)
    : VSource(trans),
      fRadius(r),
      fRotThickness(0),
      fTar(),
      fSType(type),
      fSurfaceThickness(0),
      messenger(new FragSimSourceMessenger(this))
{
    std::cout << "Constructing FragSimSource" << std::endl;
    SetTransform(trans);
    fRotThickness = ComputeRotThickness();
    SetSurfaceThickness(1.0*um);
    SetType(type);
}

FragSimSource::FragSimSource(const G4double r, 
                             const G4Transform3D& trans,
                             const FSSType type,
                             const G4double surf_thickness)
    : VSource(trans),
      fRadius(r),
      fRotThickness(0),
      fTar(),
      fSType(type),
      fSurfaceThickness(surf_thickness),
      messenger(new FragSimSourceMessenger(this))
{
    std::cout << "Constructing FragSimSource" << std::endl;
    SetTransform(trans);
    fRotThickness = ComputeRotThickness();
    SetSurfaceThickness(surf_thickness);
    SetType(type);
}

FragSimSource::FragSimSource(FragSimSource const& obj)
    : VSource(obj),
      fRadius(obj.fRadius),
      fRotThickness(obj.fRotThickness),
      fTar(),
      fSType(obj.fSType),
      fSurfaceThickness(obj.fSurfaceThickness),
      messenger(new FragSimSourceMessenger(*obj.messenger))
{
    std::cout << "Copy Constructing FragSimSource" << std::endl;
    SetTransform(obj.fTransform);
    SetSurfaceThickness(obj.fSurfaceThickness);
    SetType(obj.fSType);

}

FragSimSource&
FragSimSource::operator=(FragSimSource const& obj)
{
    std::cout << "Assignment Operator FragSimSource" << std::endl;
    if (this != &obj)
    {
        fRadius       = obj.fRadius;
        fRotThickness = obj.fRotThickness;
        fTar          = obj.fTar;
        SetTransform(obj.fTransform);
        SetSurfaceThickness(obj.fSurfaceThickness);
        SetType(obj.fSType);

        FragSimSourceMessenger *newmess = new FragSimSourceMessenger(*obj.messenger);
        delete messenger;
        messenger = newmess;

        VSource::operator=(obj);
    }
    return *this;
}

FragSimSource::~FragSimSource(void)
{
    delete messenger;
}

G4ThreeVector 
FragSimSource::GetRandomPosition(void)
{
    G4ThreeVector pos;
    G4double x, y;
    do {
        x = fRadius*(2*G4UniformRand()-1);
        y = fRadius*(2*G4UniformRand()-1);
    } while (sqrt(x*x+y*y) > fRadius) ;
    
    pos.setX(x);
    pos.setY(y);
    pos.setZ(ComputeZ(pos.x(), pos.y()));
    pos += fTranslation;
    return pos;
}

G4double 
FragSimSource::GetRadius(void) const
{
    return fRadius;
}

void     
FragSimSource::SetRadius(const G4double r)
{
    fRadius = r;
}

void FragSimSource::SetType(FSSType type)
{
    if (type==kVolume) SetType("volume");
    else if (type==kSurfaceLayer) SetType("surface_layer");
    else if (type==kSurface) SetType("surface");
}

void FragSimSource::SetType(G4String type_name)
{
    ::TreeMaker* tm = ::TreeMaker::GetTreeMakerPointer();
    DataBucket& db = tm->GetDataBucket();
    if (type_name=="volume")
    {
        G4cout << "Source type set to volume" << G4endl;
        fSType = kVolume;
        db.GetStringMap()["source_type"] = "volume";
        if (fSurfaceThickness!=0.5*fRotThickness)
            SetSurfaceThickness(0.5*fRotThickness);
    }
    else if (type_name == "surface_layer")
    {
        if (fSurfaceThickness>0)
        {
            G4cout << "Source type set to surface_layer" << G4endl;
            fSType = kSurfaceLayer;
            db.GetStringMap()["source_type"] = "surface_layer";
        }
        else
        {
            G4cout << "Cannot set type to surface layer if surface thickness is zero" << G4endl;
            G4cout << "Set the thickness to a nonzero value first" << G4endl;
        }
    }
    else if (type_name=="surface")
    {
        G4cout << "Source type set to surface" << G4endl;
        fSType = kSurface;
        db.GetStringMap()["source_type"] = "surface";
        if (fSurfaceThickness!=0) SetSurfaceThickness(0);
    }
    else
    {
        G4cout << type_name
                << " is an invalid source type. No change has been made"
                << G4endl;
    }

}

void FragSimSource::SetSurfaceThickness(G4double thick)
{
    if (thick < 0)
    {
        G4cout << "Surface layer thickness cannot be set to negative value" << G4endl;
        G4cout << "Surface layer thickness remains unchanged with value " << fSurfaceThickness/um << " um" << G4endl;
        return;
    }

    ::TreeMaker* tm = ::TreeMaker::GetTreeMakerPointer();
    DataBucket* bucket = &tm->GetDataBucket();
    if (thick == 0)
    {
        fSurfaceThickness = 0;
        if (fSType!=kSurface)
        {
            G4cout << "Because surface layer thickness was set to zero..." << G4endl;
            SetType("surface");
        }
    }
    else if (thick>=0.5*fRotThickness)
    {
        fSurfaceThickness = 0.5*fRotThickness;
        if (fSType!=kVolume)
        {
            G4cout << "Because surface layer thickness set to value greater than or eqaul to "
                    << "the half width of rotated thickness... " << G4endl;
            SetType("volume");
        }
    }
    else
    {
        fSurfaceThickness = thick;
        if (fSType != kSurfaceLayer)
        {
            G4cout << "Because surface layer thickness set to nonzero value..." << G4endl;
            SetType("surface_layer");
        }
    }
    bucket->GetDoubleMap()["surf_thickness_um"]=fSurfaceThickness/um;

}

G4double
FragSimSource::ComputeZ(const G4double x,
                        const G4double y) const
{
    G4double depth=0;
    G4double i;

    switch (fSType) {
    case kVolume:
        depth = fRotThickness*(G4UniformRand() - 0.5);
        break;
    case kSurface:
        i = G4UniformRand();
        if (i<0.5)
            depth = -0.5*fRotThickness;
        else
            depth = 0.5*fRotThickness;
        break;
    case kSurfaceLayer:
        i = G4UniformRand();
        if (i<0.5)
            depth = -0.5*fRotThickness;
        else
            depth = 0.5*fRotThickness;

        G4double surf_depth = fSurfaceThickness*G4UniformRand();
        if ( depth < 0 )
            depth += surf_depth;
        else
            depth -= surf_depth;
    }

    return (fPlaneNorm.x()*x+fPlaneNorm.y()*y)/fPlaneNorm.z() + depth;
}

G4double
FragSimSource::ComputeRotThickness(void)
{
    G4ThreeVector p0( 0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), -0.5*fTar.GetThickness());
    G4ThreeVector p1(-0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), -0.5*fTar.GetThickness());
    G4ThreeVector p2(-0.5*fTar.GetLength(),  0.5*fTar.GetWidth(), -0.5*fTar.GetThickness());
    p0 *= fTransform.getRotation();
    p1 *= fTransform.getRotation();
    p2 *= fTransform.getRotation();

    G4ThreeVector q0( 0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), 0.5*fTar.GetThickness());
    G4ThreeVector q1(-0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), 0.5*fTar.GetThickness());
    G4ThreeVector q2(-0.5*fTar.GetLength(),  0.5*fTar.GetWidth(), 0.5*fTar.GetThickness());
    q0 *= fTransform.getRotation();
    q1 *= fTransform.getRotation();
    q2 *= fTransform.getRotation();

    G4ThreeVector pa = p1 - p0;
    G4ThreeVector pb = p2 - p0;

    G4ThreeVector qa = q1 - q0;
    G4ThreeVector qb = q2 - q0;

    G4ThreeVector np = pa.cross(pb);
    G4ThreeVector nq = qa.cross(qb);
    np.setMag(1.0);
    nq.setMag(1.0);

    G4double dp = -1.0*(np.x()*p0.x()+np.y()*p0.y()+np.z()*p0.z());
    G4double dq = -1.0*(nq.x()*q0.x()+nq.y()*q0.y()+nq.z()*q0.z());

    G4double zp = -1.0*dp/np.z();
    G4double zq = -1.0*dq/nq.z();

    std::cout << "Rotated Thickness = " << fabs(zp-zq)/cm << " cm" << std::endl;
    std::cout << "Scale factor = " << fabs(zp-zq)/fTar.GetThickness() << std::endl;

    ::TreeMaker::GetTreeMakerPointer()->GetDataBucket().GetDoubleMap()["rotated_thickness_mm"] = fabs(zp-zq)/mm;
    return fabs(zp-zq);
}

void
FragSimSource::PrintInfo(void) const
{
    std::cout << std::setiosflags(std::ios::left | std::ios::fixed);
    std::cout << std::setprecision(3);
    std::cout << std::setw(10) << "Radius"   << std::setw(8) << fRadius/cm << " cm" <<  std::endl;
    std::cout << std::setw(10) << "Thickn"   << std::setw(8) << fTar.GetThickness()/cm << " cm" << std::endl;
    std::cout << std::setw(10) << "RotThkns" << std::setw(8) << fRotThickness/cm << " cm" << std::endl;
    std::cout << std::setw(10) << "SurfThkns" << std::setw(8) << fSurfaceThickness/um << " um" << std::endl;
    std::cout << std::setw(10) << "Plane "
              << (fPlaneNorm.x()/fPlaneNorm.z()) << "*x + "
              << (fPlaneNorm.y()/fPlaneNorm.z()) << "*y + "
              << "z = 0"
              << std::endl;
    std::cout << std::resetiosflags(std::ios::left | std::ios::fixed);

}
