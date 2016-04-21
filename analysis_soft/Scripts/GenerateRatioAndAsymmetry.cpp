#include <iostream>
#include <sstream>
#include <stdexcept>
#include "TFile.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TLegend.h"
#include "TObject.h"
#include "TDirectory.h"

class FuncWithErr
{
public:
    virtual Double_t Eval(Double_t, Double_t) const {std::cout<<"Base"<< std::endl; return 0;}
    virtual Double_t EvalErr(Double_t, Double_t, Double_t, Double_t) const {return 0;}
};

class Ratio : public FuncWithErr
{
public:
    Ratio() : FuncWithErr() {};
    Ratio(const Ratio& that) : FuncWithErr(that) {};
    Ratio& operator=(const Ratio& that)
                    {
        if (this!=&that)
        {
            FuncWithErr::operator=(that);
        }
        return *this;
    }

    Double_t Eval(Double_t x, Double_t y) const
    {
        if (y!=0)
            return x/y;
        else
            return 0;
    }

    Double_t EvalErr(Double_t x, Double_t y, Double_t ex, Double_t ey) const
    {
        using TMath::Sqrt;
        using TMath::Power;

        if (x!=0 && y!=0)
        {
            Double_t val = Eval(x,y);
            return val*Sqrt(Power(ex/x,2.0) + Power(ey/y,2.0));
        }
        else
            return 0;
    }
};

class Asymmetry : public FuncWithErr
{
public:
    Asymmetry() : FuncWithErr() {};
    Asymmetry(const Asymmetry& that) : FuncWithErr(that) {};
    Asymmetry& operator=(const Asymmetry& that)
                    {
        if (this!=&that)
        {
            FuncWithErr::operator=(that);
        }
        return *this;
    }

    Double_t Eval(Double_t x, Double_t y) const
    {
        if ((x+y)!=0)
            return (x-y)/(x+y);
        else
            return 0;
    }

    Double_t EvalErr(Double_t x, Double_t y, Double_t ex, Double_t ey) const
    {
        using TMath::Sqrt;
        using TMath::Power;

        if (x!=0 && y!=0 && (x+y)!=0)
        {
            return 2.0/Power(x+y,2.0)*Sqrt(x*x*ey*ey+y*y*ex*ex);
        }
        else
            return 0;
    }
};

class Functor
{
private:
    Functor();
    Functor& operator=(Functor const&) {return *this;};

public:
    Functor(FuncWithErr* func)
        : fFunc(func)
    {}

    ~Functor() { delete fFunc;}

    TGraphErrors* operator()(const TGraphErrors* gr0, const TGraphErrors* gr1)
    {
        Double_t tolerance = 2.0;
        using TMath::Abs;
        TGraphErrors *gret = 0;
        if (gr0==NULL || gr1==NULL) throw std::runtime_error("null graph pointer");
//        else if (gr0->GetN() != gr1->GetN()) throw std::runtime_error("mismatched number of points");
        else
        {
            gret = new TGraphErrors(TMath::Min(gr0->GetN(), gr1->GetN()));

            UInt_t n=0;
            Double_t x0, x1, ex0;
            Double_t y0, y1, ey0, ey1;

            UInt_t prev=0;
            for (Int_t i=0; i<gr0->GetN(); i++)
            {
                gr0->GetPoint(i,x0,y0);
                ey0 = gr0->GetErrorY(i);
                UInt_t count=0;
                for (UInt_t j=0; j<gr1->GetN(); j++)
                {
                    gr1->GetPoint(j,x1,y1);
                    ey1 = gr1->GetErrorY(j);

                    if (Abs(x1-x0) < tolerance) break;

                    count++;
                }


                if (i!=0 && prev==count && count!=gr0->GetN())
                    std::cout << "\t\tWarning! Found the same theta : gr0x=" << x0 << " gr1x=" << x1 << std::endl;

                if (count!=gr1->GetN())
                {
                    ex0 = gr0->GetErrorX(i);
                    gret->SetPoint(n,x0,fFunc->Eval(y0,y1));
                    gret->SetPointError(n,ex0,fFunc->EvalErr(y0,y1,ey0,ey1));
                    n++;
                }
                prev = count;

            }
            gret->Set(n);

        }


        return gret;
    }

private:
    const FuncWithErr* fFunc;
};

void ConvertToCos(std::vector<Double_t>& x, std::vector<Double_t>& ex)
{
    for (UInt_t i=0; i<x.size(); i++)
    {
        x[i] = TMath::Cos(x[i]*TMath::DegToRad());
        ex[i] = TMath::Sin(x[i]*TMath::DegToRad())*ex[i]*TMath::DegToRad();
    }
}

void GenerateRatioAndAsymmetryForDirectory(TDirectory* dir)
{

    TObject *obj;
    const TGraphErrors *gr_para = 0, *gr_perp = 0;
    std::ostringstream name0;
    name0 << dir->GetName() << "_phi0_yield";
    obj = dir->Get(name0.str().data());
    if (obj!=NULL)
    {
        gr_para = static_cast<TGraphErrors*>(obj);
    }
    else
    {
        std::cout << "\t" << name0.str() << " not found in directory \"" << dir->GetName() << "\"" << std::endl;
        return;
    }

    std::ostringstream name90;
    name90 << dir->GetName() << "_phi90_yield";
    obj = dir->Get(name90.str().data());
    if (obj!=NULL)
    {
        gr_perp = static_cast<TGraphErrors*>(obj);
    }
    else
    {
        std::cout << "\t" << name90.str() << " not found in directory \"" << dir->GetName() << "\"" << std::endl;
        return;
    }
    Functor ratio(new Ratio());
    //    Functor ratio(Ratio());
    //    Asymmetry asym;
    Functor asymm(new Asymmetry());

    TGraphErrors *gr_ratio = ratio(gr_para, gr_perp);
    gr_ratio->SetName("ratio");
    gr_ratio->SetTitle("Ratio");
    gr_ratio->SetMarkerStyle(22);
    gr_ratio->SetMarkerColor(kOrange+2);
    gr_ratio->SetLineColor(kOrange+2);
    gr_ratio->SetFillColor(kOrange+1);

    TGraphErrors *gr_asymm = asymm(gr_para, gr_perp);
    gr_asymm->SetName("asymm");
    gr_asymm->SetTitle("Asymmetry");
    gr_asymm->SetMarkerStyle(22);
    gr_asymm->SetMarkerColor(kOrange+2);
    gr_asymm->SetLineColor(kOrange+2);
    gr_asymm->SetFillColor(kOrange+1);

    std::vector<Double_t> xcos(gr_ratio->GetX(), gr_ratio->GetX()+gr_ratio->GetN());
    std::vector<Double_t> excos(gr_ratio->GetEX(), gr_ratio->GetEX()+gr_ratio->GetN());
    ConvertToCos(xcos, excos);
    TGraphErrors *gr_ratio_cos = new TGraphErrors(gr_ratio->GetN(),
                                                  xcos.data(), gr_ratio->GetY(),
                                                  excos.data(), gr_ratio->GetEY());
    gr_ratio_cos->SetName("ratio_cos");
    gr_ratio_cos->SetTitle("Ratio");

    TGraphErrors *gr_asymm_cos = new TGraphErrors(gr_asymm->GetN(),
                                                  xcos.data(), gr_asymm->GetY(),
                                                  excos.data(), gr_asymm->GetEY());
    gr_asymm_cos->SetName("asymm_cos");
    gr_asymm_cos->SetTitle("Asymmetry");

    TDirectory  *prev_dir = gDirectory;
    dir->cd();
    gr_ratio->Write("",TObject::kOverwrite);
    gr_asymm->Write("",TObject::kOverwrite);
    gr_ratio_cos->Write("",TObject::kOverwrite);
    gr_asymm_cos->Write("",TObject::kOverwrite);
    prev_dir->cd();

    delete gr_ratio;
    delete gr_asymm;
    delete gr_ratio_cos;
    delete gr_asymm_cos;

}




void GenerateRatioAndAsymmetry(TFile* f)
{
    if (f==0) return;
    else if (!f->IsOpen())
    {
        std::cout << f->GetName() << " doesn't exist" << std::endl;
        return;
    }

    TDirectory *dir = f->GetDirectory("uncorrected");
    if (dir!=0)
    {
        std::cout << "\tGenerating ratio and asymmetry for directory \"" << dir->GetName() << "\"" << std::endl;
        GenerateRatioAndAsymmetryForDirectory(dir);
    }
    else
    {
        std::cout << "\tDirectory \"uncorrected\" skipped b/c it doesn't exist" <<  std::endl;
    }

    dir = f->GetDirectory("corrected");
    if (dir!=0)
    {
        std::cout << "\tGenerating ratio and asymmetry for directory \"" << dir->GetName() << "\"" << std::endl;
        GenerateRatioAndAsymmetryForDirectory(dir);
    }
    else
    {
        std::cout << "\tDirectory \"corrected\" skipped b/c it doesn't exist" <<  std::endl;
    }

}
