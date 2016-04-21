#include "Isotope.h"



ClassImp(Isotope);

Isotope::Isotope() : Z(0), A(0) {}

Isotope::Isotope(const UInt_t z, 
				const UInt_t a) : Z(z), A(a) {}

