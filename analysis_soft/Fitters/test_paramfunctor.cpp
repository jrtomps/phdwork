#include "Math/ParamFunctor.h"

#include "TF2.h"


class Poly : public ROOT::Math::ParamFunctionBase
{
public:
	Poly() : ROOT::Math::ParamFunctionBase()
	{}

	Poly(const Poly& obj) : ROOT::Math::ParamFunctionBase(obj) {std::cout << "cpy" << std::endl;}

	virtual Poly* Clone() const
	{
		return new Poly(*this);
	}

	virtual Double_t operator()(Double_t* x, Double_t* p)
	{
		return p[0]+p[1]*x[0]+p[2]*x[0]*x[0] + p[3]*x[1]+p[4]*x[1]*x[1];
	}

};



Int_t Main()
{
	Poly *p = new Poly;

	ROOT::Math::ParamFunctor pf;
	pf.SetFunction(p);

	TF2 *f = new TF2("f",pf,-3,3,-4,4,5);

	Double_t pars[5] = {1,2,3,0.4,2};
	f->SetParameters(pars);

	f->Draw("surf");

	return 0;
}

