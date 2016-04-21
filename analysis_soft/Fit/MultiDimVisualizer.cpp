#include <boost/shared_ptr.hpp>
#include "TObject.h"
#include "TF2.h"
#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TTimer.h"
#include "TSystem.h"
#include "MultiDimVisualizer.h"

MultiDimVisualizer::MultiDimVisualizer(TF2& f, TVirtualPad& pad)
    : TObject(),
    fFunc(new TF2),
    fTimer(new TTimer(this,300,true)),
    fMother(0),
    fPad(&pad),
    fParIndex(0),
    fParValue(0),
    fParLow(0),
    fParHigh(0),
    fDrawOption("surf")
{
    fMother = fPad->GetCanvas();
    *fFunc = f;
}

MultiDimVisualizer::MultiDimVisualizer(const MultiDimVisualizer &obj)
    : TObject(obj),
    fFunc(new TF2),
    fTimer(),
    fMother(obj.fMother),
    fPad(obj.fPad),
    fParIndex(obj.fParIndex),
    fParValue(obj.fParValue),
    fParStep(obj.fParStep),
    fParLow(obj.fParLow),
    fParHigh(obj.fParHigh),
    fDrawOption(obj.fDrawOption)
{
    *fFunc = *obj.fFunc;
    fTimer.reset(dynamic_cast<TTimer*>(obj.fTimer->Clone()));
}

MultiDimVisualizer& MultiDimVisualizer::operator=(MultiDimVisualizer obj)
{
    swap(*this,obj);
    return *this;
}

MultiDimVisualizer::~MultiDimVisualizer() throw()
{}

void MultiDimVisualizer::SetupParameter(UInt_t index, Double_t par_step, Double_t par_low, Double_t par_high)
{
    fParIndex = index;
    fParStep  = par_step;
    fParValue = par_low;
    fParLow   = par_low;
    fParHigh  = par_high;
    fFunc->SetParameter(fParIndex,fParValue);
}

Bool_t MultiDimVisualizer::HandleTimer(TTimer* tmr)
{
//    std::cout << "Handling timer par=" << fParValue << std::endl;
    UpdateValue();
    UpdateCanvas();
    return true;
}

void MultiDimVisualizer::UpdateValue()
{
    fParValue += fParStep;
    if (fParValue>fParHigh)
    {
        fParValue = fParLow;
    }
    fFunc->SetParameter(fParIndex,fParValue);
}

void MultiDimVisualizer::UpdateCanvas()
{
    fPad->cd();
    fFunc->Draw(fDrawOption.data());
    fMother->Update();
}

void MultiDimVisualizer::TurnOn()
{
    fTimer->TurnOn();
}

void MultiDimVisualizer::TurnOff()
{
    fTimer->TurnOff();
}

TTimer& MultiDimVisualizer::GetTimer()
{
    return *fTimer;
}

void MultiDimVisualizer::SetDrawOption(std::string option)
{
    fDrawOption = option;
}
