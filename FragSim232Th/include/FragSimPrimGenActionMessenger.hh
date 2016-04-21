#ifndef FragSimPrimGenActionMESSENGER_HH
#define FragSimPrimGenActionMESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class FragSimPrimaryGeneratorAction;

class FragSimPrimGenActionMessenger : public G4UImessenger
{
public:
    FragSimPrimGenActionMessenger(FragSimPrimaryGeneratorAction* source);
    ~FragSimPrimGenActionMessenger();
    FragSimPrimGenActionMessenger(const FragSimPrimGenActionMessenger& obj);

    void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    FragSimPrimaryGeneratorAction* fPrimaryGenerator;

    G4UIdirectory*                        srcDir;
    G4UIcmdWithAString*                usrDistrParamCmd;
    G4UIcmdWithABool*                   setMassDistrCmd;
};

#endif // FragSimPrimGenActionMESSENGER_HH
