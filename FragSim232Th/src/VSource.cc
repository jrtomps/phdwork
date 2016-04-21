// VSource.cc
//
// Jeromy Tompkins
// 10/19/2011
//


#include <iostream>
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "VSource.hh"

VSource::VSource()
    : fTransform(),
    fRotation(),
    fTranslation(),
    fPlaneNorm()
{}

VSource::VSource(G4Transform3D const& trans) 
  : fTransform(trans), 
    fRotation(trans.getRotation()), 
    fTranslation(trans.getTranslation()),
    fPlaneNorm()
{
  std::cout << "Constructing VSource " << std::endl;
}

VSource::VSource(VSource const& obj) 
  : fTransform(obj.fTransform),
    fRotation(obj.fRotation),
    fTranslation(obj.fTranslation),
    fPlaneNorm(obj.fPlaneNorm)
{
  std::cout << "Copy Constructing VSource " << std::endl;
}

VSource&
VSource::operator=(VSource const& obj) 
{
  std::cout << "Assignment operator VSource " << std::endl;
  if (this != &obj)
    {
      fTransform   = obj.fTransform;
      fRotation    = obj.fRotation;
      fTranslation = obj.fTranslation;
      fPlaneNorm   = obj.fPlaneNorm;
    }
  return *this;
}

void          
VSource::SetTransform(const G4Transform3D& trans)
{
  fTransform   = trans;
  fRotation    = fTransform.getRotation();
  fTranslation = fTransform.getTranslation();
  fPlaneNorm   = fRotation.colZ();//colX().cross(fRotation.colY());
  
}
