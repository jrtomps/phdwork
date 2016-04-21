#ifndef FragSimDetMessenger_HH
#define FragSimDetMessenger_HH

#include "G4UImessenger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"

class G4UIdirectory;

class FragSimDetConstruction;

class FragSimDetMessenger : public G4UImessenger
{
public:
    FragSimDetMessenger(FragSimDetConstruction* source);
    ~FragSimDetMessenger();
    FragSimDetMessenger(const FragSimDetMessenger& obj);

    void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    FragSimDetConstruction* fPrimaryGenerator;

    G4UIdirectory*                        srcDir;
    G4UIcmdWith3VectorAndUnit*          fTransCmd;
    G4UIcmdWithADoubleAndUnit*   fRot1Cmd;
    G4UIcmdWithADoubleAndUnit*   fRot2Cmd;
    G4UIcmdWith3VectorAndUnit*  fWholeTranslCmd;
};

#endif // FragSimDetMessenger_HH
