#ifndef MULTIPLICITYGROUPGENERATOR_H
#define MULTIPLICITYGROUPGENERATOR_H

#include <set>
#include <string>
#include "Rtypes.h"

class MultiplicityGroupGenerator
{
public:
    MultiplicityGroupGenerator(UInt_t n_events, UInt_t n_regions);
    ~MultiplicityGroupGenerator() throw() {};
    MultiplicityGroupGenerator(const MultiplicityGroupGenerator& obj);
    MultiplicityGroupGenerator& operator=(MultiplicityGroupGenerator obj);

    std::set<std::string>& GetCombos(void) { return fCombos; }

    void PrintCombos();

    static std::string ConsolidateCombo(const std::string raw_combo, UInt_t n_regions);

    friend void swap(MultiplicityGroupGenerator& lhs, MultiplicityGroupGenerator& rhs) throw();

private:
    void GenerateCombos(void);
    void GenerateRawCombos(UInt_t pos, std::string curr, UInt_t max_val);
    void ConsolidateCombos(void);
    std::string ConsolidateCombo(std::string str);
    UInt_t fNEvents;
    UInt_t fNRegions;
    std::set<std::string> fCombos;
};

inline
void swap(MultiplicityGroupGenerator &lhs, MultiplicityGroupGenerator &rhs) throw()
{
    using std::swap;
    swap(lhs.fNEvents, rhs.fNEvents);
    swap(lhs.fNRegions, rhs.fNRegions);
    lhs.fCombos.swap(rhs.fCombos);
}

inline
void print(std::string str)
{
    std::cout << str << std::endl;
}

#endif // MULTIPLICITYGROUPGENERATOR_H
