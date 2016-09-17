// ScalerTool.h
//
// Written by: Jeromy Tompkins
// Date: 3/18/09
// Purpose:
//      This class creates a tool object that can perform operations on 
//      a scaler output file from CODA
//
//---------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "TObject.h"

#ifndef SCALERTOOL_H
#define SCALERTOOL_H 1

class ScalerTool;

class ScalerTool : public TObject
{
  
 public:
  // Constructor
  ScalerTool ();
  ScalerTool (std::string scaler_filename);
  // Copy Constructor
  ScalerTool(ScalerTool const& obj);
  ScalerTool& operator=(ScalerTool const& obj);
  
  // Destructor
  ~ScalerTool ();

  // Print a data from a run
  void  FindRun (Int_t runnum);
  Int_t GetRun();
  Int_t GetValueByIndex (Int_t run_num, UInt_t index);
  Int_t SumValueForRunsByIndex (Int_t run_first, Int_t run_last, UInt_t index);
  void  LoadRun(Int_t runnum);
  void  LoadRunIntoBuffer();
  Int_t ParseRunNumberFromString(std::string str);
  std::string  SkipToNextRun(void);
  
 protected:
  Bool_t isOkayToProceed(void) const;
 
  std::vector<Int_t>    sclrvals;
  std::ifstream       scalerfile;
  Bool_t               runloaded;
  Int_t                       ch;
  Int_t                      run;

  ClassDef (ScalerTool, 1);

};

#endif
