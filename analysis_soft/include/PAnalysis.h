// PAnalysis.h
//
// Author : Jeromy Tompkins
// Date   : 12/15/2010
//
// Descrip: A namespace for post analysis of the data.

#include <map>
#include <vector>
#include "TFile.h"
#include "TString.h"

class TH1;

namespace PAnalysis 
{

    //  void ParseFileName(const Char_t *fname, std::vector<Int_t>& run, std::vector<Int_t>& skptwice);
    void CombineRuns(const Char_t *filename);
    void IntegrateSiStrips(TString fname, TString hist_prefix="adc", TString hist_suffix="", TString outfname="", TString dname="");
    void IntegrateSiStrips(const Char_t *fname, Int_t binlo, Int_t binhi=-1, const Char_t* hist_prefix="adc", const Char_t* hist_suffix="");
    void IntegrateSiStrips(const Char_t *fname, const Char_t *boundsfile, const Char_t* hist_prefix="adc", const Char_t* hist_suffix="");
    void IntegrateCombineSiStrips(const Char_t *fname, const Char_t *boundsfile, const Char_t* hist_prefix="adc", const Char_t* hist_suffix="");
    void IntegrateAndExtrapolateSiStrips(const Char_t *fname, const Char_t *boundsfile);

    void CreateRateHists(const Char_t *fname, Float_t seconds);
    void CreateExtrapolatedRateHists(const Char_t *fname, Float_t seconds);

    int GuessRunNumberFromFileName(const Char_t* filename);

    std::vector<TH1*> GetAllTH1(TFile& rootfile);

};


