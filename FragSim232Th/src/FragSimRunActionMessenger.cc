#include <sstream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "FragSimRunAction.hh"
#include "FragSimRunActionMessenger.hh"
#include "G4String.hh"

FragSimRunActionMessenger::FragSimRunActionMessenger(FragSimRunAction* source)
    : fRunAction(source)
{
    srcDir = new G4UIdirectory("/FragSim02/run/");
    srcDir->SetGuidance("Commands for controlling the FragSimRunAction");

    runNameCmd = new G4UIcmdWithAString("/FragSim02/run/name",this);
    std::ostringstream guidance;
    guidance << "Set run name.";
    runNameCmd->SetGuidance("Set run name.");
    runNameCmd->SetParameterName("name",true);
    runNameCmd->SetDefaultValue("run");
    runNameCmd->AvailableForStates(G4State_PreInit);
}

FragSimRunActionMessenger::FragSimRunActionMessenger(const FragSimRunActionMessenger &obj)
    : fRunAction(obj.fRunAction), // this doesn't own the RunAction
    srcDir(new G4UIdirectory(*obj.srcDir)),
    runNameCmd(new G4UIcmdWithAString(*obj.runNameCmd))
{
}

FragSimRunActionMessenger::~FragSimRunActionMessenger()
{
    if (srcDir) { delete srcDir; srcDir=0;}
    if (runNameCmd) { delete runNameCmd; runNameCmd=0;}
}

void FragSimRunActionMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == runNameCmd)
    {
        fRunAction->SetRunName(newValue);
    }
}

