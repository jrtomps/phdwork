// VSource.hh
//
// Jeromy Tompkins
// 10/19/2011
//
// 

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#ifndef VSOURCE_HH
#define VSOURCE_HH 1

//! Interface for 3D sources used in the FragSim01 simulation
/*!
 * Describes the necessary functionality of a 3D source that is
 * called by FragSimPrimaryGeneratorAction
 */
class VSource 
{
   
public:
  //! Default constructor
  /*! Intentionally left undefined */
  VSource(void);

public:
  //! Constructor
  /*!
   * @param trans is the transformation that defines the solid's position and orientation
   */
  VSource(G4Transform3D const& trans);
  //! Copy constructor
  VSource(VSource const& obj);
  //! Assignment operator
  VSource& operator=(VSource const& obj);
  
  //! Destructor
  virtual ~VSource(void) {};
  
  // Pure virtual function
  /*! 
   * All derived objects must be to generate a random position within them 
   */
  virtual G4ThreeVector GetRandomPosition(void) = 0;
  
  //! Normal member function
  G4Transform3D   GetTransform(void) const 
  {return fTransform;};

  //! Normal member function
  void  SetTransform(const G4Transform3D& trans);
  
protected:
  G4Transform3D      fTransform;  //!< transformation defining position and orientation of source
  G4RotationMatrix    fRotation;  //!< a copy of fTransform's rotation
  G4ThreeVector    fTranslation;  //!< a copy of fTransform's translation
  G4ThreeVector      fPlaneNorm;  //!< the normal vector of the plane created by fRotation
  
};
#endif
