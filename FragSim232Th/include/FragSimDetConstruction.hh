
#include "G4Transform3D.hh" // G4Transform3D is a typedef, not a class
#include "SensitiveSSD.hh"
#include "SensitiveMultiFuncParamSphere.hh"
#include "Target.hh"
#include "G4VUserDetectorConstruction.hh"
#include "FragSimDetMessenger.hh"
#include "G4AssemblyVolume.hh"

#ifndef FragSimDetConstruction_H
#define FragSimDetConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Isotope;
class G4Element;
class G4Material;

//! The class that generates the world geometry
/*!
 * The concrete instance of the G4VUserDetectorConstruction class. This
 * class creates all of the materials that are used in the world
 * geometry. It owns 
 */
class FragSimDetConstruction : public G4VUserDetectorConstruction
{
public:
  //! Default constructor
  FragSimDetConstruction();
  //! Destructor
  ~FragSimDetConstruction();
  
  //! Normal member function
  /*!
   * Defines all of the materials that are used in the world volume
   */
  void DefineMaterials(void);
  //! Normal member function
  /*!
   * Assembles the entire world geometry and returns it.
   *
   * @returns the world volume
   */
  G4VPhysicalVolume* Construct();

  //! Normal member function
  /*!
   * @returns the transformation defining the orientation of the target in space
   */
  G4Transform3D* GetTargetTransform(void) const;
  
  void SetTargetShift(const G4ThreeVector& transl);
  void SetRot1Shift(const G4double& rot);
  void SetRot2Shift(const G4double& rot);

  void SetShiftOfWholeAssembly(const G4ThreeVector& transl);

private:
  //! Normal member function
  /*! 
   * Sets up all of the transformations used to place objects in
   * the world volume
   */
  void SetupTransforms(void);

  G4AssemblyVolume* BuildTargetAssembly();
  void  ComputeOffsets(G4ThreeVector& mount, G4ThreeVector& bot, G4ThreeVector& top);
  
  // Materials, etc.
  G4Material* vacuum;
  G4Material* aluminum;
  G4Isotope*  U235;
  G4Isotope*  U238;
  G4Element*  natU;
  G4Element* elCr;
  G4Element* elFe;
  G4Material* natUMaterial;
  G4Material* stSteel;

  // Logical volumes
  G4LogicalVolume* experimentalHall_log;
  G4LogicalVolume* chamberWall_log;
  G4LogicalVolume* target_log;
  G4LogicalVolume* target_ins_bot_lv;
  G4LogicalVolume* target_ins_top_lv;
  
  // Physical volumes
  G4VPhysicalVolume* experimentalHall_phys;
  G4VPhysicalVolume* chamberWall_phys;
  G4VPhysicalVolume* target_phys;
  G4VPhysicalVolume* target_ins_bot_pv;
  G4VPhysicalVolume* target_ins_top_pv;
  
  G4Transform3D *tr0;
  G4Transform3D *tr1;
  G4Transform3D *tr2;
  G4Transform3D *tr3;

  G4RotationMatrix *rotMatrix;
  G4Transform3D *tr_tar;

  SensitiveSSD SSD0;
  SensitiveSSD SSD1;
  SensitiveSSD SSD2;
  SensitiveSSD SSD3;

  Target tar;

  G4ThreeVector fTarShift;
  G4double      fRot1Shift;
  G4double      fRot2Shift;
  
  SensitiveMultiFuncParamSphere multiFuncSphere;

  G4AssemblyVolume* fTarget_assembly;
  G4ThreeVector fRot_center_transl;
  G4RotationMatrix fMod45_tar_rot;

  G4ThreeVector fWholeTransl;

  FragSimDetMessenger * fMessenger;
};

#endif

