// DeleteHist.C
//
// Jeromy Tompkins 
// 8/8/2011
//
// Deletes a histogram with name

#include <cstdlib>
#include "TDirectory.h"
using namespace std;

void
DeleteHist(TDirectory *dir, const Char_t* format, Int_t num1, Int_t num2)
{
  dir->Delete(Form(format,num1,num2));
}
