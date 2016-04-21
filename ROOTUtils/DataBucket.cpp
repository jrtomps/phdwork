
#include "TFileMergeInfo.h"
#include "TCollection.h"
#include "DataBucket.h"
#include "TNamed.h"
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
ClassImp(DataBucket)


DataBucket::DataBucket()
    :TNamed(),
    fIntMap(),
    fDoubleMap(),
    fStringMap()
{
  fTime           = -1;
  fNumberOfEvents = -1;

  fDoubleMap["elapsed_time (s)"] = 0;
  fIntMap["nevents"] = -1;
}

DataBucket::DataBucket(const DataBucket& orig)
 : TNamed(orig),
 fIntMap(orig.fIntMap),
 fDoubleMap(orig.fDoubleMap),
 fStringMap(orig.fStringMap)
{
  fTime           = orig.fTime;
  fNumberOfEvents = orig.fNumberOfEvents;
}


Int_t DataBucket::GetIntByKey(std::string key)
{
    Int_t res = 0;
    std::map<std::string, Int_t>::iterator it;
    it = fIntMap.find(key);
    if (it!=fIntMap.end())
        res = it->second;
    else
    {
        std::cerr << "No value exists for key " << key << std::endl;
    }
    return res;
}

Double_t DataBucket::GetDoubleByKey(std::string key)
{
    Double_t res = 0;
    std::map<std::string, Double_t>::iterator it;
    it = fDoubleMap.find(key);
    if (it!=fDoubleMap.end())
        res = it->second;
    else
    {
        std::cerr << "No value exists for key " << key << std::endl;
    }
    return res;
}

std::string DataBucket::GetStringByKey(std::string key)
{
    std::string res;
    std::map<std::string, std::string>::iterator it;
    it = fStringMap.find(key);
    if (it!=fStringMap.end())
        res = it->second;
    else
    {
        std::cerr << "No value exists for key " << key << std::endl;
    }
    return res;
}

std::map<std::string,Int_t>& DataBucket::GetIntMap() { return fIntMap;}

std::map<std::string,Double_t>& DataBucket::GetDoubleMap() { return fDoubleMap;}

std::map<std::string, std::string>& DataBucket::GetStringMap() { return fStringMap;}

Bool_t DataBucket::InsertInt(std::string key, Int_t val)
{
    std::pair<std::map<std::string, Int_t>::iterator, bool> res = fIntMap.insert(std::make_pair(key,val));

    if (!res.second) std::cout << "Failed to insert" << std::endl;

    return res.second;
}

Bool_t DataBucket::InsertDouble(std::string key, Double_t val)
{
    std::pair<std::map<std::string, Double_t>::iterator, bool> res = fDoubleMap.insert(std::make_pair(key,val));

    if (!res.second) std::cout << "Failed to insert" << std::endl;

    return res.second;
}

Bool_t DataBucket::InsertString(std::string key, std::string str)
{
    std::pair<std::map<std::string, std::string>::iterator, bool> res = fStringMap.insert(std::make_pair(key,str));

    if (!res.second) std::cout << "Failed to insert" << std::endl;

    return res.second;
}

void DataBucket::DumpContents()
{
    std::ios::fmtflags oflags = std::cout.setf(std::ios::left | std::ios::fixed);
    std::streamsize oprec = std::cout.precision(2);

    std::cout << "\nIntMap listings" << std::endl;
    std::map<std::string, Int_t>::iterator iit;
    for (iit=fIntMap.begin(); iit!=fIntMap.end(); iit++)
    {
        std::cout << std::setw(25) << iit->first
                << std::setw(12) << iit->second << std::endl;
    }

    std::cout << "\nDoubleMap listings" << std::endl;
    std::map<std::string, Double_t>::iterator dit;
    std::cout << std::resetiosflags(std::ios::fixed)
            << std::setiosflags(std::ios::scientific);
    for (dit=fDoubleMap.begin(); dit!=fDoubleMap.end(); dit++)
    {
        std::cout << std::setw(25) << dit->first
                << std::setw(12) << dit->second << std::endl;
    }
    std::cout << std::resetiosflags(std::ios::scientific);
    std::cout << std::setiosflags(std::ios::fixed);

    std::cout << "\nStringMap listings" << std::endl;
    std::map<std::string, std::string>::iterator sit;
    for (sit=fStringMap.begin(); sit!=fStringMap.end(); sit++)
    {
        std::cout << std::setw(25) << sit->first
                << std::setw(12) << sit->second << std::endl;
    }

    std::cout.setf(oflags);
    std::cout.precision(oprec);

}

Long64_t
DataBucket::Merge(TCollection* li)
{
   // Merge the DataBuckets in the TList to this.
    //
    // Returns the total number of events.
    //

    if (!li) return 0;
    TIter next(li);
    DataBucket *db;
    while (( db = dynamic_cast<DataBucket*>(next()) )) {
        if (db==NULL) continue;
        if (db==this) continue;
        if (!db->InheritsFrom(DataBucket::Class())) {
            Error("Add","Attempt to add object of class: %s to a %s", db->ClassName(), ClassName());
            return -1;
        }

        fNumberOfEvents += db->GetNumEvents();

// THIS NEEDS TO BE FIXED!!!
        // Iterate through the IntMap
        // First account for the entries that are the same
        // Second account for the entries that are present in one and
        //  missing in the other
        std::map<std::string, Int_t> vec;
        std::map<std::string, Int_t>::iterator it;

        std::map<std::string, Int_t>& dbimap = db->GetIntMap();
        std::set_intersection(fIntMap.begin(),fIntMap.end(),
                              dbimap.begin(), dbimap.end(),
                              std::inserter(vec, vec.end()),
                              vec.value_comp());
        for (it=vec.begin(); it!=vec.end(); it++)
        {
            fIntMap[it->first] += dbimap[it->first];
        }

        vec.clear();
        std::set_symmetric_difference(fIntMap.begin(),fIntMap.end(),
                                      dbimap.begin(), dbimap.end(),
                                      std::inserter(vec,vec.end()),
                                      vec.value_comp());
        for (it=vec.begin(); it!=vec.end(); it++)
        {
            // if this doesn't have a value associated with the key, then
            // add it and set it equal to the other one that doesn't have it
            if (fIntMap.find(it->first) == fIntMap.end())
                fIntMap[it->first] = dbimap[it->first];

            // if this does have a value associated with the key, then
            // the other one doesn't so we don't need to do anything
        }

        // Repeat for the fDoubleMap
        std::map<std::string, Double_t> dvec;
        std::map<std::string, Double_t>::iterator itd;
        std::map<std::string, Double_t>& dbdmap = db->GetDoubleMap();

        std::set_intersection(fDoubleMap.begin(),fDoubleMap.end(),
                                      dbdmap.begin(), dbdmap.end(),
                                      std::inserter(dvec,dvec.end()),
                                      dvec.value_comp());
        for (itd=dvec.begin(); itd!=dvec.end(); itd++)
        {
            fDoubleMap[itd->first] += dbdmap[itd->first];
        }

        dvec.clear();
        std::set_symmetric_difference(fDoubleMap.begin(),fDoubleMap.end(),
                                      dbdmap.begin(), dbdmap.end(),
                                      std::inserter(dvec,dvec.end()),
                                      dvec.value_comp());
        for (itd=dvec.begin(); itd!=dvec.end(); itd++)
        {
            // if this doesn't have a value associated with the key, then
            // add it and set it equal to the other one that doesn't have it
            if (fDoubleMap.find(itd->first) == fDoubleMap.end())
                fDoubleMap[itd->first] = dbdmap[itd->first];

            // if this does have a value associated with the key, then
            // the other one doesn't so we don't need to do anything
        }

// // this ends the part of debugging
    }

    return GetNumEvents();

}
