#ifndef COMBINATIONENGINE_H
#define COMBINATIONENGINE_H

#include "Rtypes.h"
#include <vector>
#include <string>
#include <algorithm>

struct CombinationEngineFailure {};

class CombinationEngine
{
public:
    CombinationEngine(UInt_t n_choices, UInt_t n_vals);
    CombinationEngine(const CombinationEngine& obj);
    CombinationEngine& operator=(CombinationEngine obj);

    friend void swap(CombinationEngine& lhs, CombinationEngine& rhs);

    void SetNChoices(UInt_t n) {fNChoices = n; }
    void SetNVals(UInt_t n) {fNVals = n; }

    UInt_t GetNChoices(void) const {return fNChoices;}
    UInt_t GetNVals(void) const {return fNVals;}

    std::vector<std::string>& GetCombos(void) {return fCombos; }

private:
    void GenerateCombinations(void);
    void GenerateCombinations(UInt_t npos, std::string curr, UInt_t max_val);

    UInt_t fNChoices;
    UInt_t fNVals;
    std::vector<std::string> fCombos;

};

inline void swap(CombinationEngine &lhs, CombinationEngine &rhs)
{
    std::swap(lhs.fNChoices,rhs.fNChoices);
    std::swap(lhs.fNVals, rhs.fNVals);
    std::swap(lhs.fCombos, rhs.fCombos);
}


#endif // COMBINATIONENGINE_H
