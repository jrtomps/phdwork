// FragSimEventAction.hh
//
// Jeromy Tompkins
// 9/16/2011
//
// See comments below for the functionality of the class.

#include "SSDHit.hh"
#include "G4THitsMap.hh"
#include "G4THitsCollection.hh"
#include "TargetHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4UserEventAction.hh"

#ifndef FRAGSIMUSEREVENTACTION_H
#define FRAGSIMUSEREVENTACTION_H 1


//! A class to perform tasks the start and end of every event
/*!
 * The concrete instance of G4UserEventAction. This class
 * handles the filling of the TreeMaker by sorting through each
 * of the SSDHitsCollection objects stored in the G4HCofThisEvent
 * object. 
 */
class FragSimEventAction : public G4UserEventAction
{
public:
  //! Default constructor
  FragSimEventAction();
  //! Destructor
  ~FragSimEventAction();
  
  //! Normal member function
  /*!
   * An empty function 
   * @param anEvent is the current G4Event object
   */
  void BeginOfEventAction(const G4Event* anEvent);
  //! Normal member function
  /*!
   * This function gets the G4HCofThisEvent that stores
   * the SSDHitsCollection objects for each of the sensitive 
   * detectors registered to the SDManager. In this simulation, 
   * there are 4 SSDHitCollection objects in every G4HCofThisEvent
   * that correspond to each of the four detectors. If any of
   * the SSDHitsCollections are nonempty, then they are processed 
   * by ProcessEvent() to gather the information from each SSDHit
   * contained within it. Finally a call to RecordEvent() fills
   * the TreeMaker tree with the gathered information.
   * 
   * @param anEvent is the current G4Event object
   */
  void EndOfEventAction(const G4Event* anEvent);

private:
  //! Normal member function (NEEDS PROPER COMPLETION)
  /*!
   * This function is called by EndOfEventAction(). It loops
   * through each of the SSDHits stored in the SSDHitsCollection
   * that is stored in the fhitsCollection member pointer. It gathers
   * all of the information to be stored from the final SSDHit. 
   * 
   * NOTE:: This needs to be completed to properly treat events with
   *        multiple hits.
   */
  void ProcessEvent(void);

  void ProcessSSDHit(SSDHitsCollection* hc);

  void ProcessHitMap(G4THitsMap<G4double>* hc);

  void ProcessTargetHit(TargetHitsCollection* thc);

  void ResetData(void);

  bool IsPSPrestepEnergy(G4THitsMap<G4double>* hc);

  //! Normal member function
  /*!
   * This function merely uses the data gathered by ProcessEvent()
   * to fill the tree controlled by TreeMaker.
   */
  void RecordEvent(void);
  
  //! Normal member function
  /*!
   * For the purpose of storing the G4ThreeVector data into a TTree simply,
   * it is copied into an array of length 3.
   *
   * @param arr is the array to copy the data to
   * @param vec is the G4ThreeVector to copy data from
   */
  void CopyThreeVectorToArray(G4double arr[3], const G4ThreeVector& vec);

  std::pair<G4double, G4double> ComputeThetaAndPhiFromReplicaIndex(const G4int index);

  G4HCofThisEvent*            fHCoE;  //!< G4HCofThisEvent of the current event
  G4int                    fnumHits;  //!< Number of hits in the event
  G4HitsCollection* fhitsCollection; //! Pointer to a SSDHitsCollection
  const G4Event*         fThisEvent;

  G4int                 fssdDetID; //!< Detector ID ("DetID")
  G4int               fssdSiStrip; //!< Strip ID    ("SiStripID")
  G4double             fssdEffPhi; //!< Angle phi   ("Phi")
  G4double           fssdEffTheta; //!< Angle theta ("Theta")
  G4double        fssdWorldPos[3]; //!< Initial world position of particle ("InitWorldPos")
  G4double   fssdDeltaWorldPos[3]; //!< Final world position particle ("FinalWorldPos")
  G4double        fssdLocalPos[3]; //!< Final local position particle ("FinalLocalPos")
  G4double        fssdMomentum[3]; //!< Initial particle velocity ("InitMomentum")
  G4double   fssdDeltaMomentum[3]; //!< Final particle velocity ("FinalMomentum")
  G4double                 fssdKE; //!< Initial particle kinetic energy ("InitKE")
  G4double            fssdDeltaKE; //!< Final particle kinetic energy ("FinalKE")
  G4double            fssdTotDepE; //!< Deposited kinetic energy
  G4double    fssdNonIonizingDepE;
  G4String            fssdDetName; //!< A prestring for use in CSV output ("DetName")
  G4String       fssdParticleName; //!< Particle name ("ParticleName")
  G4double               fssdMass; //!< Particle mass ("Mass")
  G4double             fssdCharge; //!< Particle charge ("Charge")

  G4double           fprestepE;
  G4double           fsurfCurr;

  G4double           ftarPhi;
  G4double           ftarTheta;
  G4double           ftarPos[3];
  G4double           ftarDeltaPos[3];
  G4double           ftarMomentum[3];
  G4double           ftarDeltaMomentum[3];
  G4double           ftarKE;
  G4double           ftarDeltaKE;
  G4double           ftarTotDepE;
  G4double           ftarNonIonizingDepE;
  G4String           ftarParticleName;
  G4double           ftarMass;
  G4double           ftarCharge;
  G4double           ftarDeltaCharge;
  G4double           ftarTrackLength;
  G4double           ftarAveTrackLength;
  G4int              ftarNSteps;
  G4double           ftarLocalPos[3];

  bool     fDataExists;
};

#endif
