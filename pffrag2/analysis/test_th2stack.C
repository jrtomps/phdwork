#include "TH2.h"
#include "TRandom.h"
#include "THStack.h"

void FillUp(TH2* h, UInt_t n)
{
	TRandom r;
	Double_t x , y;
	for (UInt_t i=0; i<n; i++)
	{
		x = r.Gaus(2,1);
		y = r.Uniform(0,4);
		h->Fill(x,y);
	}
}


void MyStack()
{
	TH2D* h0 = new TH2D("h0","",16,0,4,16,0,4);
	h0->SetFillColor(kRed);
	TH2D* h1 = new TH2D("h1","",16,0,4,16,0,4);
	h1->SetFillColor(kBlue);
	TH2D* h2 = new TH2D("h2","",16,0,4,16,0,4);
	h2->SetFillColor(kGreen);

	FillUp(h0, 200);
	FillUp(h1, 1000);
	FillUp(h2, 3000);
	
	THStack *hs = new THStack("hs","stacked histos");
	hs->Add(h2);
	hs->Add(h0);
	hs->Add(h1);

	hs->Draw();
};
