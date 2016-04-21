#include <sstream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "FragSimPrimaryGeneratorAction.hh"
#include "FragSimPrimGenActionMessenger.hh"
#include "G4String.hh"

FragSimPrimGenActionMessenger::FragSimPrimGenActionMessenger(FragSimPrimaryGeneratorAction* source)
    : fPrimaryGenerator(source)
{
    srcDir = new G4UIdirectory("/FragSim02/generator/");
    srcDir->SetGuidance("Commands for controlling the FragSimPrimGenAction");

    usrDistrParamCmd = new G4UIcmdWithAString("/FragSim02/generator/set_user_distribution",this);
    std::ostringstream guidance;
    guidance << "Enter name of file with parameters to set up user distribution."
            << "\nUsage of this function forces the defined user distribution"
            << "\nto be sampled when choosing a momentum direction.";
    usrDistrParamCmd->SetGuidance(guidance.str().data());
    usrDistrParamCmd->SetParameterName("param_file_name",true);
    usrDistrParamCmd->SetDefaultValue("macros/default.mac");

    setMassDistrCmd = new G4UIcmdWithABool("/FragSim02/generator/set_mass_distribution",this);
    guidance.str(""); guidance.clear();

    guidance << "Enter true to generate fragments from an "
            << "\nexp. mass and energy distribution";
    setMassDistrCmd->SetGuidance(guidance.str().data());
    setMassDistrCmd->SetParameterName("use_it",true);
    setMassDistrCmd->SetDefaultValue(false);

}

FragSimPrimGenActionMessenger::FragSimPrimGenActionMessenger(const FragSimPrimGenActionMessenger &obj)
    : fPrimaryGenerator(obj.fPrimaryGenerator), // this doesn't own the PrimaryGenerator
    srcDir(new G4UIdirectory(*obj.srcDir)),
    usrDistrParamCmd(new G4UIcmdWithAString(*obj.usrDistrParamCmd)),
    setMassDistrCmd(new G4UIcmdWithABool(*obj.setMassDistrCmd))
{
}

FragSimPrimGenActionMessenger::~FragSimPrimGenActionMessenger()
{
    if (srcDir) { delete srcDir; srcDir=0;}
    if (usrDistrParamCmd) { delete usrDistrParamCmd; usrDistrParamCmd=0;}
    if (setMassDistrCmd) { delete setMassDistrCmd; setMassDistrCmd=0;}
}

void FragSimPrimGenActionMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == usrDistrParamCmd)
    {
        fPrimaryGenerator->SetUserDistribution(newValue);
    }

    if (command == setMassDistrCmd)
    {
        G4cout << "MassDistr Command = " << setMassDistrCmd->GetCurrentValue() << G4endl;
        if (setMassDistrCmd->GetNewBoolValue(newValue))
            fPrimaryGenerator->SetupMassDistribution();
        else
            G4cout << "skipped the command" <<  G4endl;
    }
}

