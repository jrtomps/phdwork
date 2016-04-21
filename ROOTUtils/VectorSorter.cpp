#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <utility>
#include <queue>
#include "Rtypes.h"
#include "VectorSorter.h"

VectorSorter::VectorSorter()
    : fData(NULL),
    fSortedData(NULL)
{
    fData = new std::vector<std::vector<Double_t> >;
    fSortedData = new std::vector<std::vector<Double_t> >;
}

VectorSorter::~VectorSorter()
{
    delete fData;
    delete fSortedData;
}

VectorSorter::VectorSorter(const VectorSorter& that)
    : fData(NULL), fSortedData(NULL)
{
    if (this!=&that)
    {
        std::vector<std::vector<Double_t> >* tmp1, *tmp2;
        tmp1 = new std::vector<std::vector<Double_t> >(*that.fData);
        tmp2 = new std::vector<std::vector<Double_t> >(*that.fSortedData);

        delete fData;
        delete fSortedData;

        fData = tmp1;
        fSortedData = tmp2;
    }
}

VectorSorter& VectorSorter::operator=(const VectorSorter& that)
                                     {
    if (this!=&that)
    {
        std::vector<std::vector<Double_t> >* tmp1, *tmp2;
        tmp1 = new std::vector<std::vector<Double_t> >(*that.fData);
        tmp2 = new std::vector<std::vector<Double_t> >(*that.fSortedData);

        delete fData;
        delete fSortedData;

        fData = tmp1;
        fSortedData = tmp2;
    }
    return *this;
}

void VectorSorter::Clear()
{
    fData->clear();
    fSortedData->clear();
}

std::vector<std::vector<Double_t> >::iterator VectorSorter::RegisterVector(const std::vector<Double_t> &vec)
{
    fData->push_back(vec);
    return fData->begin()+(fData->size()-1);
}

std::vector<Double_t> VectorSorter::GetUnsortedVector(UInt_t index)
{
    return fData->at(index);
}

std::vector<Double_t> VectorSorter::GetSortedVector(UInt_t index)
{
    return fSortedData->at(index);
}

void VectorSorter::Sort(UInt_t index)
{
    if (index >= fData->size())
    {
        std::cerr << "Index range invalid" << std::endl;
        return;
    }
    std::map<UInt_t, Double_t> mmap;
    for (UInt_t i=0; i<fData->at(index).size(); i++)
    {
        mmap.insert(std::make_pair(i,fData->at(index).at(i)));
    }

    // taken from http://www.gamedev.net/topic/275617-stl-stdmap-and-sorting-using-value_compare/
    typedef std::pair<UInt_t, Double_t> data_t;
    typedef std::priority_queue<data_t, std::deque<data_t>, greater_second<data_t> > queue_t;

    queue_t q(mmap.begin(), mmap.end());
    data_t d;

    fSortedData->resize(fData->size());
    for (UInt_t i=0; i<fData->size(); i++)
    {
        fSortedData->at(i).resize(q.size());
    }

    UInt_t qsize = q.size();
    for (UInt_t i=0; i<qsize; i++)
    {
        d = q.top();

        for (UInt_t j=0; j<fData->size(); j++)
        {
            fSortedData->at(j).at(i) = fData->at(j).at(d.first);
        }
        q.pop();
    }
}
