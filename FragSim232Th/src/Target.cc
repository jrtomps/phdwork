// Target.cc
//
// Jeromy Tompkins
// 10/20/2011
//

#include "globals.hh"
#include "Target.hh"


Target::Target (void)
  : fThickness(0.10*cm),
    fLength(5.01*cm),
    fWidth(2.80*cm)
{
}

Target::Target(Target const& that)
  : fThickness(that.fThickness),
    fLength(that.fLength),
    fWidth(that.fWidth)
{
}

Target& 
Target::operator=(Target const& that)
{
  if (this != &that)
    {
      fThickness = that.fThickness;
      fLength    = that.fLength;
      fWidth     = that.fWidth;
    }
  return *this;
}

Target::~Target(void) {}

G4double 
Target::GetThickness(void) const
{ return fThickness;}

G4double 
Target::GetLength(void) const
{ return fLength; }

G4double 
Target::GetWidth(void) const
{ return fWidth;}
