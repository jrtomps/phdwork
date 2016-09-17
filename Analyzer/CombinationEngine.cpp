#include "Rtypes.h"
#include <vector>
#include <string>
#include <sstream>
#include "CombinationEngine.h"

CombinationEngine::CombinationEngine(UInt_t n_choices, UInt_t n_vals)
    : fNChoices(n_choices),
    fNVals(n_vals),
    fCombos()
{
    GenerateCombinations();
}

CombinationEngine::CombinationEngine(const CombinationEngine &obj)
    : fNChoices(obj.fNChoices),
    fNVals(obj.fNVals),
    fCombos(obj.fCombos)
{
}

CombinationEngine& CombinationEngine::operator=(CombinationEngine obj)
{
    swap(*this,obj);
    return *this;
}

void CombinationEngine::GenerateCombinations(void)
{
    GenerateCombinations(fNChoices-1,"",fNVals-1);
}

void
CombinationEngine::GenerateCombinations(UInt_t pos,
                                        std::string curr,
                                        UInt_t max_val)
{
    UInt_t cval = 0;
    if (curr.length()>0)
    {
        std::istringstream ss(curr);
        ss.seekg(-1,std::ios::end);
        ss >> cval;
        if (ss.fail()) throw CombinationEngineFailure();
    }

    while (cval<=max_val)
    {
        std::ostringstream ss(curr, std::ios::out | std::ios::app);
        ss << cval;

        if (pos>0)
            GenerateCombinations(pos-1, ss.str(), max_val);
        else
            fCombos.push_back(ss.str());

        cval++;
    }
}
