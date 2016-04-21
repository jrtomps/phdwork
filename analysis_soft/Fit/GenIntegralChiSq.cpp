#include <vector>
#include <boost/shared_ptr.hpp>
#include "Rtypes.h"
#include "TGraph2DErrors.h"
#include "TMath.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "GenIntegralChiSq.h"
#include "ConverterMultiDim.h"

using ROOT::Math::IParametricFunctionMultiDim;

GenIntegralChiSq::GenIntegralChiSq(TGraph2DErrors *gr,
                                   const IParametricFunctionMultiDim &func)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(dynamic_cast<IParametricFunctionMultiDim*>(func.Clone())),
    fConversion(),
    fIntegrator(new ROOT::Math::IntegratorMultiDim()),
    fX(),
    fY(),
    fZ(),
    fEX(),
    fEY(),
    fEZ()
{
    fIntegrator->SetFunction(*fFunc);
    CopyDataFromGraph(gr);
    RemoveZeroesFromData();
}

GenIntegralChiSq::GenIntegralChiSq(const GenIntegralChiSq &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFunc(dynamic_cast<IParametricFunctionMultiDim*>(obj.fFunc->Clone())),
    fConversion(obj.fConversion),
    fIntegrator(obj.fIntegrator), // this now shares the pointer with obj
    fX(obj.fX),
    fY(obj.fY),
    fZ(obj.fZ),
    fEX(obj.fEX),
    fEY(obj.fEY),
    fEZ(obj.fEZ)
{}

GenIntegralChiSq& GenIntegralChiSq::operator=(GenIntegralChiSq obj)
{
    swap(*this, obj);
    return *this;
}

GenIntegralChiSq::~GenIntegralChiSq()
{}

void GenIntegralChiSq::SetConversion(const ROOT::Math::IBaseFunctionOneDim &f)
{
    fConversion.reset(new ConverterMultiDim(NDim(),std::shared_ptr<ROOT::Math::IBaseFunctionOneDim>(f.Clone())));
}

void GenIntegralChiSq::SetConversion(const std::shared_ptr<ConverterMultiDim> &f)
{
    if (f->size() == fFunc->NDim())
        fConversion = f;
    else
    {
        std::cerr << "Not enough Conversion functions for the number of dimensions"
                << "\nConversion will not be stored" << std::endl;
    }
}

void GenIntegralChiSq::ConvertData(const std::shared_ptr<ConverterMultiDim> &f)
{
    // Assumes that the conversions are simple scalings
    // and thus the errors can be converted in the same manner
    if (f->size() == fFunc->NDim())
    {
        ROOT::Math::IBaseFunctionOneDim& xconv = *(*f)[0];
        ROOT::Math::IBaseFunctionOneDim& yconv = *(*f)[1];

        for (Int_t i=0; i<fZ.size(); i++)
        {
            std::cout << "\nx["<<i<< "]=" << fX[i] << "\ty[" <<i << "]=" << fY[i];
            fX[i] = xconv(fX[i]);
            fY[i] = yconv(fY[i]);

            std::cout << "--> x["<<i<< "]=" << fX[i] << "\ty[" <<i << "]=" << fY[i];

            fEX[i] = xconv(fEX[i]);
            fEY[i] = yconv(fEY[i]);
        }
    }
    else
    {
        std::cerr << "Not enough Conversion functions for the number of dimensions"
                << "\nData will not be converted!"<< std::endl;
    }


}

GenIntegralChiSq* GenIntegralChiSq::Clone() const
{
    return new GenIntegralChiSq(*this);
}

UInt_t GenIntegralChiSq::NDim() const
{
    return 4;
}

std::shared_ptr<ROOT::Math::IntegratorMultiDim>& GenIntegralChiSq::GetIntegrator()
{
    return fIntegrator;
}

void GenIntegralChiSq::SetIntegrator(const std::shared_ptr<ROOT::Math::IntegratorMultiDim> &integrator)
{
    fIntegrator = integrator;
}


Double_t GenIntegralChiSq::DoEval(const double *pars) const
{
    fFunc->SetParameters(pars);
    Double_t sq_sum = 0;

    Double_t v = 0;
    Double_t IntLow[2];
    Double_t IntHigh[2];

    for (Int_t i=0; i<fZ.size(); i++)
    {
        IntLow[0] = (fX[i]-fEX[i]);
        IntLow[1] = (fY[i]-fEY[i]);

        IntHigh[0] = (fX[i]+fEX[i]);
        IntHigh[1] = (fY[i]+fEY[i]);

        if (fConversion.use_count()!=0)
        {
            IntLow[0] = (*(*fConversion)[0])(IntLow[0]);
            IntLow[1] = (*(*fConversion)[1])(IntLow[1]);
            IntHigh[0] = (*(*fConversion)[0])(IntHigh[0]);
            IntHigh[1] = (*(*fConversion)[1])(IntHigh[1]);
        }

        v = fIntegrator->Integral(IntLow, IntHigh);

        sq_sum += TMath::Power((fZ[i]-v)/fEZ[i],2.0);
    }

    return sq_sum/(fZ.size()-1);
}

void GenIntegralChiSq::CopyDataFromGraph(TGraph2DErrors* gr)
{
    if (gr==0)
    {
        std::cerr << "FCN_ChiSq::CopyDataFromGraph " << "NULL graph argument" << std::endl;
        throw 2002;
    }
    //assert(gr);

    fX.resize(gr->GetN());
    fY.resize(gr->GetN());
    fZ.resize(gr->GetN());

    fEX.resize(gr->GetN());
    fEY.resize(gr->GetN());
    fEZ.resize(gr->GetN());

    fX.assign(gr->GetX(),gr->GetX()+gr->GetN());
    fY.assign(gr->GetY(),gr->GetY()+gr->GetN());
    fZ.assign(gr->GetZ(),gr->GetZ()+gr->GetN());

    fEX.assign(gr->GetEX(),gr->GetEX()+gr->GetN());
    fEY.assign(gr->GetEY(),gr->GetEY()+gr->GetN());
    fEZ.assign(gr->GetEZ(),gr->GetEZ()+gr->GetN());
}

void GenIntegralChiSq::RemoveZeroesFromData(void)
{

    UInt_t or_size = fZ.size();

    for (UInt_t i=0; i<fZ.size(); i++)
    {
        if (fZ[i] == 0)
        {
            fX.erase(fX.begin()+i);
            fY.erase(fY.begin()+i);
            fZ.erase(fZ.begin()+i);

            fEX.erase(fEX.begin()+i);
            fEY.erase(fEY.begin()+i);
            fEZ.erase(fEZ.begin()+i);
            i--;
        }
    }

    if ( (or_size-fZ.size())!=0 )
    {
        std::cout << "Removed "
                << or_size - fZ.size()
                << " elements with value zero" << std::endl;
    }

}
