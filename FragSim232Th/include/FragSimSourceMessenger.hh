#ifndef FRAGSIMSOURCEMESSENGER_HH
#define FRAGSIMSOURCEMESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class FragSimPrimaryGeneratorAction;
class FragSimSource;
class FragSimPrimaryGeneratorAction;

class FragSimSourceMessenger : public G4UImessenger
{
public:
    FragSimSourceMessenger(FragSimSource* source);
    ~FragSimSourceMessenger();
    FragSimSourceMessenger(const FragSimSourceMessenger& obj);

    void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    FragSimSource* fFSSource;

    G4UIdirectory*                        srcDir;
    G4UIcmdWithAString*               srcTypeCmd;
    G4UIcmdWithADoubleAndUnit*   srcSurfThickCmd;
};

#endif // FRAGSIMSOURCEMESSENGER_HH
