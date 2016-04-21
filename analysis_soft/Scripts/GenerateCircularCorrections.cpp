// GenerateCircularCorrections.cpp
//
// Jeromy Tompkins
// 11/29/2011
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include "TGraphErrors.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TrimTheZeroes.cpp"
using namespace std;


vector<Double_t> corr0;
vector<Double_t> corr1;

Double_t
ComputeCorrection(Double_t y)
{
  if (y>=1.0e-20)
    return 1.0/y;
  else
    return 0;
}


TGraphErrors*
CreateCorrectionGraphs(TGraphErrors* gr0)
{
  TGraphErrors* gr = new TGraphErrors(gr0->GetN());
  
  transform(gr0->GetY(),
	    gr0->GetY()+gr0->GetN(),
	    gr->GetY(),
	    ComputeCorrection);
 
  copy(gr0->GetX(),
       gr0->GetX()+gr0->GetN(),
       gr->GetX());

  copy(gr0->GetEX(),
       gr0->GetEX()+gr0->GetN(),
       gr->GetEX());

  //  copy(gr0->GetEY(),
  //      gr0->GetEY()+gr0->GetN(),
  //     gr->GetEY());
  fill (gr->GetEY(), gr->GetEY()+gr->GetN(), 0);
 
  return gr;
}


void
PrintResultsToFile(ofstream& ofile, TGraphErrors* gr)
{

  ofile << "Data from " 
	<< gr->GetName() 
	<< endl;

  ofile << setw(10) << "X"
	<< setw(10) << "Y"
	<< setw(10) << "EX"
	<< setw(10) << "EY"
	<< endl;
  ofile << setfill('-') << setw(40) << '-' << endl;
  ofile << setfill(' ');
  
  Double_t *x = gr->GetX();
  Double_t *y = gr->GetY();
  Double_t *ex = gr->GetEX();
  Double_t *ey = gr->GetEY();

  ofile << setprecision(2) << setiosflags( ios::fixed );

  for (Int_t i=0; i<gr->GetN(); i++)
    {
      ofile << setw(10) << x[i]
	    << resetiosflags( ios::fixed )
	    << setw(10) << setiosflags( ios::scientific ) << y[i]
	    << resetiosflags( ios::scientific ) 
	    << setiosflags( ios::fixed )
	    << setw(10) << ex[i]
	    << setw(10) << ey[i]
	    << endl;
      
    }

  ofile << resetiosflags( ios::fixed );

}

void
GenerateCircularCorrections(TDirectory* f, const Char_t* ofname)
{
   if (f==NULL) 
     {
       cout << "null file argument" << endl;
       return;
     }
   else if ( f->InheritsFrom(TFile::Class()) 
	     && ! (static_cast<TFile*>(f))->IsOpen() )
     {
       cout << "File " << f->GetName() << " doesn't exist" << endl;
       return;
     }
 
   TDirectory *dir=0;
   TObject *obj=0;
   TGraphErrors *gr0=0, *gr1=0;
   TGraphErrors *gr_corr0, *gr_corr1;
   TGraphErrors *gr_corr0_trim, *gr_corr1_trim;

   string dirname("uncorrected");
   if ( (obj = f->Get(dirname.data())) )
     {
       
       dir = static_cast<TDirectory*>(obj);
       obj = dir->Get((dirname + "_phi0_yield").data());
       if (obj!=NULL) gr0 = static_cast<TGraphErrors*>(obj);
       obj = dir->Get((dirname + "_phi90_yield").data());
       if (obj!=NULL) gr1 = static_cast<TGraphErrors*>(obj);
       
       gr_corr0 = CreateCorrectionGraphs(gr0);
       gr_corr0->SetName("phi0_circ_corrections");
       gr_corr1 = CreateCorrectionGraphs(gr1);
       gr_corr1->SetName("phi90_circ_corrections");

       gr_corr0_trim = TrimTheZeroes(gr_corr0);
       gr_corr0_trim->SetName("phi0_circ_corr_trimmed");
       gr_corr1_trim = TrimTheZeroes(gr_corr1);
       gr_corr1_trim->SetName("phi1_circ_corr_trimmed");

       ofstream ofile(ofname, ios_base::out);
       PrintResultsToFile(ofile, gr_corr0);
       PrintResultsToFile(ofile, gr_corr1);
       ofile.close();
       
       string rootoutfile(ofname);
       size_t in = rootoutfile.rfind(".");
       rootoutfile.replace(in,rootoutfile.length()-in,".root");

       TFile *rofile = new TFile(rootoutfile.data(),"UPDATE");
       rofile->cd();
       cout << "Writing the following graphs to " << rootoutfile << endl;
       cout << gr0->GetName() << endl;
       gr0->Write("",TObject::kOverwrite);

       cout << gr1->GetName() << endl;
       gr1->Write("",TObject::kOverwrite);

       cout << gr_corr0->GetName() << endl;
       gr_corr0->Write("",TObject::kOverwrite);

       cout << gr_corr1->GetName() << endl;
       gr_corr1->Write("",TObject::kOverwrite);

       cout << gr_corr0_trim->GetName() << endl;
       gr_corr0_trim->Write("",TObject::kOverwrite);

       cout << gr_corr1_trim->GetName() << endl;
       gr_corr1_trim->Write("",TObject::kOverwrite);

       rofile->Close();
     }
   else
     {
       cout << dirname << " doesn't exist!" << endl;
     }
   
}
