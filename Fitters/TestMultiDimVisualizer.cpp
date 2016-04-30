#include "TCanvas.h"
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TF2.h"
#include "TSystem.h"
#include "TTimer.h"
#include "Math/WrappedParamFunction.h"
#include "ParamFunctorAdapter.h"
#include "Math/ParamFunctor.h"
#include "MultiDimVisualizer.h"


Double_t func(const Double_t* x, const Double_t* p)
{
    return p[0]*x[0]*x[0]+p[1]*x[1]*x[1]+p[2]*x[0]*x[1];
}
TTimer *timer;
MultiDimVisualizer *mdv;

struct Print : public TObject
{
    Bool_t HandleTimer(TTimer* tmr)
    {
        std::cout << "handling " << std::endl;
        return true;
    }
};

Int_t Main(Int_t argc, Char_t* argv[])
{
#ifndef __CINT__
    TApplication theapp("theapp",&argc, argv);
    theapp.Init();
#endif

    Double_t par[3] = {1,0.3,0.5};
    ROOT::Math::WrappedParamFunction<> *wpf= new ROOT::Math::WrappedParamFunction<>(&func,2,3,par);

    ParamFunctorAdapter pfa(*wpf);

    ROOT::Math::ParamFunctor pfunc;
    pfunc.SetFunction(&pfa);
    TF2* f = new TF2("f",pfunc,-4,4,-4,4,3);
    f->SetParameters(par);

    TCanvas* c = new TCanvas("c");
    TVirtualPad *pad = c->cd();

    mdv = new MultiDimVisualizer(*f,*pad);
    mdv->SetupParameter(2,0.1,-4,4);
    mdv->TurnOn();

#ifndef __CINT__
    theapp.Run();
#endif
    return 0;
}

#ifndef __CINT__
Int_t main(Int_t argc, Char_t* argv[])
{
    return Main(argc, argv);
}

#endif
