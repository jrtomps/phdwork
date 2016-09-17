
#include "TH1.h"
#include "TH2.h"
#include "Utils.h"

void
SafeDeleteTH1(TH1* hist) 
{
  if (hist!=0) delete hist; 
  hist=0;
}

void
SafeDeleteTH2(TH2* hist) 
{
  if (hist!=0) delete hist; 
  hist=0;
}
