#ifndef FragSimRunActionMESSENGER_HH
#define FragSimRunActionMESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class FragSimRunAction;

class FragSimRunActionMessenger : public G4UImessenger
{
public:
    FragSimRunActionMessenger(FragSimRunAction* source);
    ~FragSimRunActionMessenger();
    FragSimRunActionMessenger(const FragSimRunActionMessenger& obj);

    void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    FragSimRunAction* fRunAction;

    G4UIdirectory*                 srcDir;
    G4UIcmdWithAString*        runNameCmd;
};

#endif // FragSimRunActionMESSENGER_HH
