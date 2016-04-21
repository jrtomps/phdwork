#include <sstream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "FragSimDetConstruction.hh"
#include "FragSimDetMessenger.hh"
#include "G4String.hh"

FragSimDetMessenger::FragSimDetMessenger(FragSimDetConstruction* source)
    : fPrimaryGenerator(source)
{
    srcDir = new G4UIdirectory("/FragSim02/exp_geometry/");
    srcDir->SetGuidance("Commands for controlling FragSimDetConstruction");

    fTransCmd = new G4UIcmdWith3VectorAndUnit("/FragSim02/exp_geometry/target_translation",this);
    std::ostringstream guidance;
    guidance << "Enter offset of target position from its normal location";
    fTransCmd->SetGuidance(guidance.str().data());
    G4ThreeVector vec(0,0,0);
    fTransCmd->SetDefaultValue(vec);
    fTransCmd->SetDefaultUnit("mm");


    fRot1Cmd = new G4UIcmdWithADoubleAndUnit("/FragSim02/exp_geometry/rot1", this);
    guidance.str(""); guidance.clear();
    guidance << "Rotational offset rotations of target about y-axis";
    fRot1Cmd->SetGuidance(guidance.str().data());
    fRot1Cmd->SetDefaultValue(0);
    fRot1Cmd->SetDefaultUnit("deg");

    fRot2Cmd = new G4UIcmdWithADoubleAndUnit("/FragSim02/exp_geometry/rot2", this);
    guidance.str(""); guidance.clear();
    guidance << "Angular offset defining pitch of target";
    guidance << "\n(Increasingly negative value stand the target upright)";
    fRot2Cmd->SetGuidance(guidance.str().data());
    fRot2Cmd->SetDefaultValue(0);
    fRot2Cmd->SetDefaultUnit("deg");

    fWholeTranslCmd = new G4UIcmdWith3VectorAndUnit("/FragSim02/exp_geometry/global_translation",this);
    guidance.str(""); guidance.clear();
    guidance << "Enter offset of entire assembly from beam axis";
    fWholeTranslCmd->SetGuidance(guidance.str().data());
    fWholeTranslCmd->SetDefaultValue(G4ThreeVector(0,0,0));
    fWholeTranslCmd->SetDefaultUnit("mm");

}

FragSimDetMessenger::FragSimDetMessenger(const FragSimDetMessenger &obj)
    : fPrimaryGenerator(obj.fPrimaryGenerator), // this doesn't own the PrimaryGenerator
    srcDir(new G4UIdirectory(*obj.srcDir)),
    fTransCmd(new G4UIcmdWith3VectorAndUnit(*obj.fTransCmd)),
    fRot1Cmd(new G4UIcmdWithADoubleAndUnit(*obj.fRot1Cmd)),
    fRot2Cmd(new G4UIcmdWithADoubleAndUnit(*obj.fRot2Cmd)),
    fWholeTranslCmd(new G4UIcmdWith3VectorAndUnit(*obj.fWholeTranslCmd))
{
}

FragSimDetMessenger::~FragSimDetMessenger()
{
    if (srcDir) { delete srcDir; srcDir=0;}
    if (fTransCmd) { delete fTransCmd; fTransCmd=0;}
    if (fRot1Cmd) { delete fRot1Cmd; fRot1Cmd=0;}
    if (fRot2Cmd) { delete fRot2Cmd; fRot2Cmd=0;}
    if (fWholeTranslCmd) { delete fWholeTranslCmd; fWholeTranslCmd=0;}
}

void FragSimDetMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fTransCmd)
    {
        fPrimaryGenerator->SetTargetShift(fTransCmd->GetNew3VectorValue(newValue));
    }
    else if (command == fRot1Cmd)
    {
        fPrimaryGenerator->SetRot1Shift(fRot1Cmd->GetNewDoubleValue(newValue));
    }
    else if (command == fRot2Cmd)
    {
        fPrimaryGenerator->SetRot2Shift(fRot2Cmd->GetNewDoubleValue(newValue));
    }
    else if (command == fWholeTranslCmd)
    {
        fPrimaryGenerator->SetShiftOfWholeAssembly(fWholeTranslCmd->GetNew3VectorValue(newValue));
    }

}

