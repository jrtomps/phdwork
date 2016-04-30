

#include <iostream>
#include <iomanip>
#include <string>
#include "TFile.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TGraph2DErrors.h"
#include "GenIntegralChiSq.h"
#include "Plane.h"
#include "ParamFunctorAdapter.h"
#include "TF2.h"
#include "ReducedDimFunction.h"
#include "TVirtualPad.h"
#include "MultiDimVisualizer.h"

TGraph2DErrors* GetGraph(std::string fname)
{
    TGraph2DErrors* gr = NULL;

    std::string fGrName = "mygraph";

    TFile *f = new TFile(fname.data());
    if (!f->IsOpen())
    {
        std::cerr << "Failed to open " << fname << std::endl;
        return gr;
    }

    f->ls();

    std::cout << "Enter name of graph : ";
    std::cin >> fGrName;

    gr = dynamic_cast<TGraph2DErrors*>(f->Get(fGrName.data()));
    if (gr!=NULL)
    {

        TGraph2DErrors* tgr = static_cast<TGraph2DErrors*>( gr->Clone("mygraph_0"));
        gr=tgr;
        gr->SetName("mygraph_0");

        gr->SetDirectory(0);
    }

    return gr;
}

template<class T>
struct Wrapper : public ROOT::Math::ParamFunctionBase
{
    Wrapper(const T& t) : fFunc(t.Clone()) {}

    virtual ~Wrapper() {delete fFunc; }

    Wrapper(const Wrapper& wr) : fFunc(wr.fFunc->Clone()) {}

    virtual Wrapper* Clone() const { return new Wrapper(*this); }

    virtual Double_t operator()(Double_t* x, Double_t*)
    {
        return (*fFunc)(x);
    }

private:
    T* fFunc;
};

int main(int argc, char* argv[])
{
    TApplication app("app",&argc, argv);

//    app.Init();

    gStyle->SetCanvasPreferGL(true);

    TGraph2DErrors* mygr =GetGraph("myfile.root");

    std::vector<Double_t> pars(4,1);
    Plane p(pars);
    GenIntegralChiSq fcn(mygr,p);

    ReducedDimFunction rdf(fcn,2);
    Double_t par[] = {0.2,0.2,0.2,0.2};
    rdf.SetInternalValues(par,par+4);

//    ParamFunctorAdapter pfa(fcn);
    Wrapper<ReducedDimFunction> *pfa = new Wrapper<ReducedDimFunction>(rdf);

    ROOT::Math::ParamFunctor pfunc;
    pfunc.SetFunction(pfa);
    TF2 f_0("f_0",pfunc,0,1.0,0,1.0,0);

//    TF2 f_1("f_1",pfunc,0,1.0,0,1.0,0);

    TCanvas *my_vis_can = new TCanvas("my_vis_can","",1000,800);
 //   my_vis_can->Divide(2,1,0.001,0.001);
    my_vis_can->Draw();

    TVirtualPad* pad = my_vis_can->cd();
//    MultiDimVisualizer mdv0(f_0,*pad);
//    mdv0.SetupParameter(2,0.1,0,2.0);
//    mdv0.TurnOn();
    f_0.Draw("surf");

//    pad = my_vis_can->cd(2);
//    MultiDimVisualizer mdv1(f_1,*pad);
//    mdv1.SetupParameter(3,0.1,0,2.0);
//    mdv1.TurnOn();
//    f_1.Draw("surf2");


//    TPaveText *pt = new TPaveText(0.1,0.1,0.9,0.9);
//    pt->AddText("asdfasdf;asdja;sljf");
//    pt->Draw();
//
    app.InitializeGraphics();
    app.Run();

    return 0;
}
