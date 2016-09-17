#ifndef STRIPGROUPER_H
#define STRIPGROUPER_H

#include "TRint.h"
#include <set>
#include <vector>
#include "TString.h"

typedef std::set<UInt_t> StripGroup;
typedef std::set<UInt_t> DetectorGroup;

class StripGrouper
{
public:
    StripGrouper();
    ~StripGrouper();

    void LoadGroupings(const TString& fname);

private:
    bool AllStripsAccountedFor(void) const;
    void LoadStripGroup(TString& aline);
    void LoadDetectorGroup(TString& aline);


    UInt_t fStripOffset;
    UInt_t fNStripsPerDet;
    std::vector<StripGroup> fStripGroups;
    std::vector<DetectorGroup> fDetectorGroups;

};

#endif // STRIPGROUPER_H
