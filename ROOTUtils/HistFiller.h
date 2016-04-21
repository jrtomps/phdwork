// HistFiller.h
//
// Author : Jeromy Tompkins
// Date   : 4/15/2010
//
// Purpose: A class to manage the filling of histograms
//          during online analysis.

#include "TObject.h"
#include "TGraph.h"
#include "TH1F.h"

#ifndef HistFiller_h
#define HistFiller_h 1

class HistFiller {
public:
  HistFiller() {;};
  HistFiller(const Char_t *fname) {strcpy(fName, fname);};
  void FillGraphsFromFile(TGraph **gr);
  void FillGraphFromFile(TGraph *gr);
  void FillHistFromGraph(TH1F *h, TGraph *gr);
private:
  Char_t fName[100];
  Double_t x;
  Double_t y;
  ClassDef(HistFiller,0);
};
#endif
