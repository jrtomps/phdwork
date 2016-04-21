#include "TGraphErrors.h"
#include <vector>
#include <sstream>
#include "TMath.h"

void ConvertToCos(std::vector<Double_t>& x, std::vector<Double_t>& ex)
{
    for (UInt_t i=0; i<x.size(); i++)
    {
        x[i] = TMath::Cos(x[i]*TMath::DegToRad());
        ex[i] = TMath::Sin(x[i]*TMath::DegToRad())*ex[i]*TMath::DegToRad();
    }
}


TGraphErrors* ConvertToCosineAxis(TGraphErrors* gr)
{
    std::vector<Double_t> xcos(gr->GetX(), gr->GetX()+gr->GetN());
    std::vector<Double_t> excos(gr->GetEX(), gr->GetEX()+gr->GetN());
    ConvertToCos(xcos, excos);
    TGraphErrors *gr_cos = new TGraphErrors(gr->GetN(),
                                            xcos.data(), gr->GetY(),
                                            excos.data(), gr->GetEY());
    std::ostringstream os;
    os << gr->GetName() << "_cos";
    gr_cos->SetName(os.str().data());
    gr_cos->SetTitle(gr->GetTitle());

    return gr_cos;
}
