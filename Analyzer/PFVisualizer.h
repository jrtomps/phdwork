// PFVisualizer.h
//
// Jeromy Tompkins
// 6/15/2011
//

#include <vector>
#include "TString.h"
#include "Visualizer.h"
#include "TObject.h"

#ifndef PFVISUALIZER_H
#define PFVISUALIZER_H 1

class TFile;

class PFVisualizer : public Visualizer 
{
public:
    PFVisualizer  (const Int_t run);
    PFVisualizer  (const Char_t *fname, const Int_t run);
    ~PFVisualizer (void);

    PFVisualizer  (PFVisualizer const& obj);
    PFVisualizer& operator=(PFVisualizer const& obj);

    void Draw      (const TString &basename, TString options="") const;
    void Draw      (const TString &basename, const Int_t low_index,
                    const Int_t hi_index, TString options="") const;

    void OpenFile  (void);
    void CloseFile (void);

protected:
    TString GuessBase();
    std::vector<TString> GetDir(TString& dirname);
    void    SetupFileName();
    void    LoadChannelInfo();
    TString fFileName;
    TFile   *fFile;
    Int_t   fRun;
    std::vector<Bool_t>  fIsGoodCh;


private:
    ClassDef (PFVisualizer,0);
};

#endif
