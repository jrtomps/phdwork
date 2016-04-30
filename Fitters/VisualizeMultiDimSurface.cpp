#include "TTimer.h"
#include "TF1.h"
#include "TF2.h"
#include "TH2.h"
#include "TH3.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TSystem.h"
#include "TMath.h"

Double_t func(const Double_t* x, const Double_t* p)
{
    return p[0]+p[1]*x[0]+p[2]*TMath::Sin(x[0]/10.0*2.0*TMath::Pi());
}

Double_t func2(const Double_t* x, const Double_t* p)
{
    return p[0]+p[1]*x[0]+p[2]*TMath::Sin(x[0]/10.0*2.0*TMath::Pi())*x[1]*x[1];
}


struct TF1Animator : public TObject
{
    TF1Animator(TF1 *f, TCanvas* mother)
        : fFunc(f),
        fMother(mother),
        fParIndex(0),
        fParValue(0),
        fParLow(0),
        fParHigh(0)
    {
//        fFunc = dynamic_cast<TF1*>(f->Clone("fFunc"));
        if (fFunc==0) throw "null function";
    }

    ~TF1Animator()
    {
        delete fFunc;
    }

    void SetupParameter(UInt_t index, Double_t par_step, Double_t par_low, Double_t par_high)
    {
        fParIndex = index;
        fParStep  = par_step;
        fParValue = par_low;
        fParLow   = par_low;
        fParHigh  = par_high;
    }

    virtual Bool_t HandleTimer(TTimer* tmr)
    {
        UpdateValue();
        UpdateCanvas();
        return true;
    }

    void UpdateValue()
    {
        fParValue += fParStep;
        if (fParValue>fParHigh)
        {
            fParValue = fParLow;
        }
        fFunc->SetParameter(fParIndex,fParValue);
    }

    void UpdateCanvas()
    {
//        fFunc->Draw("L");
        fFunc->Draw("surf");
        fMother->Update();
    }


private:
    TF1* fFunc;
    TCanvas* fMother;
    UInt_t fParIndex;
    Double_t fParValue;
    Double_t fParStep;
    Double_t fParLow;
    Double_t fParHigh;

};

TTimer *timer;
TF1Animator *tf1a;
TF1 *f;

Int_t Display()
{
    TCanvas* c_my = new TCanvas("c_my");
    TH3* hbg = new TH3I("hbg",";x;y;z",20,-10,10,20,-10,10,2,-100,100);
//    hbg->SetAxisRange(-100,100,"Z");
    hbg->Draw();
    //    f = new TF1("f",&::func,-10,10,3);
    f  = new TF2("f",&::func2,-10,10,-10,10, 3);
    f->SetParameter(0,2.0);
    f->SetParameter(1,1.0);
    f->SetParameter(2,1.0);

    tf1a = new TF1Animator(f,c_my);

    tf1a->SetupParameter(2,0.1,-4.0,4.0);

    timer = new TTimer(tf1a,120,false);

    timer->TurnOn();

    std::cout << "stop by run TurnOff() !" << std::endl;

    return 0;
}


void TurnOff()
{
    timer->TurnOff();
    delete tf1a;
}

