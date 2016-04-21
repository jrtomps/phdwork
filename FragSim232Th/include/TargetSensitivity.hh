#include "G4VSensitiveDetector.hh"
#include "TargetHit.hh"
#include "G4ThreeVector.hh"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifndef TargetSENSITIVITY_H
#define TargetSENSITIVITY_H 1

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


//! Concrete implementation of G4VSensitiveDetector class for Target
/*!
 * A class that concretely implements the G4VSensitiveDetector class.
 * In essence, this provides the sensitivity of the target logical volume.
 * The name passed into the constructor names the detector and will
 * also be used to name the TargetHitsCollection object that is
 * created by this object.
 *
 * This is about as simple of a concrete implementation that can be
 * written b/c it performs no special calculations. Instead it just
 * creates a TargetHit and adds it to the hit collection for the event.
 *
 */
class TargetSensitivity : public G4VSensitiveDetector
{
public: 
  //! Constructor
  /*!
   * Initializes all of the data members. 
   * @param name is the name of the detector 
   * @param detID is a numerical ID for the detector
   */
  TargetSensitivity(G4String name, G4int collectionID);
  //! Deconstructor
  ~TargetSensitivity();

  //! Virtual member function
  /*! 
   * The TargetHitsCollection to be associated with this for the entire
   * event is generated and inserted into the parameter's collection
   * of collections.
   * 
   * @param HCE is a pointer to a G4HCofThisEvent ("G4 Hit Collection of This Event")
   */
  virtual void   Initialize(G4HCofThisEvent* HCE);

protected:
  //! Virtual member function
  /*!
   * Computation of the strip number involved is done in this function.
   * Further the new TargetHit object is created and stored inserted into the
   * class's TargetHitCollection.
   *
   * @param aStep is the the current G4Step 
   * @param ROHist is the G4TouchableHistory of the Readout Geometry (must be specified to be used)
   */
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROHist);
  
public:
  //! Virtual member function
  /*!
   * An empty function that doesn't do anything.
   *
   * @param HCE is a pointer to a G4HCofThisEvent
   */
  virtual void   EndOfEvent(G4HCofThisEvent* HCE);

private:
  TargetHitsCollection *hitsCollection;  //!< a pointer to the hit collection passed by the event

  G4int            fcollectionID; //!< the id number for hit collections produced by objects of this class
  const G4String        fdetName;
};
#endif
