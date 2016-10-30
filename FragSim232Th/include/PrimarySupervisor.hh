// PrimarySupervisor.hh
//
// Jeromy Tompkins
// 10/21/2011
//

#include <exception>
#include "G4String.hh"

#ifndef PRIMARYSUPERVISOR_HH
#define PRIMARYSUPERVISOR_HH 1

class MyException : public std::exception
{
public:
  MyException(const G4String& huh) throw() : std::exception(), fWhat(huh) {};
  ~MyException(void) throw()  {};
  const char* what() const throw()  { return fWhat.data(); };
private:
  G4String fWhat;
};

class G4RunManager; 
class G4VSolid;
class G4AffineTransform;

//! Ensures primary vertex is generated with a geometric volume
class PrimarySupervisor
{
private:
  /*!
   * Deliberately left undefined because the class doesn't need a
   * default constructor
   */
  PrimarySupervisor(void);

public:
  //! Constructor
  /*!
   * @param worldname is the name of world volume
   * @param pvname is the name of the physical volume that vertices must be inside
   */
  PrimarySupervisor(const G4String& worldname, const G4String& pvname);
  //! Copy constructor
  PrimarySupervisor(const PrimarySupervisor & that);
  //! Assignment operator
  PrimarySupervisor& operator=(const PrimarySupervisor& that);
  ~PrimarySupervisor(void);

  //! A normal member function
  /*!
   * Checks to see if the vertex is inside or on the surface.
   * @param vec is a point in space
   * @returns true if point is inside or on the surface and false if not. 
   */
  G4bool IsGoodVertex(const G4ThreeVector& vec);

private:
  //! A normal member function
  /*!
   * Searches the world volume for the fPVName and stores its transformation.
   */
  void InitializeSolidPtr(void);

  G4String             fWorldName; //!< name of the world volume
  G4String                fPVName; //!< name of the physical volume defining spatial constraint
  G4VSolid*                fSolid; //!< solid defining fPVName
  G4AffineTransform*       fTrans; //!< transform that defines fSolid's orientation in space

};

#endif


