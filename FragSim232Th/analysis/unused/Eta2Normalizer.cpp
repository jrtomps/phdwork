#include "Eta2.h"
#include "Eta2Normalizer.h"

Eta2Normalizer::Eta2Normalizer()
{
}


Double_t Eta2Normalizer::operator ()(const Eta2& eta)
{
    // The normalization for the length will be independently determined
    //
    TH3 *h = eta.GetTH3()
}
