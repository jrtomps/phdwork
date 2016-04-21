#include "G4VSensitiveDetector.hh"
#include "SSDHit.hh"
#include "G4ThreeVector.hh"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifndef SSDSENSITIVITY_H
#define SSDSENSITIVITY_H 1

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


//! Concrete implementation of G4VSensitiveDetector class for SSD
/*!
 * A class that concretely implements the G4SensitiveDetector class.
 * In essence, this provides the sensitivity of the logical volume 
 * that defines the silicon detector itself (@see SSDBuilder). The 
 * name passed into the constructor names the detector and will
 * also be used to name the SSDHitsCollection object that is 
 * created by this object. In this way, the various detectors can
 * be differentiated from each other. In addition, an ID number is
 * assigned to the detector to be used for the same purpose.
 *
 * An important task that is completed by this class is the determination
 * of the Si strip that was hit, which is done by geometrical 
 * considerations after coordinated transformation to the local
 * reference frame of the Si logical volume.
 *
 * This class also acts to filter out hits not located on the Si.
 * If it occurred outside of the Si volume then it is not stored as
 * an SSDHit. 
 */
class SSDSensitivity : public G4VSensitiveDetector
{
public: 
  //! Constructor
  /*!
   * Initializes all of the data members. 
   * @param name is the name of the detector 
   * @param detID is a numerical ID for the detector
   */
  SSDSensitivity(G4String name, G4int detID);
  //! Deconstructor
  ~SSDSensitivity();

  //! Normal member function
  /*!
   * Sets the center of the circle that defines the arcs separating
   * the Si strips. This shouldn't need to be called because
   * the default center coordinates are set by the constructor. 
   *
   * @param x is the x position
   * @param y is the y position
   * @param z is the z position
   */
  void SetCenter(const G4double x, const G4double y, const G4double z);

  //! Virtual member function
  /*! 
   * The SSDHitsCollection to be associated with this for the entire
   * simulation is generated and inserted into the parameter's collection
   * of collections.
   * 
   * @param HCE is a pointer to a G4HCofThisEvent ("G4 Hit Collection of This Event")
   */
  virtual void   Initialize(G4HCofThisEvent* HCE);

  //! Virtual member function
  /*!
   * Computation of the strip number involved is done in this function.
   * Further the new SSDHit object is created and stored inserted into the
   * class's SSDHitCollection.
   *
   * @param aStep is the the current G4Step 
   * @param ROHist is the G4TouchableHistory of the Readout Geometry (must be specified to be used)
   */
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROHist);
  
  //! Virtual member function
  /*!
   * An empty function that doesn't do anything.
   *
   * @param HCE is a pointer to a G4HCofThisEvent
   */
  virtual void   EndOfEvent(G4HCofThisEvent* HCE);

  //! Normal member function
  /*! 
   * @return the detector name
   */
  G4String GetDetName(void) const {return fdetName;};

private:
  SSDHitsCollection *hitsCollection;  //!< a pointer to the hit collection passed by the event

  G4int      fcollectionID; //!< the id number for hit collections produced by objects of this class
  const G4String        fdetName;
  G4int             fdetID; //!< the id number of the detector associated with this
  G4ThreeVector    fcenter; //!< the coordinate of the arc center describing strip boundaries 
  G4bool              fhit; //!< a flag to filter out false hits (ie. no strip was hit --> no SSDHit object produced)
};
#endif
