// STDatime.h
//
// Author : Jeromy Tompkins
// Date   : 12/17/2010
//
// Descrip: TDatime extended to included ability to subtract.

#include "TObject.h"
#include "TDatime.h"

#ifndef STDATIME_H
#define STDATIME_H 1

class STDatime : public TDatime 
{
 public:
  STDatime();
  STDatime(const STDatime& d);
  STDatime(const char* sqlDateTime);
  STDatime(UInt_t tloc, Bool_t dosDate = kFALSE);
  STDatime(Int_t date, Int_t time);
  STDatime(Int_t year, Int_t month, Int_t day, Int_t hour, Int_t min, Int_t sec);
  
  UInt_t  GetTimeInSeconds();
  Float_t GetTimeInMinutes();

  void Subtract(TDatime& d);
  
  ClassDef(STDatime,0);

};
#endif
