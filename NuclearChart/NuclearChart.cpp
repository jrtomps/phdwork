#include <cstdlib>
#include <regex>
#include "NuclearChart.h"

ClassImp(NuclearChart);

NuclearChart* NuclearChart::theInstance = 0;

NuclearChart* NuclearChart::GetInstance()
{
    if (theInstance==0) theInstance = new NuclearChart();

    return theInstance;
}

NuclearChart::NuclearChart()
    : fNameMap(),
    fZMap()
{
    SetUpNameMap();
    SetUpZMap();
}

void NuclearChart::SetUpNameMap()
{
    fNameMap["H"]  = 1;
    fNameMap["He"] = 2;
    fNameMap["Li"] = 3;
    fNameMap["Be"] = 4;
    fNameMap["B"]  = 5;
    fNameMap["C"]  = 6;
    fNameMap["N"]  = 7;
    fNameMap["O"]  = 8;
    fNameMap["F"]  = 9;
    fNameMap["Ne"] = 10;
    fNameMap["Na"] = 11;
    fNameMap["Mg"] = 12;
    fNameMap["Al"] = 13;
    fNameMap["Si"] = 14;
    fNameMap["P"]  = 15;
    fNameMap["S"]  = 16;
    fNameMap["Cl"] = 17;
    fNameMap["Ar"] = 18;
    fNameMap["K"]  = 19;
    fNameMap["Ca"] = 20;
    fNameMap["Sc"] = 21;
    fNameMap["Ti"]  = 22;
    fNameMap["V"]  = 23;
    fNameMap["Cr"] = 24;
    fNameMap["Mn"] = 25;
    fNameMap["Fe"] = 26;
    fNameMap["Co"] = 27;
    fNameMap["Ni"] = 28;
    fNameMap["Cu"] = 29;
    fNameMap["Zn"] = 30;
    fNameMap["Ga"] = 31;
    fNameMap["Ge"] = 32;
    fNameMap["As"] = 33;
    fNameMap["Se"] = 34;
    fNameMap["Br"] = 35;
    fNameMap["Kr"] = 36;
    fNameMap["Rb"] = 37;
    fNameMap["Sr"] = 38;
    fNameMap["Y"]  = 39;
    fNameMap["Zr"] = 40;
    fNameMap["Nb"] = 41;
    fNameMap["Mo"] = 42;
    fNameMap["Tc"] = 43;
    fNameMap["Ru"] = 44;
    fNameMap["Rh"] = 45;
    fNameMap["Pd"] = 46;
    fNameMap["Ag"] = 47;
    fNameMap["Cd"] = 48;
    fNameMap["In"] = 49;
    fNameMap["Sn"] = 50;
    fNameMap["Sb"] = 51;
    fNameMap["Te"] = 52;
    fNameMap["I"]  = 53;
    fNameMap["Xe"] = 54;
    fNameMap["Cs"] = 55;
    fNameMap["Ba"] = 56;
    fNameMap["La"] = 57;
    fNameMap["Ce"] = 58;
    fNameMap["Pr"] = 59;
    fNameMap["Nd"] = 60;
    fNameMap["Pm"] = 61;
    fNameMap["Sm"] = 62;
    fNameMap["Eu"] = 63;
    fNameMap["Gd"] = 64;
    fNameMap["Tb"] = 65;
    fNameMap["Dy"] = 66;
    fNameMap["Ho"] = 67;
    fNameMap["Er"] = 68;
    fNameMap["Tm"] = 69;
    fNameMap["Yb"] = 70;
    fNameMap["Lu"] = 71;
    fNameMap["Hf"] = 72;
    fNameMap["Ta"] = 73;
    fNameMap["W"]  = 74;
    fNameMap["Re"] = 75;
    fNameMap["Os"] = 76;
    fNameMap["Ir"] = 77;
    fNameMap["Pt"] = 78;
    fNameMap["Au"] = 79;
    fNameMap["Hg"] = 80;
    fNameMap["Tl"] = 81;
    fNameMap["Pb"] = 82;
    fNameMap["Bi"] = 83;
    fNameMap["Po"] = 84;
    fNameMap["At"] = 85;
    fNameMap["Rn"] = 86;
    fNameMap["Fr"] = 87;
    fNameMap["Ra"] = 88;
    fNameMap["Ac"] = 89;
    fNameMap["Th"] = 90;
    fNameMap["Pa"] = 91;
    fNameMap["U"] = 92;
    fNameMap["Np"] = 93;
    fNameMap["Pu"] = 94;
    fNameMap["Am"] = 95;
    fNameMap["Cm"] = 96;
    fNameMap["Bk"] = 97;
    fNameMap["Cf"] = 98;
    fNameMap["Es"] = 99;
    fNameMap["Fm"] = 100;
    fNameMap["Md"] = 101;
    fNameMap["No"] = 102;
    fNameMap["Lr"] = 103;
    fNameMap["Rf"] = 104;
    fNameMap["Db"] = 105;
    fNameMap["Sg"] = 106;
    fNameMap["Bh"] = 107;
    fNameMap["Hs"] = 108;
    fNameMap["Mt"] = 109;
    fNameMap["Ds"] = 110;
    fNameMap["Rg"] = 111;
    fNameMap["Cn"] = 112;
    fNameMap["Uut"] = 113;
    fNameMap["Uuq"] = 114;
    fNameMap["Uup"] = 115;
    fNameMap["Uuh"] = 116;
    fNameMap["Uus"] = 117;
    fNameMap["Uuo"] = 118;

}

Isotope NuclearChart::ParseName(const std::string &name)
{
    Isotope iso; iso.A=0; iso.Z=0;

    std::smatch match;
    std::regex expr ("([A-Z]+[a-z]*)([0-9]+)");
    if (regex_search(name.begin(),name.end(),match, expr))
    {
        iso.Z = fNameMap[match[1].str()];
        iso.A = ::atoi(match[2].str().data());
    }

    return iso;
}

std::string NuclearChart::GetName(UInt_t Z)
{
    return fZMap.find(Z)->second;
}

void NuclearChart::SetUpZMap()
{
    std::map<std::string, UInt_t>::iterator it;
    for (it=fNameMap.begin(); it!=fNameMap.end(); it++)
    {
        fZMap[it->second] = it->first;
    }
}
