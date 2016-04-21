#include "TGraphErrors.h"


TGraphErrors* TrimTheZeroes(TGraphErrors* gr)
{
   TGraphErrors *grn = new TGraphErrors(gr->GetN());

   Double_t x,y, ex, ey;
   Int_t npoints=0;
   for (Int_t i=0; i<gr->GetN(); i++)
   {
       gr->GetPoint(i,x,y);
       if (y>=1.0e-10)
       {
           grn->SetPoint(npoints,x,y);

           ex = gr->GetErrorX(i);
           ey = gr->GetErrorY(i);

           grn->SetPointError(npoints,ex,ey);

           npoints++;
       }
   }

   grn->Set(npoints);

   return grn;
}
