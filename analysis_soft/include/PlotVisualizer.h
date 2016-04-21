// PlotVisualizer.h
//
// Author : Jeromy Tompkins
// Date   : 3/22/2010
//
// Purpose: To organize and separate the drawing features 
//          of the plot.C file

#include <map>
#include "TString.h"
#include "TObject.h"
#include "Visualizer.h"

#ifndef PlotVisualizer_h
#define PlotVisualizer_h 1

class TFile;
class TCanvas;
class TH1F;

class PlotVisualizer : public Visualizer {

 public:
  PlotVisualizer();
  PlotVisualizer(const TString &configfile);
  virtual ~PlotVisualizer();
  PlotVisualizer(PlotVisualizer const& obj);
  PlotVisualizer& operator=(PlotVisualizer const& obj);

  virtual void Draw(const TString& basename, TString opt="");

  void DrawAll();
  void DrawTDC(TString opt="");
  void DrawADC(TString opt="");

  void PrintMap(void);

 protected:
  Bool_t IsAValidMapping(void);
  void ResetMap(void);
  void SetDetMapFromFile(const TString &configfile);

  std::map<Int_t,Int_t> fDetMap;

  TObject *optr; //[1]
  TCanvas *cptr;  //[1]
  TH1F *hptr;   //[1]

 private:
  Bool_t fIsValidMap;
  TString fConfigFilename;

  ClassDef(PlotVisualizer,0);
};

#endif
