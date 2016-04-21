#include <iostream>
#include <iomanip>
#include <cmath>
#include "G4VSensitiveDetector.hh"
#include "SSDSensitivity.hh"
#include "SSDHit.hh"
#include "SSDBuilder.hh"

SSDSensitivity::SSDSensitivity(G4String detLocation, G4int detID) 
    : G4VSensitiveDetector(detLocation),
      fcollectionID(detID),
      fdetName(detLocation),
      fdetID(detID),
      fcenter(SSDBuilder::strip_arc_center)
{
    G4String warnd;
    collectionName.insert(warnd=detLocation+"/SSDHitCollection");
}

SSDSensitivity::~SSDSensitivity(){;}

void 
SSDSensitivity::SetCenter(const G4double x, const G4double y, const G4double z)
{
    fcenter.set(x,y,z);
}

void
PrintVComp(G4ThreeVector v1, G4ThreeVector v2, G4ThreeVector v3)
{
    using std::setw;
    using std::setprecision;
    std::cout << std::setiosflags(std::ios::fixed)
              << std::left
              << setprecision(1);
    std::cout << setw(8) << v1.x()
              << setw(8) << v1.y()
              << setw(8) << v1.z()
              << "|";
    std::cout << setw(8) << v2.x()
              << setw(8) << v2.y()
              << setw(8) << v2.z()
              << "|";
    std::cout << setw(8) << v3.x()
              << setw(8) << v3.y()
              << setw(8) << v3.z();
    std::cout << std::resetiosflags(std::ios::fixed)
              << std::right
              << setprecision(6)
              << std::endl;

}

G4bool 
SSDSensitivity::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Processes the hits for a given step and touchable history
    // Identifies the strip based on simple geometrical considerations.
    G4double r=0.5;
    G4double theta=0;
    G4int    strip=-1;
    const G4TouchableHistory* theTouchable;

    G4StepPoint*  preStepPoint = aStep->GetPreStepPoint();//aStep->GetPostStepPoint();
    theTouchable = static_cast<const G4TouchableHistory*>(preStepPoint->GetTouchable());
    G4ThreeVector worldPos = preStepPoint->GetPosition();
    G4ThreeVector localPos = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
    G4ThreeVector offsetPos = localPos - fcenter;
    // G4ThreeVector siOffsetPos = SSDBuilder().GetSiOffset();
    // offsetPos -= siOffsetPos;
    // z components shoiuld be within the localPos range of (-4.02,4.41)
    //    std::cout << *(theTouchable->GetHistory()) << std::endl;
    r = sqrt(offsetPos.z()*offsetPos.z() + offsetPos.x()*offsetPos.x()); // units cm
    theta = atan(offsetPos.x()/offsetPos.z());
//    PrintVComp(fcenter,localPos,offsetPos);

    fhit = true;
    G4double r_first_strip = SSDBuilder::radius_first_strip_edge;
    G4double strip_width   = SSDBuilder::strip_width;
    // if ((r/cm) > 4.718)
    if (r > r_first_strip && offsetPos.z()>0)
    {
        if ((r < r_first_strip+strip_width*13) && abs(theta) < 20.0*deg)
        {
            // strip = (r/cm-4.718)/.508;
            strip = static_cast<G4int>((r-r_first_strip)/strip_width);
        }
        else if ((r < r_first_strip+strip_width*14) && abs(theta) < 17.555*deg)
        {
            strip = 13;
        }
        else if ((r < r_first_strip+strip_width*15) && abs(theta) < 14.03*deg)
        {
            strip = 14;
        }
        else if ((r < r_first_strip+strip_width*16) && abs(theta) < 9.14*deg)
        {
            strip = 15;
        }
        else
            fhit = false;
    } 
    else
        fhit = false;

    if (fhit == true)
    {
        SSDHit*  newHit = new SSDHit(fdetName,aStep,fcollectionID,strip);
        newHit->SetLocalPos(localPos);
        hitsCollection->insert( newHit );
    }

#ifdef DEBUG_Sensitivity
    if ((hitsCollection->entries()-1)%20==0 && fhit && strip<2)
    {
        std::cout << "Processing hit " << hitsCollection->entries() << std::endl;
        std::cout << "touchableVolume = "
                  << theTouchable->GetVolume()->GetName().data()
                  << std::endl;
        std::cout << "TopVolume = "
                  << theTouchable->GetHistory()->GetTopVolume()->GetName().data()
                  << std::endl;
        std::cout << "world pos (cm) \n"
                  << "x=" << std::setprecision(3) << std::setw(8) << std::left << worldPos.getX()/cm
                  << "y=" << std::setprecision(3) << std::setw(8) << std::left << worldPos.getY()/cm
                  << "z=" << std::setprecision(3) << std::setw(8) << std::left << worldPos.getZ()/cm
                  << std::endl;
        std::cout << "local pos (cm) \n"
                  << "x=" << std::setprecision(3) << std::setw(8) << std::left << localPos.getX()/cm
                  << "y=" << std::setprecision(3) << std::setw(8) << std::left << localPos.getY()/cm
                  << "z=" << std::setprecision(3) << std::setw(8) << std::left << localPos.getZ()/cm
                  << std::endl;
        std::cout << "offset pos (cm) \n"
                  << "x=" << std::setprecision(3) << std::setw(8) << std::left << offsetPos.getX()/cm
                  << "y=" << std::setprecision(3) << std::setw(8) << std::left << offsetPos.getY()/cm
                  << "z=" << std::setprecision(3) << std::setw(8) << std::left << offsetPos.getZ()/cm
                  << std::endl;
        std::cout << "offset   = " << r_first_strip/cm << " cm" << std::endl;
        std::cout << "Strip id = " << strip << std::endl;
    }
#endif


    return fhit;
}

void 
SSDSensitivity::Initialize(G4HCofThisEvent* HCE)
{
    hitsCollection = new SSDHitsCollection(fdetName,
                                           G4String() = fdetName+"collection");

    HCE->AddHitsCollection(fcollectionID,hitsCollection);
}

void 
SSDSensitivity::EndOfEvent(G4HCofThisEvent*)
{
}

