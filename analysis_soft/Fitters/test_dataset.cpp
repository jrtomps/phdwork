#include <fstream>
#include <iostream>
#include <algorithm>
#include "Parameters.h"
#include "DataSetBuilder.h"
#include "Fit/DataRange.h"
#include "TFile.h"
#include "TH2.h"
#include "TGraph2DErrors.h"
#include "AngDist.h"

using namespace std;

Int_t main()
{

    std::ifstream parfile("params.dat",std::ios::in);
    Parameters p(parfile);

    if (p.size()!=4) return -10;
    AngDist W;
    W.SetParameters(p.find(0)->second->GetValue(),
                    p.find(1)->second->GetValue(),
                    p.find(2)->second->GetValue(),
                    p.find(3)->second->GetValue());

    DataSetBuilder dsb;
    dsb.SetGenFunction(&W,W.NDim());
    const ROOT::Fit::DataRange* range = W.GetDataRange();
    dsb.SetRange(*range);
    dsb.Init("");
    Int_t ret;
    TGraph2DErrors* gr = dsb.GenerateSmallSampleGraph(100,0.05);
    gr->SetName("mygraph");

    TFile f("myfile.root","UPDATE");
    gr->Write("",TObject::kOverwrite);
    f.Close();

    delete gr;

    return 0;
}
