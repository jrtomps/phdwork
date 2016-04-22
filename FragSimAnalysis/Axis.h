#ifndef AXIS_H
#define AXIS_H

#include "Rtypes.h"
#include <iterator>

class Axis
{
private:
    Float_t fMin;
    Float_t fMax;
    static UInt_t fNbins;
    Float_t fWidth;

public:

    class _it : public std::iterator<std::input_iterator_tag,UInt_t>
    {
    private:
        UInt_t fI;

    public:
        _it() : fI(0) {}
        _it(const _it& it) : fI(it.fI) {}
        _it& operator=(const _it& it) {fI = it.fI; return *this;}

        _it& operator++() { (fI+1)>fNbins ? fI = fNbins : fI++; return *this;}
        _it operator++(Int_t i) { (fI+i)>fNbins ? fI = fNbins : fI += i ; return *this;}
        Bool_t operator==(const _it& it) const { return (*it == fI);}
        Bool_t operator==(UInt_t i) const { return (i==fI); }
        Bool_t operator!=(const _it& it) const { return (*it != fI); }
        Bool_t operator!=(UInt_t i) const { return (i != fI); }
        const UInt_t& operator*() const {return fI;}
    };

    typedef _it iterator;
    typedef std::reverse_iterator<_it> reverse_iterator;

public:
    Axis(Float_t lowedge, Float_t upedge, UInt_t nbins);

    Float_t GetMin() const;
    Float_t GetMax() const;
    Int_t GetBin(Float_t x) const;
    UInt_t GetNbins() const;
    Float_t GetBinWidth() const;


};

#endif // AXIS_H
