

#include <iostream>
#include <iomanip>
#include <string>
#include "TFile.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TGraph2DErrors.h"
#include "GenChiSq.h"
#include "Plane.h"
#include "TF2.h"
#include "ReducedDimFunctionController.h"
#include "TVirtualPad.h"
#include "MultiDimVisualizer.h"
#include "TTimer.h"
#include "TSystem.h"
#include "Math/IFunction.h"

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

Int_t Main(int argc=0, char* argv[]=0)
{
#ifndef __CINT__
    TApplication app("app",&argc, argv);
#endif

    TGraph2DErrors* mygr =GetGraph("myfile.root");

    std::vector<Double_t> pars(4,1);
    Plane p(pars);
    GenChiSq fcn(mygr,p);

    ReducedDimFunctionController rdf(fcn,2);
    Double_t ivals[4] = {0.5,0.5,0.5,0.5};
    rdf.SetInternalValues(ivals,ivals+4);

    std::map<UInt_t,UInt_t> ind_map;
    ind_map[0] = 1;
    ind_map[1] = 2;
    rdf.SetIndexMap(ind_map);
    std::cout << rdf << std::endl;

    ROOT::Math::ParamFunctor pfunc;
    pfunc.SetFunction(&rdf);

    TF2 f_0("f_0",pfunc,
            0,3.0,
            0,3.0,
            4);
    f_0.SetParameters(ivals);

    TCanvas *my_vis_can = new TCanvas("my_vis_can","",1000,800);
    my_vis_can->SetLogz(true);

    TVirtualPad* pad = my_vis_can->cd();
    MultiDimVisualizer mdv0(f_0,*pad);
    mdv0.SetupParameter(0,0.2,0,5.0);
    mdv0.GetTimer().SetTime(300);
    mdv0.TurnOn();

    f_0.Draw("surf0");

#ifndef __CINT__
    app.Run();
#endif

    return 0;
}

#ifndef __CINT__
Int_t main()
{
    return Main();
}
#endif
