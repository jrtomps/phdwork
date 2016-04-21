// PrimarySupervisor.hh
//
// Jeromy Tompkins
// 10/21/2011
//

#include "globals.hh"
#include "G4String.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4AffineTransform.hh"
#include "PrimarySupervisor.hh"

PrimarySupervisor::PrimarySupervisor(const G4String& worldname, 
			       const G4String& pvname)
  : fWorldName(worldname),
    fPVName(pvname),
    fSolid(NULL),
    fTrans(NULL)
{
  // The G4Navigator is not existant here so the setting of 
  // the fSolid pointer was deferred to when it is called.
  //  InitializeSolidPtr();
}

PrimarySupervisor::PrimarySupervisor(const PrimarySupervisor & that)
  : fWorldName(that.fWorldName),
    fPVName(that.fPVName),
    fSolid(that.fSolid),
    fTrans(that.fTrans)
{
}

PrimarySupervisor&
PrimarySupervisor::operator=(const PrimarySupervisor & that)
{
  if (this != &that)
    {
      fWorldName = that.fWorldName;
      fPVName    = that.fPVName;
      fSolid     = that.fSolid;
      fTrans     = that.fTrans;
    }
  return *this;
}

PrimarySupervisor::~PrimarySupervisor(void)
{
  if (fTrans!=NULL) { delete fTrans; fTrans=0; };
}

G4bool 
PrimarySupervisor::IsGoodVertex(const G4ThreeVector& vec) 
{
  if (fSolid==NULL) InitializeSolidPtr();

  G4ThreeVector tvec = fTrans->Inverse().TransformPoint(vec);
  G4bool f = (fSolid->Inside(tvec) != kOutside);
#ifdef DEBUG_PrimarySupervisor
  if (!f) std::cout << "\routside";
#endif
  return f;
}

void 
PrimarySupervisor::InitializeSolidPtr(void)
{
  G4TransportationManager* tm = G4TransportationManager::GetTransportationManager();
  G4Navigator*            nav = tm->GetNavigator(fWorldName);
  G4VPhysicalVolume*      wpv;
  if (nav!=NULL) wpv = nav->GetWorldVolume();
  else throw MyException("PrimarySupervisor:: G4Navigator is NULL");
    
  G4LogicalVolume*        wlv = wpv->GetLogicalVolume();
  G4VPhysicalVolume*      dpv;
  
  G4int i=0; 
  while (i < wlv->GetNoDaughters())
    {
      dpv = wlv->GetDaughter(i);
      if (dpv->GetName() == fPVName)
        {
	fTrans = new G4AffineTransform(dpv->GetRotation(), dpv->GetTranslation());
	fSolid = dpv->GetLogicalVolume()->GetSolid();
	break;
        }
      i++;
      if (i == wlv->GetNoDaughters())
        {
	throw MyException("PrimarySupervisor unable to find specified solid");
        }
    }
}
