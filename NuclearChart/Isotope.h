
#ifndef ISOTOPE_H
#define ISOTOPE_H

#include <ostream>
#include "Rtypes.h"

class Isotope
{
public:
    UInt_t Z;
    UInt_t A;
	
	Isotope(); 
	Isotope(const UInt_t Z, const UInt_t A); 

	ClassDef(Isotope,0);
};


inline std::ostream& operator<<(std::ostream& stream, 
								Isotope& iso)
{
	stream << "A=" << iso.A << "\tZ=" << iso.Z;
	return stream;
}


#endif
