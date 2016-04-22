// TSelectorInsert.h
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <ostream>
#include <iostream>
#include "TTreeProxy.h"

#ifndef TSELECTORINSERT_HH
#define TSELECTORINSERT_HH 1

class TTree;

class TSelectorInsert
{
 public:
  TSelectorInsert(void) {};
  virtual ~TSelectorInsert(void) {};
  
  virtual Bool_t Process(Long64_t entry) = 0;
  virtual void   Save(void) = 0;
  virtual void   PrintResults(std::ostream& stream = std::cout) = 0;

  virtual void   GetValuesFromProxy(TTreeProxy* proxy) = 0;

  ClassDef(TSelectorInsert,0);
};

#endif
