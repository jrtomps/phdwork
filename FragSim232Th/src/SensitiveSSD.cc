// SensitiveSSD.hh
//
// Jeromy Tompkins
// 9/20/2011
//
// A class that constructs an SSD with an associated 
// SSDSensitivity object.

#include <iostream>
#include "SSDBuilder.hh"
#include "SSDSensitivity.hh"
#include "SensitiveSSD.hh"

SensitiveSSD::SensitiveSSD(const G4String name,
		       const G4int detID)
  : ftheBuilder(),
    ftheSensitivity(new SSDSensitivity(name,detID))
{
  ftheBuilder.SetSensitivity(ftheSensitivity);
  std::cout << "Instance of SensitiveSSD constructed : "
	  << name.data() << std::endl;
}

SensitiveSSD::~SensitiveSSD()
{
  // Do not delete the sensitivity b/c it is owned by
  // the SDManager!!
  //  delete ftheSensitivity;
  std::cout << "Instance of SensitiveSSD destructed : "
	  << ftheSensitivity->GetDetName().data() << std::endl;
}

G4AssemblyVolume*
SensitiveSSD::GetADetector(void)
{
  return ftheBuilder.GetADetector();
}
