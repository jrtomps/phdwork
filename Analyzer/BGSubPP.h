// BGSubPP.h
//
// Author : Jeromy Tompkins
// Date   : 11/13/2010
//
// Purpose: An extension of the ProofPlot class that substracts the bg from the en spectrum.

#include "TObject.h"
#include "TObjArray.h"
#include "ProofPlot.h"
#include "ConfigEntry.h"

#ifndef BGSubPP_h
#define BGSubPP_h 1

class BGSubPP : public ProofPlot {
 public:
  BGSubPP (ConfigEntry* ce, Double_t intx1, Double_t intx2, bool save=true);
  BGSubPP (BGSubPP const& obj);
  virtual ~BGSubPP (void);
  Double_t GetIntX1(void);
  Double_t GetIntX2(void);
  void SetIntX1 (Double_t x);
  void SetIntX2 (Double_t x);
  void SubtractEnBG (void);
  virtual void Terminate (void);

 private:
  Double_t fIntx1;
  Double_t fIntx2;
  TObjArray arr_tof;
  TObjArray arr_necut;

  ClassDef(BGSubPP,0);
};
#endif
