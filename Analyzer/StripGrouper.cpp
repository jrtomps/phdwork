#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "TString.h"
#include "StripGrouper.h"

StripGrouper::StripGrouper()
    : fStripOffset(0),
    fNStripsPerDet(16),
    fStripGroups(),
    fDetectorGroups()
{
}

StripGrouper::~StripGrouper() throw()
{

}

void StripGrouper::LoadGroupings(const TString& fname)
{

    std::ifstream ifile (fname.Data(),std::ifstream::in);
    if (!ifile.is_open())
    {
        std::cerr << fname << " doesn't exist" << std::endl;
        return;
    }

    while(1)
    {
        TString aline;
        aline.ReadLine(ifile);
        UInt_t val;
        if (ifile.eof() || ifile.bad())
            break;
        else if (aline.Length()==0)
            continue;
        else if (aline.Contains("detector_group"))
        {
            LoadDetectorGroup(aline);
        }
        else if (aline.Contains("strip_group"))
        {
            LoadStripGroup(aline);
        }
        else
            continue;
    }

    if (! AllStripsAccountedFor())
    {
        std::cerr << "Not all strips have been accounted for!" << std::endl;
    }

}

void LoadStripGroup(TString& aline)
{
    aline.ReplaceAll("strip_group","");
    aline.Trim();

    StripGroup agroup;
    std::stringstream ss;
    ss << aline.Data();
    while(1)
    {
        ss >> val;
        if (ss.rdstate()!=0)
            break;
        else
            agroup.insert(val);
    }
    fStripGroups.push_back(agroup);

}

void LoadDetectorGroup(TString& aline)
{
    aline.ReplaceAll("detector_group","");
    aline.Trim();

    DetectorGroup agroup;
    std::stringstream ss;
    ss << aline.Data();
    while(1)
    {
        ss >> val;
        if (ss.rdstate()!=0)
            break;
        else
            agroup.insert(val);
    }
    fDetectorGroups.push_back(agroup);

}

bool StripGrouper::AllStripsAccountedFor(void) const
{
    std::vector<StripGroup>::const_iterator it;
    StripGroup::const_iterator sg_it;

    std::vector<UInt_t> vals(fNStrips,0);
    std::vector<UInt_t>::iterator v_it;
    for (UInt_t i=fStripOffset; i<(fNStrips+fStripOffset); i++)
    {
        vals[i] = i;
    }

    for (it=fStripGroupings.begin(); it!=fStripGroupings.end(); it++)
    {
        for (sg_it=it->begin(); sg_it!=it->end(); sg_it++)
        {
            v_it = std::find(vals.begin(),vals.end(),*sg_it);
            if (v_it!=vals.end())
                *v_it = fNStrips+fStripOffset;
        }
    }

    bool allaccounted=true;
    for (v_it=vals.begin(); v_it!=vals.end(); v_it++)
    {
        if (*v_it != fNStrips+fStripOffset)
        {
            allaccounted = false;
            break;
        }
    }
    return allaccounted;
}

