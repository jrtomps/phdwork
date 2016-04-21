#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "FragSimSource.hh"
#include "FragSimSourceMessenger.hh"
#include "G4String.hh"

FragSimSourceMessenger::FragSimSourceMessenger(FragSimSource* source)
    : fFSSource(source)
{
    srcDir = new G4UIdirectory("/source/");
    srcDir->SetGuidance("Commands for controlling the FragSimSource");

    srcTypeCmd = new G4UIcmdWithAString("/source/type",this);
    srcTypeCmd->SetGuidance("Set source type.");
    srcTypeCmd->SetGuidance("Choices: volume, surface_layer, surface");
    srcTypeCmd->SetParameterName("type",true);
    srcTypeCmd->SetDefaultValue("surface");

    srcSurfThickCmd = new G4UIcmdWithADoubleAndUnit("/source/surface_thickness",this);
    srcSurfThickCmd->SetGuidance("Sets the thickness of surface layer.");
    srcSurfThickCmd->SetParameterName("Thickness",true);
    srcSurfThickCmd->SetUnitCategory("Length");
    srcSurfThickCmd->SetRange("Thickness>0.0");
    srcSurfThickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    srcSurfThickCmd->SetDefaultValue(1.0*um);
}

FragSimSourceMessenger::FragSimSourceMessenger(const FragSimSourceMessenger &obj)
    : srcDir(new G4UIdirectory(*obj.srcDir)),
    srcTypeCmd(new G4UIcmdWithAString(*obj.srcTypeCmd)),
    srcSurfThickCmd(new G4UIcmdWithADoubleAndUnit(*obj.srcSurfThickCmd))
{
}

FragSimSourceMessenger::~FragSimSourceMessenger()
{
    if (srcDir) { delete srcDir; srcDir=0;}
    if (srcTypeCmd) { delete srcTypeCmd; srcTypeCmd=0;}
    if (srcSurfThickCmd) { delete srcSurfThickCmd; srcSurfThickCmd=0;}
}

void FragSimSourceMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == srcTypeCmd)
    {
        fFSSource->SetType(newValue);
    }
    else if (command==srcSurfThickCmd)
    {
        fFSSource->SetSurfaceThickness(srcSurfThickCmd->GetNewDoubleValue(newValue));
    }
}

