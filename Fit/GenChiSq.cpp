#include <vector>
#include "Rtypes.h"
#include "TGraph2DErrors.h"
#include "TMath.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "ConverterMultiDim.h"
#include "GenChiSq.h"

using ROOT::Math::IParametricFunctionMultiDim;

GenChiSq::GenChiSq(TGraph2DErrors *gr,
                   const ROOT::Math::IParametricFunctionMultiDim &func)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(),
    fConversion(),
    fX(),
    fY(),
    fZ(),
    fEX(),
    fEY(),
    fEZ()
{

  fFunc.reset(dynamic_cast<IParametricFunctionMultiDim*>(func.Clone()));
  CopyDataFromGraph(gr);
  RemoveZeroesFromData();
}

GenChiSq::GenChiSq(const GenChiSq &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFunc(dynamic_cast<IParametricFunctionMultiDim*>(obj.fFunc->Clone())),
    fConversion(obj.fConversion),
    fX(obj.fX),
    fY(obj.fY),
    fZ(obj.fZ),
    fEX(obj.fEX),
    fEY(obj.fEY),
    fEZ(obj.fEZ)
{}

GenChiSq& GenChiSq::operator=(GenChiSq obj)
{
    swap(*this, obj);
    return *this;
}

GenChiSq::~GenChiSq()
{}

void GenChiSq::SetConversion(const ROOT::Math::IBaseFunctionOneDim &f)
{
    fConversion.reset(new ConverterMultiDim(NDim(),std::shared_ptr<ROOT::Math::IBaseFunctionOneDim>(f.Clone())));
}

void GenChiSq::SetConversion(const std::shared_ptr<ConverterMultiDim> &f)
{
    if (f->size() == fFunc->NDim())
        fConversion = f;
    else
    {
        std::cerr << "Not enough Conversion functions for the number of dimensions"
                << "\nConversion will not be stored" << std::endl;
    }
}

void GenChiSq::ConvertData(const std::shared_ptr<ConverterMultiDim> &f)
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


GenChiSq* GenChiSq::Clone() const
{
    return new GenChiSq(*this);
}


UInt_t GenChiSq::NDim() const
{
    return fFunc->NPar();
}

Double_t GenChiSq::DoEval(const double *pars) const
{
    fFunc->SetParameters(pars);
    Double_t sq_sum = 0;

    Double_t v = 0;
    Double_t x[2];

    for (Int_t i=0; i<fZ.size(); i++)
    {
        x[0] = fX[i];
        x[1] = fY[i];

        if (fConversion.use_count()!=0)
        {
//            std::cout << "Using a conversion" << std::endl;
            x[0] = (*(*fConversion)[0])(x[0]);
            x[1] = (*(*fConversion)[1])(x[1]);
        }

        v = (*fFunc)(x);

        if (fEZ[i]!=0)
            sq_sum += TMath::Power((fZ[i]-v)/fEZ[i],2.0);

    }

    if (fZ.size()>1)
        return sq_sum/(fZ.size()-1);
    else
        return 0;
}

void GenChiSq::CopyDataFromGraph(TGraph2DErrors* gr)
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

void GenChiSq::RemoveZeroesFromData(void)
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
