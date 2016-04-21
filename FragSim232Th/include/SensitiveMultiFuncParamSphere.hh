// SensitiveMultiFuncParamSphere.hh
//
// Jeromy Tompkins
// 9/20/2011
//
// A class that constructs an SSD with an associated
// SSDSensitivity object.


#ifndef SensitiveMultiFuncParamSphere_HH
#define SensitiveMultiFuncParamSphere_HH 1

#include <algorithm>
#include "G4String.hh"
#include "ParameterisedSphereBuilder.hh"
#include "G4MultiFunctionalDetector.hh"

//! A class that combines the functionality of the Sensitive Detector and the geometry
/*!
 * This class groups together the SSDBuilder functionality and its
 * sensitivity, encapsulated in SSDSensitivity.
 *
 */
class SensitiveMultiFuncParamSphere
{
private:
  ParameterisedSphereBuilder            ftheBuilder;
  G4MultiFunctionalDetector        *ftheSensitivity;
  G4int                               fcollectionID;

public:
    SensitiveMultiFuncParamSphere();


  //! Constructor
  /*!
   * Constructs the ParameterisedSphereBuilder on the stack and
   * the G4MultiFunctionalDetector on the heap. Following construction
   * the G4MultiFunctionalDetector is passed to the SSDBuilder via the
   * ParameterisedSphereBuilder::SetSensitivity() method. Assumes that the
   * collectionID is 0 !
   *
   * @param name is the name of the G4MultiFunctionalDetector object (the sensitive detector)
   */
  SensitiveMultiFuncParamSphere(const G4String name,
                                G4double iradius, G4double oradius,
                                G4double deltaTheta, G4double deltaPhi,
                                G4Material* mat);
  SensitiveMultiFuncParamSphere(const SensitiveMultiFuncParamSphere& obj);
  SensitiveMultiFuncParamSphere& operator=(SensitiveMultiFuncParamSphere obj);
  friend void swap(SensitiveMultiFuncParamSphere& lhs, SensitiveMultiFuncParamSphere& rhs);

  //! Deconstructor
  /*!
   * The G4MultiFunctionalDetector object originally constructed by this class
   * is not deleted because it is eventually deleted by the SDManager
   */
  ~SensitiveMultiFuncParamSphere(void);

  //! Normal member function
  /*!
   * @return the builder object
   */
  const ParameterisedSphereBuilder&        GetBuilder(void) const {return ftheBuilder;}
  //! Normal member function
  /*!
   * @return a pointer to the G4MultiFunctionalDetector object created by this class
   */
  G4MultiFunctionalDetector*   GetSensitivity(void) const {return ftheSensitivity;}

  //! Normal member function
  /*!
   * A convenience function that calls ParameterisedSphereBuilder::GetADetector()
   */
  G4AssemblyVolume* GetADetector(void);

  G4int GetCollectionID(void) const {return fcollectionID;}

  void SetCollectionID(G4int id) { if (id>0) fcollectionID = id; }

};

inline void swap(SensitiveMultiFuncParamSphere &lhs, SensitiveMultiFuncParamSphere &rhs)
{
    std::swap(lhs.ftheBuilder, rhs.ftheBuilder);
    std::swap(lhs.ftheSensitivity, rhs.ftheSensitivity);
    std::swap(lhs.fcollectionID, rhs.fcollectionID);
}

#endif

