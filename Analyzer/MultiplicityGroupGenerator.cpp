
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "MultiplicityGroupGenerator.h"

MultiplicityGroupGenerator::MultiplicityGroupGenerator(UInt_t n_events, UInt_t n_regions)
    : fNEvents(n_events), fNRegions(n_regions), fCombos()
{
    GenerateCombos();
}

MultiplicityGroupGenerator::MultiplicityGroupGenerator(const MultiplicityGroupGenerator &obj)
    : fNEvents(obj.fNEvents), fNRegions(obj.fNRegions), fCombos(obj.fCombos)
{
}

MultiplicityGroupGenerator& MultiplicityGroupGenerator::operator=(MultiplicityGroupGenerator obj)
                                                                 {
    swap(*this, obj);
    return *this;
}

void MultiplicityGroupGenerator::PrintCombos()
{
    std::for_each(fCombos.begin(), fCombos.end(), print);
}

void MultiplicityGroupGenerator::GenerateCombos()
{
    GenerateRawCombos(fNEvents-1,"",fNRegions-1);
//    std::cout << "Raw combos" << std::endl;
//    PrintCombos();

    ConsolidateCombos();
//    std::cout << "\nConsolidatedCombos" << std::endl;
//    PrintCombos();

}

void MultiplicityGroupGenerator::GenerateRawCombos(UInt_t pos,
                                                   std::string curr,
                                                   UInt_t max_val)
{
    UInt_t cval = 0;

    while (cval<=max_val)
    {
        std::ostringstream ss(curr, std::ios::out | std::ios::app);
        ss << cval;

        if (pos>0)
            GenerateRawCombos(pos-1, ss.str(), max_val);
        else
            fCombos.insert(ss.str());

        cval++;
    }
}

std::string MultiplicityGroupGenerator::ConsolidateCombo(const std::string raw_combo, UInt_t n_regions)
{
    UInt_t i;
    UInt_t c;
    std::stringstream ss;
    for (UInt_t i=0; i<n_regions; i++) ss << "0";

    std::istringstream iss(raw_combo);
    Char_t cc='\0', cc2='\0';

    UInt_t n=0;

    iss >> cc;
    do
    {
        try
        {
            c = boost::lexical_cast<UInt_t>(cc);
        }
        catch (boost::bad_lexical_cast&)
        { continue;}

        ss.seekg(c,std::ios::beg);
        try
        {
            ss >> cc2;
            i = boost::lexical_cast<UInt_t>(cc2);
        }
        catch (boost::bad_lexical_cast&)
        { continue;}

        ss.seekp(c,std::ios::beg);
        ss << (++i);

        if (iss.eof()) break;

        iss >> cc;

    } while (!iss.fail());

    return ss.str();
}

std::string MultiplicityGroupGenerator::ConsolidateCombo(std::string str)
{
    return ConsolidateCombo(str, fNRegions);
}

void MultiplicityGroupGenerator::ConsolidateCombos()
{
    std::set<std::string> cons_combos;

    std::set<std::string>::iterator it;

    it=fCombos.begin();
    for (; it!=fCombos.end(); it++)
    {
        cons_combos.insert(ConsolidateCombo(*it));
    }
    fCombos.swap(cons_combos);
}




