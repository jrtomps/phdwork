#ifndef VectorSorter_H
#define VectorSorter_H

#include <vector>
#include <utility>
#include <functional>
#include "Rtypes.h"


template<class T>
struct greater_second : std::binary_function<T,T,bool>
{
   inline bool operator()(const T& lhs, const T& rhs)
   {
      return (lhs.second > rhs.second);
   }
};

class VectorSorter
{
public:
    VectorSorter();
    VectorSorter(const VectorSorter& that);
    VectorSorter& operator=(const VectorSorter& that);

    ~VectorSorter();

    void Clear();

    std::vector<std::vector<Double_t> >::iterator RegisterVector(const std::vector<Double_t>& vec);

    std::vector<Double_t> GetUnsortedVector(UInt_t index);
    std::vector<Double_t> GetSortedVector(UInt_t index);

    void Sort(UInt_t index=0);

private:
    std::vector< std::vector<Double_t> >* fData;
    std::vector< std::vector<Double_t> >* fSortedData;
};

#endif // VectorSorter_H
