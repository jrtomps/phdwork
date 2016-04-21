// SensitiveSSD.hh
//
// Jeromy Tompkins
// 9/20/2011
//
// A class that constructs an SSD with an associated 
// SSDSensitivity object.

#include "SSDBuilder.hh"
#include "SSDSensitivity.hh"

#ifndef SENSITIVESSD_HH
#define SENSITIVESSD_HH 1

//! A class that combines the functionality of the Sensitive Detector and the geometry
/*!
 * This class groups together the SSDBuilder functionality and its
 * sensitivity, encapsulated in SSDSensitivity. 
 * 
 */
class SensitiveSSD
{

public:
  //! Constructor
  /*!
   * Constructs the SSDBuilder on the stack and 
   * the SSDSensitivity on the heap. Following construction
   * the SSDSensitivity is passed to the SSDBuilder via the
   * SSDBuilder::SetSensitivity() method.
   * 
   * @param name is the name of the SSDSensitivity object (the sensitive detector)
   * @param detID is the id number to be associated with the SSDSensitivity object
   */
  SensitiveSSD(const G4String name, const G4int detID);

  //! Deconstructor
  /*!
   * The SSDSensitivity object originally constructed by this class
   * is not deleted because it is eventually deleted by the SDManager
   */
  ~SensitiveSSD(void);

  //! Normal member function
  /*!
   * @return the builder object
   */
  SSDBuilder        GetBuilder(void)     const {return ftheBuilder;};
  //! Normal member function
  /*!
   * @return a point to the SSDSensitivity object created by this class
   */
  SSDSensitivity*   GetSensitivity(void) const {return ftheSensitivity;};

  //! Normal member function
  /*!
   * A convenience function that calls SSDBuilder::GetADetector()
   */
  G4AssemblyVolume* GetADetector(void);
  
private:
  SSDBuilder            ftheBuilder; 
  SSDSensitivity   *ftheSensitivity;

};

#endif
 
