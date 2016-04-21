// Target.hh
//
// Jeromy Tompkins
// 10/20/2011
//

#include "globals.hh"

#ifndef TARGET_HH
#define TARGET_HH 1

class Target 
{
public:
  Target (void);
  Target(Target const& that);
  Target& operator=(Target const& that);

  ~Target(void);

  G4double GetThickness(void) const;
  G4double GetLength(void) const;
  G4double GetWidth(void) const;

private:
  G4double fThickness;
  G4double fLength;
  G4double fWidth;

};
 
#endif
