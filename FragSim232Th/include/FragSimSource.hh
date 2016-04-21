// FragSimSource.hh
//
// Jeromy Tompkins
// 10/19/2011
//
// 

#include "G4Transform3D.hh"
#include "Target.hh"
#include "VSource.hh"
#include "FragSimSourceMessenger.hh"

#ifndef FRAGSIMSOURCE_HH
#define FRAGSIMSOURCE_HH 1

class FragSimSource : public VSource
{
public:
  enum FSSType { kSurface, kSurfaceLayer, kVolume };

public:
  FragSimSource(const G4double r, 
	      const G4Transform3D& trans);
  FragSimSource(const G4double r, 
	      const G4Transform3D& trans,
	      const FSSType type);
  FragSimSource(const G4double r, 
	      const G4Transform3D& trans,
	      const FSSType type,
	      const G4double surf_thickness);
  FragSimSource(FragSimSource const& obj);
  FragSimSource& operator=(FragSimSource const& obj);
  virtual ~FragSimSource(void);

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
  G4double fRadius;
  G4double fRotThickness;
  Target   fTar;
  FSSType  fSType;     
  G4double fSurfaceThickness;
  FragSimSourceMessenger *messenger;

};

#endif 
