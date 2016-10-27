#include "G4Run.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "FragSimPrimaryGeneratorAction.hh"
#include "PrimarySupervisor.hh"
#include "TreeMaker.hh"
#include "G4String.hh"
#include "G4UserRunAction.hh"
#include "FragSimRunAction.hh"
#include "FragSimRunActionMessenger.hh"
#include "TString.h"

FragSimRunAction::FragSimRunAction()
  : G4UserRunAction(),
    ftreeMaker(NULL),
    frun_name("run"),
    messenger(new FragSimRunActionMessenger(this))
{
  G4cout << "Instance of FragSimRunAction Constructed!" << G4endl;
  ftreeMaker = ::TreeMaker::GetTreeMakerPointer();
}//END of constructor

FragSimRunAction::FragSimRunAction(const G4String run_name)
  : G4UserRunAction(),
    ftreeMaker(NULL),
    frun_name(run_name),
    messenger(new FragSimRunActionMessenger(this))
{
  G4cout << "Instance of FragSimRunAction Constructed!" << G4endl;
}//END of constructor

FragSimRunAction::~FragSimRunAction()
{
  G4cout << "Instance of FragSimRunAction Destructed!" << G4endl;
  ftreeMaker = NULL;
  delete messenger;
}//END of destructor

void 
FragSimRunAction::BeginOfRunAction(const G4Run *aRun)
{
  aRun = 0;
  ftreeMaker = ::TreeMaker::GetTreeMakerPointer();

  TString path = "./sim_output";
  const char* outdir = getenv("SIM_OUTPUT_DIR");
  if (outdir) {
    path = outdir;
  }
  TString name = frun_name.data();
  TString treeName = "Sim_Tree";
  Int_t compression = 1;
  
  ftreeMaker->CreateNewFileAndTree( path, name, compression, treeName );
  ftreeMaker->SetNumberOfEvents(GetNumberOfEventsInRun());
  ftreeMaker->SetDisplayInterval(5/*seconds*/);

}//END of BeginOfRunAction()

void 
FragSimRunAction::EndOfRunAction(const G4Run *aRun)
{

    G4SDManager::GetSDMpointer()->ListTree();

  aRun = 0;

  ftreeMaker->AutoSaveTree();
  ftreeMaker->CloseCurrentFile();
  ftreeMaker->~TreeMaker();

}//END of EndOfRunAction()

G4int
FragSimRunAction::GetNumberOfEventsInRun(void)
{
  G4RunManager* rm = G4RunManager::GetRunManager();
  return rm->GetCurrentRun()->GetNumberOfEventToBeProcessed();
}

void FragSimRunAction::SetRunName(G4String run_name)
{
    frun_name = run_name;
}
