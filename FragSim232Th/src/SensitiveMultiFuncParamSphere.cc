// SensitiveMultiFuncParamSphere.hh
//
// Jeromy Tompkins
// 4/13/2012
//

#include <iostream>
#include "G4MultiFunctionalDetector.hh"
#include "G4Material.hh"
#include "G4String.hh"
#include "ParameterisedSphereBuilder.hh"
#include "SensitiveMultiFuncParamSphere.hh"


SensitiveMultiFuncParamSphere::SensitiveMultiFuncParamSphere()
    : ftheBuilder(),
    ftheSensitivity(0),
    fcollectionID(0)
{}

SensitiveMultiFuncParamSphere::SensitiveMultiFuncParamSphere(const G4String name,
                                                             G4double iradius, G4double oradius,
                                                             G4double deltaTheta, G4double deltaPhi,
                                                             G4Material *mat)
 : ftheBuilder(iradius, oradius, deltaTheta, deltaPhi, mat),
    ftheSensitivity(new G4MultiFunctionalDetector(name)),
    fcollectionID(0)
{
  ftheBuilder.SetSensitivity(ftheSensitivity);
  std::cout << "Instance of SensitiveMultiFuncParamSphere constructed : "
          << name.data() << std::endl;
}

SensitiveMultiFuncParamSphere::SensitiveMultiFuncParamSphere(const SensitiveMultiFuncParamSphere &obj)
    : ftheBuilder(obj.ftheBuilder),
    ftheSensitivity(new G4MultiFunctionalDetector(obj.ftheSensitivity->GetName())),
    fcollectionID(obj.fcollectionID)
{
}

SensitiveMultiFuncParamSphere& SensitiveMultiFuncParamSphere::operator=(SensitiveMultiFuncParamSphere obj)
{
    swap(*this, obj);
    return *this;
}

SensitiveMultiFuncParamSphere::~SensitiveMultiFuncParamSphere()
{
  // Do not delete the sensitivity b/c it is owned by
  // the SDManager!!
  //  delete ftheSensitivity;
//  std::cout << "Instance of SensitiveMultiFuncParamSphere destructed : "
//          << ftheSensitivity->GetName().data() << std::endl;
}

G4AssemblyVolume*
SensitiveMultiFuncParamSphere::GetADetector(void)
{
  return ftheBuilder.GetADetector();
}
