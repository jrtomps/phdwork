// STDatime.cpp
//
// Author : Jeromy Tompkins
// Date   : 12/17/2010
//
// Descrip: TDatime extended to included ability to subtract.

#include <iostream>
#include "TObject.h"
#include "STDatime.h"

ClassImp(STDatime)

STDatime::STDatime() : TDatime() {}
STDatime::STDatime(const STDatime& d) : TDatime(d) {}
STDatime::STDatime(const char* sqlDateTime) : TDatime(sqlDateTime) {}
STDatime::STDatime(UInt_t tloc, Bool_t dosDate) : TDatime(tloc, dosDate) {}
STDatime::STDatime(Int_t date, Int_t time) : TDatime(date, time) {}
STDatime::STDatime(Int_t year, Int_t month, Int_t day, Int_t hour, Int_t min, Int_t sec) 
  : TDatime(year, month, day, hour, min, sec) {}

UInt_t
STDatime::GetTimeInSeconds()
{
  if (GetMonth() > 1)
    {
      std::cout << "Time difference is too large to calculate" << std::endl;
      return 0;
    }
  
  return 86400*(GetDay()-1) + 3600*GetHour() + 60*GetMinute() + GetSecond();

}

Float_t
STDatime::GetTimeInMinutes()
{
  if (GetMonth() > 1)
    {
      std::cout << "Time difference is too large to calculate" << std::endl;
      return 0;
    }
  
  return 1440.*(GetDay()-1) + 60.*GetHour() + GetMinute() + GetSecond()/60.;  
}

void
STDatime::Subtract(TDatime& d)
{
  UInt_t at = d.Convert();
  UInt_t con = Convert(fDatime);
  if (con >= at)
    Set(con-at);
  else
    {
      std::cout << "Error: negative time" << std::endl;
      return;
    }
}
