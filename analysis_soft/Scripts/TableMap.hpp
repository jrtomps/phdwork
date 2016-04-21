
#ifndef TABLEMAP_H
#define TABLEMAP_H 1

#include <map>
#include <iostream>
#include <iomanip>
#include "TString.h"

template<typename Iterator>
void
PrintTable(std::map<TString,Iterator>& tmap, Int_t nentries)
{
    typename std::map<TString,Iterator>::iterator it;

    /* cout << setiosflags(ios::fixed); */
    std::cout << std::setprecision(6);
    std::cout << "\n" << std::setw(4) << " ";
    for (it=tmap.begin(); it!=tmap.end(); it++)
        std::cout << std::right << std::setw(10) << it->first.Data();

    for (Int_t index=0; index<nentries; index++)
    {
        std::cout << "\n"
                  << std::left
                  << std::setw(4) << index;
        for (it=tmap.begin(); it!=tmap.end(); it++)
            std::cout << std::right
                      << std::setw(10) << it->second[index][0];
    }
    /* cout << resetiosflags(ios::fixed); */
    std::cout << std::endl;
}


template<class DataContainer>
void PrintTable(std::map<TString,DataContainer>& tmap)
{
    typename std::map<TString,DataContainer>::iterator it;
    typename std::vector<Double_t>::iterator it2;

    /* cout << setiosflags(ios::fixed); */
    std::cout << std::setprecision(6);
    std::cout << "\n" << std::setw(4) << " ";
    for (it=tmap.begin(); it!=tmap.end(); it++)
        std::cout << std::right << std::setw(10) << it->first.Data();

    for (Int_t index=0; index<tmap.begin()->second.size(); index++)
    {
        std::cout << "\n"
                  << std::left
                  << std::setw(4) << index;
        for (it=tmap.begin(); it!=tmap.end(); it++)
        {
            for (it2=it->second.begin(); it2!=it->second.end(); it2++ )
            {
                std::cout << std::right << std::setw(10) << (*it2);
            }
            if ((it+1)!=tmap.end()) std::cout << " | ";
        }
    }
    /* cout << resetiosflags(ios::fixed); */
    std::cout << std::endl;
}

#endif

/*
std::map<TString,double*> tablemap;
tablemap["Gflash"] = gflash;
tablemap["Det_dist"] = det_dist;
tablemap["Ped"] = ped;
tablemap["Adc_edge"] = adc_edge;
tablemap["NxCs"] = NxCs;
tablemap["Deut_en"] = deut_en;
tablemap["TDC_off"] = tdc_offset;
PrintTable(tablemap, ndet+1);
*/
