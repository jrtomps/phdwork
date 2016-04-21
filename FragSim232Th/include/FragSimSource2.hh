// FragSimSource2.hh
//
// Jeromy Tompkins
// 10/19/2011
//
// 

#include "G4Transform3D.hh"
#include "Target.hh"
#include "VSource.hh"
//#include "FragSimSource2Messenger.hh"

#ifndef FragSimSource2_HH
#define FragSimSource2_HH 1

class FragSimSource2 : public VSource
{
public:
  enum FSSType { kSurface, kSurfaceLayer, kVolume };

public:
  FragSimSource2(const G4double r,
              const G4String& obj_name,
	      const FSSType type,
	      const G4double surf_thickness);
  FragSimSource2(FragSimSource2 const& obj);
  FragSimSource2& operator=(FragSimSource2 const& obj);
  virtual ~FragSimSource2(void);

  G4ThreeVector GetRandomPosition(void);

  G4double GetRadius(void) const;
  void     SetRadius(const G4double r);

  FSSType GetType(void) const  { return fSType; }
  void SetType( FSSType type );
  void SetType( G4String type_name );

  G4double GetSurfaceThickness (void) const {  return fSurfaceThickness; }
  void SetSurfaceThickness (const G4double thick);

  void PrintInfo(void) const;

private:
  G4double ComputeRotThickness(void);
  G4double ComputeZ(const G4double x, const G4double y) const;
  void Initialize();
  G4Transform3D FindTransform(const G4String& obj_name);
  G4double fRadius;
  G4double fRotThickness;
  Target   fTar;
  FSSType  fSType;     
  G4double fSurfaceThickness;
  G4Transform3D* fTransformPtr;
  G4String  fObjName;
//  FragSimSource2Messenger *messenger;

};

#endif 
