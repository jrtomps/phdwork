#ifndef ETA2NORMALIZER_H
#define ETA2NORMALIZER_H

#include "Eta2.h"

class Eta2Normalizer
{


public:
    Eta2Normalizer();
    Double_t operator()(const Eta2& eta);
};

#endif // ETA2NORMALIZER_H
