#ifndef MULTIDIMVISUALIZER_H
#define MULTIDIMVISUALIZER_H

#include <string>
#include "TObject.h"
#include "TF2.h"
#include "TVirtualPad.h"
#include "TCanvas.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TF2>;
    struct shared_ptr<TTimer>;
}
#endif

class TTimer;

class MultiDimVisualizer : public TObject
{

private:
    std::shared_ptr<TF2> fFunc;
    std::shared_ptr<TTimer> fTimer;
    TCanvas* fMother;
    TVirtualPad* fPad;
    UInt_t fParIndex;
    Double_t fParValue;
    Double_t fParStep;
    Double_t fParLow;
    Double_t fParHigh;
    std::string fDrawOption;

private:
    MultiDimVisualizer();

public:
    MultiDimVisualizer(TF2& f, TVirtualPad& mother);
    MultiDimVisualizer(const MultiDimVisualizer& obj);
    MultiDimVisualizer& operator=(MultiDimVisualizer obj);
    friend void swap(MultiDimVisualizer& lhs, MultiDimVisualizer& rhs) throw();

    virtual ~MultiDimVisualizer() throw();
    void SetupParameter(UInt_t index, Double_t par_step, Double_t par_low, Double_t par_high);
    virtual Bool_t HandleTimer(TTimer* tmr);

    void TurnOn();
    void TurnOff();

    void SetDrawOption(std::string option);

    TTimer& GetTimer();

private:
    void UpdateValue();
    void UpdateCanvas();

};

inline void swap(MultiDimVisualizer &lhs, MultiDimVisualizer &rhs) throw()
{
    lhs.fFunc.swap(rhs.fFunc);
    lhs.fTimer.swap(rhs.fTimer);
    std::swap(lhs.fTimer,rhs.fTimer);
    std::swap(lhs.fPad,rhs.fPad);
    std::swap(lhs.fParValue,rhs.fParValue);
    std::swap(lhs.fParStep,rhs.fParStep);
    std::swap(lhs.fParLow,rhs.fParLow);
    std::swap(lhs.fParHigh,rhs.fParHigh);
    lhs.fDrawOption.swap(rhs.fDrawOption);
}

#endif // MULTIDIMVISUALIZER_H
