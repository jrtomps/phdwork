// FragSimEventAction.cc
//
// Jeromy Tompkins
// 9/16/2011
//
// 

#include "TreeMaker.hh"
#include "G4Event.hh"
#include "SSDHit.hh"
#include "G4String.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "G4HCofThisEvent.hh"
#include "G4UserEventAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "TString.h"
#include "TROOT.h"
#include "G4THitsMap.hh"
#include "globals.hh"
#include "FragSimEventAction.hh"

FragSimEventAction::FragSimEventAction(void)
    : G4UserEventAction(),
    fHCoE(NULL),
    fnumHits(0),
    fhitsCollection(NULL),
    fssdDetID(0),
    fssdDetName("")
{
}

FragSimEventAction::~FragSimEventAction()
{
    G4cout << "Instance of FragSimEventAction Destructed!" << G4endl;
}

void 
        FragSimEventAction::BeginOfEventAction(const G4Event* /*anEvent*/)
{
#ifdef DEBUG_EventAction
    std::cout << "Begin event" << std::endl;
#endif
}


void FragSimEventAction::ProcessEvent(void)
{  
    // Loop over all the collections for each of the sensitive detectors
    for(G4int i=0; i<fHCoE->GetNumberOfCollections(); i++)
    {
 //       G4cout << i << ":";
        SSDHitsCollection *ssdhc = dynamic_cast<SSDHitsCollection*>(fHCoE->GetHC(i));
        if( ssdhc )
        {
            ProcessSSDHit(ssdhc);
        }

        G4THitsMap<G4double>* hmap = dynamic_cast<G4THitsMap<G4double>* >(fHCoE->GetHC(i));
        if ( hmap )
        {
            ProcessHitMap(hmap);
        }

        TargetHitsCollection* thc = dynamic_cast<TargetHitsCollection*>(fHCoE->GetHC(i));
        if (thc)
        {
            ProcessTargetHit(thc);
        }

//        G4cout << G4endl;
    }//END of for loop

}


void FragSimEventAction::ProcessSSDHit(SSDHitsCollection *ssdhc)
{
    G4int numHits = ssdhc->entries();
    SSDHit thisHit;

    if (ssdhc->GetSize()==0) return;

//    G4cout << ssdhc->GetSDname() << "(hits=" << ssdhc->GetSize() << ")/";

    // Loop through the hits collection.
    // And sum up the hits to get the total
    // hit... this effectively accounts for
    // summing the deposited energy correctly.
    for(G4int i=0; i<numHits; i++)
    {
        if (i==0)
            thisHit = *((*ssdhc)[i]);
        else
            thisHit += *((*ssdhc)[i]);
    }

    fssdDetID    = thisHit.GetDetID();
    fssdSiStrip  = thisHit.GetStripID();
    fssdEffPhi      = thisHit.GetPhi();
    fssdEffTheta    = thisHit.GetTheta();
    CopyThreeVectorToArray(fssdWorldPos,  thisHit.GetWorldPos());
    CopyThreeVectorToArray(fssdDeltaWorldPos, thisHit.GetDeltaPosition());
    CopyThreeVectorToArray(fssdLocalPos, thisHit.GetFinalLocalPos());
    CopyThreeVectorToArray(fssdMomentum,  thisHit.GetMomentum());
    CopyThreeVectorToArray(fssdDeltaMomentum, thisHit.GetDeltaMomentum());
    fssdKE   = thisHit.GetKineticEnergy();
    fssdDeltaKE  = thisHit.GetDeltaKineticEnergy();
    fssdTotDepE = thisHit.GetTotalEnergyDeposit();
    fssdNonIonizingDepE = thisHit.GetNonIonizingEnergyDeposit();
    fssdParticleName = thisHit.GetParticleName();
    fssdMass     = thisHit.GetMass();
    fssdCharge   = thisHit.GetCharge();

    if (fssdDeltaWorldPos[0]!=0 || fssdDeltaWorldPos[1]!=0 || fssdDeltaWorldPos[2]!=0) fDataExists=true;
}

void FragSimEventAction::ProcessHitMap(G4THitsMap<G4double> *hc)
{
    if (hc->GetMap()->size()==0) return;//{std::cout << std::endl; return;}

    std::cout << " nonempty" << std::endl;
    std::pair<G4double, G4double> theta_phi;

    std::map<G4int, G4double*>::iterator it = hc->GetMap()->begin();

//    theta_phi = ComputeThetaAndPhiFromReplicaIndex(it->first);
//    ftheta = theta_phi.first;
//    fphi   = theta_phi.second;

    if (IsPSPrestepEnergy(hc))
        fprestepE = *it->second;
    else
        fsurfCurr = *it->second;

    // Since I am storing eventwise info, it is necessary to
    // reset the map everytime
    //hc->clear();

    if (fprestepE!=0 || fsurfCurr!=0) fDataExists = true;
}

void FragSimEventAction::ProcessTargetHit(TargetHitsCollection *thc)
{
    if (thc->GetSize()==0) return;

//    G4cout << thc->GetSDname() << "(hits=" << thc->GetSize() << ")/";

    G4int numHits = thc->entries();
    TargetHit thisHit;

    // Loop through the hits collection.
    // And sum up the hits to get the total
    // hit... this effectively accounts for
    // summing the deposited energy correctly.
    for(G4int i=0; i<numHits; i++)
    {
        if (i==0)
            thisHit = *((*thc)[i]);
        else
            thisHit += *((*thc)[i]);
    }

    ftarNSteps = numHits;

    ftarPhi = thisHit.GetPhi();
    ftarTheta = thisHit.GetTheta();
    CopyThreeVectorToArray(ftarPos,  thisHit.GetPosition());
    CopyThreeVectorToArray(ftarDeltaPos, thisHit.GetDeltaPosition());
    CopyThreeVectorToArray(ftarMomentum,  thisHit.GetMomentum());
    CopyThreeVectorToArray(ftarDeltaMomentum, thisHit.GetDeltaMomentum());
    ftarKE = thisHit.GetKineticEnergy()/MeV;
    ftarDeltaKE  = thisHit.GetDeltaKineticEnergy()/MeV;
    ftarTotDepE = thisHit.GetTotalEnergyDeposit()/MeV;
    ftarNonIonizingDepE = thisHit.GetNonIonizingEnergyDeposit()/MeV;
    ftarParticleName = thisHit.GetParticleName();
    ftarMass    = thisHit.GetMass();
    ftarCharge   = thisHit.GetCharge();
    ftarDeltaCharge = thisHit.GetDeltaCharge();
    ftarTrackLength = thisHit.GetTrackLength();
    ftarAveTrackLength = ftarTrackLength/ftarNSteps;
    CopyThreeVectorToArray(ftarLocalPos, thisHit.GetLocalPosition());

    if (ftarDeltaPos[0]!=0 || ftarDeltaPos[1]!=0 || ftarDeltaPos[2]!=0) fDataExists = true;
}

std::pair<G4double, G4double> FragSimEventAction::ComputeThetaAndPhiFromReplicaIndex(G4int index)
{
    G4int nPhi = 36; // assuming deltaPhi = 10*deg
    G4double phi = (index%nPhi)*deg + 5.0*deg;
    G4double theta = (index/nPhi)*deg + 5.0*deg;
    return std::make_pair(theta,phi);
}

bool FragSimEventAction::IsPSPrestepEnergy(G4THitsMap<G4double> *hc)
{
    return (hc->GetName().rfind("psenergy")!=G4String::npos);
}

void FragSimEventAction::ResetData()
{
    fDataExists = false;

    fssdDetID = -1;
    fssdSiStrip = -1;
    fssdEffPhi = 0;
    fssdEffTheta = 0;
    fssdWorldPos[0] = 0;    fssdWorldPos[1] = 0;    fssdWorldPos[2] = 0;
    fssdDeltaWorldPos[0] = 0;    fssdDeltaWorldPos[1] = 0;    fssdDeltaWorldPos[2] = 0;
    fssdLocalPos[0] = 0;    fssdLocalPos[1] = 0;    fssdLocalPos[2] = 0;
    fssdMomentum[0] = 0;    fssdMomentum[1] = 0;    fssdMomentum[2] = 0;
    fssdDeltaMomentum[0] = 0;    fssdDeltaMomentum[1] = 0;    fssdDeltaMomentum[2] = 0;
    fssdKE = 0;
    fssdDeltaKE = 0;
    fssdTotDepE = 0;
    fssdNonIonizingDepE = 0;
    fssdDetName = "";
    fssdParticleName = "";
    fssdMass = 0;
    fssdCharge = 0;

    fprestepE = 0;
    fsurfCurr = 0;

    ftarPhi = 0;
    ftarTheta = 0;
    ftarPos[0] = 0;           ftarPos[1]=0;           ftarPos[2] = 0;
    ftarDeltaPos[0] = 0;      ftarDeltaPos[1]=0;      ftarDeltaPos[2] = 0;
    ftarMomentum[0] = 0;      ftarMomentum[1]=0;      ftarMomentum[2] = 0;
    ftarDeltaMomentum[0] = 0; ftarDeltaMomentum[1]=0; ftarDeltaMomentum[2] = 0;
    ftarKE = 0;
    ftarDeltaKE = 0;
    ftarTotDepE = 0;
    ftarNonIonizingDepE = 0;
    ftarMass = 0;
    ftarCharge = 0;
    ftarDeltaCharge = 0;
    ftarTrackLength = 0;
    ftarNSteps = 0;
    ftarAveTrackLength = 0;
    ftarLocalPos[0] = 0; ftarLocalPos[1] = 0; ftarLocalPos[2] = 0;

}

void FragSimEventAction::RecordEvent(void)
{
    ::TreeMaker *treeMakerControl = ::TreeMaker::GetTreeMakerPointer();

    if (! fDataExists) return;

    treeMakerControl->FillBranch("DetID", "I", &fssdDetID);
    treeMakerControl->FillBranch("SiStripID", "I", &fssdSiStrip);
    treeMakerControl->FillBranch("EffPhi", "D", &fssdEffPhi);
    treeMakerControl->FillBranch("EffTheta","D",&fssdEffTheta);
    treeMakerControl->FillBranch("SSDWorldPos","D",&fssdWorldPos[0],3);
//    treeMakerControl->FillBranch("SSDDeltaWorldPos","D",&fssdDeltaWorldPos[0],3);
    treeMakerControl->FillBranch("SSDLocalPos","D",&fssdLocalPos[0],3);
//    treeMakerControl->FillBranch("SSDMomentum","D",&fssdMomentum[0],3);
//    treeMakerControl->FillBranch("SSDDeltaMomentum","D",&fssdDeltaMomentum[0],3);
    treeMakerControl->FillBranch("SSDKE","D",&fssdKE);
//    treeMakerControl->FillBranch("SSDDeltaKE","D",&fssdDeltaKE);
    treeMakerControl->FillBranch("SSDTotDepE","D",&fssdTotDepE);
//    treeMakerControl->FillBranch("SSDNonIonizingDepE","D",&fssdNonIonizingDepE);
    TString str(fssdParticleName.data());
    treeMakerControl->FillBranch("SSDParticleName","C",&str);
//    treeMakerControl->FillBranch("SSDMass","D",&fssdMass);
//    treeMakerControl->FillBranch("SSDCharge","D",&fssdCharge);

//    treeMakerControl->FillBranch("PreStepE","D", &fprestepE);
//    treeMakerControl->FillBranch("SurfCurrent","D", &fsurfCurr);

    treeMakerControl->FillBranch("TarPhi", "D", &ftarPhi);
    treeMakerControl->FillBranch("TarTheta","D",&ftarTheta);
    treeMakerControl->FillBranch("TarWorldPos","D",ftarPos,3);
    treeMakerControl->FillBranch("TarDeltaWorldPos","D",ftarDeltaPos,3);
    treeMakerControl->FillBranch("TarMomentum","D",ftarMomentum,3);
    treeMakerControl->FillBranch("TarDeltaMomentum","D",ftarDeltaMomentum,3);
    treeMakerControl->FillBranch("TarKE","D",&ftarKE);
    treeMakerControl->FillBranch("TarDeltaKE","D",&ftarDeltaKE);
    treeMakerControl->FillBranch("TarTotDepE","D",&ftarTotDepE);
    treeMakerControl->FillBranch("TarNonIonizingDepE","D",&ftarNonIonizingDepE);
    TString tarstr(ftarParticleName.data());
    treeMakerControl->FillBranch("TarParticleName","C",&tarstr);
//    treeMakerControl->FillBranch("TarMass","D",&ftarMass);
    treeMakerControl->FillBranch("TarCharge","D",&ftarCharge);
    treeMakerControl->FillBranch("TarDeltaCharge","D",&ftarDeltaCharge);
    treeMakerControl->FillBranch("TarTrackLength","D",&ftarTrackLength);
    treeMakerControl->FillBranch("TarNSteps","I",&ftarNSteps);
    treeMakerControl->FillBranch("TarAveTrackLength","D",&ftarAveTrackLength);
    treeMakerControl->FillBranch("TarLocalPos","D",ftarLocalPos,3);

    treeMakerControl->FillTree();
}//END of RecordEvent()

void FragSimEventAction::EndOfEventAction(const G4Event* thisEvent)
{
    // Store the address of this event for use in ProcessEvent
    fThisEvent = thisEvent;

    ResetData();

    // The HCofThisEvent has an entry for the SSDHitsCollection*
    // associated with each sensitive detector registered to
    // the SDManager
    fHCoE = thisEvent->GetHCofThisEvent();

    if( fHCoE )
    {
        ProcessEvent();
        RecordEvent();
    }

    std::ios::fmtflags rmask = (std::ios::fixed | std::ios::right);
    std::ios::fmtflags lmask = (std::ios::fixed | std::ios::left);

    const G4Run* run = G4RunManager::GetRunManager()->GetCurrentRun();

    G4int n = run->GetNumberOfEventToBeProcessed()/20;
    if (n==0) return;
    if ((run->GetNumberOfEvent()%n)==0)
    {
        std::cout << std::setiosflags(rmask);
        std::cout << "\r" << std::setw(2) << std::setprecision(0)
                << 100.0*run->GetNumberOfEvent()/run->GetNumberOfEventToBeProcessed()
                << std::resetiosflags(rmask);
        std::cout << std::left << "%";
        std::cout << std::resetiosflags(lmask);
        std::cout.flush();
    }
}

void FragSimEventAction::CopyThreeVectorToArray(G4double* arr,
                                                   const G4ThreeVector& vec)
{
    arr[0]= vec.getX();
    arr[1]= vec.getY();
    arr[2]= vec.getZ();
}
