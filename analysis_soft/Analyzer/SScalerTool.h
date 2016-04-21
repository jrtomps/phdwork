// SScalerTool.h
//
// Author : Jeromy Tompkins
// Date   : 8/6/2010
//
// Descrip: A fancier Scaler tool that adds names for channels
//

#include <string>
#include <vector>
#include "ScalerTool.h"
#include "TObject.h"

#ifndef SSCALERTOOL_H
#define SSCALERTOOL_H

class SScalerTool : public ScalerTool 
{
 public:
  SScalerTool () : ScalerTool () { fNamesRead = 0; };
  SScalerTool (std::string filename, std::string configfilename="scaler.config");
  SScalerTool ( const SScalerTool& obj);
  ~SScalerTool (void);
  void ReadScalerDotConfig(void);
  void ReadGenericScalerConfig(std::string filename);
  Int_t GetValueByName(Int_t run, std::string ch_name);
  Int_t SumValueForRunsByName(Int_t run_first, Int_t run_last, std::string ch_name);
  Int_t FindChannel(std::string ch_name);
  void PrintChannels(void) const;
 private:
  Bool_t fNamesRead;
  std::vector<std::string> fChNames;
  std::string tmp;

  ClassDef(SScalerTool,0);
};
#endif
