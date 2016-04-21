
#include <vector>
#include <map>
#include <iostream>
#include "Rtypes.h"
#include "Math/ParamFunctor.h"
#include "Math/WrappedFunction.h"
#include "ReducedDimFunction.h"
#include "TF2.h"

Double_t func(const Double_t* x)
{
    return x[0]*x[0] + 2.0*x[1] + 4.0*x[1]*x[2];
}


// This cannot be plugged directly in the TF2 constructor passes
// by value and not reference. It, therefore, ignores the
// implementation of operator() by virtual methods in subclasses.
// So I need to create this and then create another paramfunctor that
//

template<class T>
struct Wrapper : public ROOT::Math::ParamFunctionBase
{
    Wrapper(const T& t) : ROOT::Math::ParamFunctionBase(), fFunc(t.Clone())
    {
        std::cout << "ctor Wrapper" << std::endl;
    }

    Wrapper(const Wrapper& obj) : ROOT::Math::ParamFunctionBase(obj)
    {
        std::cout << "copy ctor Wrapper" << std::endl;
        fFunc = obj.fFunc->Clone();
    }

    virtual Wrapper* Clone() const
    {
        return new Wrapper(*this);
    }

    ~Wrapper() {delete fFunc;}

    Double_t operator()(Double_t* x, Double_t *)
    {
        return fFunc->operator()(x);
    }

private:
    T* fFunc;
};


Wrapper<ReducedDimFunction> *wrf;

TF2 *f;

Int_t Main()
{
    try
    {
        // This is a ROOT::Math::IBaseFunctionMulitDim type
        ROOT::Math::WrappedMultiFunction<ROOT::Math::FreeMultiFunctionPtr> wmf(&func,3);

        // This is also a ROOT::Math::IBaseFunctionMultiDim type
        ReducedDimFunction rdf(wmf,2);

        std::vector<Double_t> vals(3,0);
        vals[0] = 3.0;
        vals[1] = 0.5;
        vals[2] = 10.0;
        rdf.SetInternalValues(vals);

        std::map<UInt_t,UInt_t> mymap;
        mymap[0] = 1;
        mymap[1] = 2;
        rdf.SetIndexMap(mymap);

        std::cout << rdf << std::endl;
        Double_t xx[2] = {3,3};
        std::cout << "rdf(xx)=" << rdf(xx) << std::endl;


        // This is a ROOT::Math::ParamFunctor type
        wrf = new Wrapper<ReducedDimFunction>(rdf);
        std::cout << "(*wrf)(xx,0)=" << (*wrf)(xx,0) << std::endl;

        ROOT::Math::ParamFunctor parfunc;
        parfunc.SetFunction(wrf);
        f = new TF2("f",parfunc,-10,10,-10,10,0);

        std::cout << f->Eval(2,2) << std::endl;
        std::cout << f->Eval(-5,8) << std::endl;


        f->Draw("surf");
    }
    catch (std::exception& exc)
    { std::cout << "caught exception..." << exc.what() << std::endl; }

    return 0;
}

#ifndef __CINT__
int main()
{
    return Main();
}
#endif
