// Visualizer.h
//
// Jeromy Tompkins
// 3/9/2011
//
// Base class for all visualizer classes.

#include <iostream>
#include <vector>
#include "TDirectory.h"
#include "TObject.h"
#include "TString.h"
#include "TColor.h"
#include "TCanvas.h"

#ifndef VISUALIZER_H
#define VISUALIZER_H

class TH1;
class TSeqCollection;
class TLine;

class Visualizer : public TObject 
{
public:
    Visualizer();
    Visualizer(const TString& configfile);
    Visualizer(const Visualizer& obj);
    Visualizer& operator=(const Visualizer& obj);
    virtual ~Visualizer();

    virtual void Draw(const TString &basename, TString options="") const;
    virtual void Draw(const TString &basename, const Int_t low_index,
                      const Int_t hi_index, TString options="") const;
protected:
    template<class T>
    void DrawObjects(std::vector<T*>& vec, TString options="") const;

    template<class T>
    void DrawObjects(std::vector<T*>& vec, const Int_t low_index,
                     const Int_t hi_index, TString options="") const;
public:
    virtual void DrawLines(std::vector<TLine*>& vec, TString options="") const;
    virtual void DrawLines(std::vector<TLine*>& vec, const Int_t low_index,
                           const Int_t hi_index, TString options="") const;

    void ForceUpdate();
    //  void ForceUpdate(const TString &canvasname);
    void CloseAll();
    //  void CloseCanvas(const TString &basename);
    void IconifyAll();
    //  void IconifyCanvas(const TString &canvasname);

    TCanvas* GetCanvas(const TString& canvasname) const;
    void SetLogy(const TString& canvasname, const Bool_t turnOn) const;
    void SetAxisLabelSize(TCanvas* canvas, Float_t size, Char_t axis='x') const;
    void SetAxisLabelSize(const TString& canvasname,Float_t size, Char_t axis='x') const;
    void SetAxisLabelSizeOnAllCanvases(Float_t size, Char_t axis='x') const;
    void SaveAllCanvases(const TString& dirfile, Bool_t separateFiles=false) const;
    void SaveAllCanvases(TDirectory *dir) const;
    void SaveCanvas(TCanvas* c, const TString basename) const;
    void ScaleAllHistsInCanvasWithBaseName(const TString& canvas_name,
                                           const TString& basename,
                                           Double_t scale_val) const;
    std::vector<TH1*> GetAllHistsInCanvasWithBaseName(const TString& canvas_name,
                                                      const TString& basename) const;

    Int_t GetNRows(void) const;
    Int_t GetNColumns(void) const;

    void SetNRows(const Int_t nrows);
    void SetNColumns(const Int_t ncolumns);

protected:
    TSeqCollection *fCanvases;  //[1]

    Int_t GuessRunNumberFromName(const TString& dirname) const;
    TString FormRunNumberExtension(const Int_t runnumber) const;
    TString FormHistName(const TString& name, const Int_t hist_index) const;
    virtual Bool_t  SetAndRemoveLogy(TString& options) const;
    virtual TString SetAndRemoveSpecifiedCanvas(TString& options) const;
    virtual Int_t   SetAndRemoveColor(TString& options) const;
    Int_t GetCanvasIndexFromCanvasName(const TString& str) const;
    TString RemoveCanvasIndexFromCanvasName(const TString& str) const;

    Int_t  fNRows;
    Int_t  fNCols;
private:
    void SetParametersFromFile(const TString& fname);


    ClassDef(Visualizer,0);

};

template<class T>
void Visualizer::DrawObjects(std::vector<T *>& vec, TString options) const
{
    TString basename = "object";
    if (vec.size()==0)
    {
        std::cerr << "Visualizer::DrawObject:::> Empty vector argument" << std::endl;
        return;
    }
#ifdef DEBUG_VIS
    else
    {
        std::cout << "Visualizer::DrawObject:::> Passed vector of size=" << vec.size() << std::endl;
        std::cout.flush();
    }
#endif

    TString canname;
    Bool_t drawLogy=0;
    Bool_t specificCanvas=0;
    Int_t  color=1;

    options.Strip();
    options.ToLower();
    options.Append(" H");

    drawLogy = SetAndRemoveLogy(options);
    color    = SetAndRemoveColor(options);

    Int_t hist_index=0, failcount=0, maxfails=2;
    Int_t pad_index=0, canvas_count=0, xoff=20;

    TDirectory *dir = gDirectory;
    TString RunNumberStr = FormRunNumberExtension(GuessRunNumberFromName(dir->GetName()));

    canname  = SetAndRemoveSpecifiedCanvas(options);
    if (!canname.IsNull()) specificCanvas = kTRUE;
    else
    {
        canname = TString::Format("%s%s",basename.Data(),RunNumberStr.Data());
        canname.ReplaceAll("%i","");
    }

    T* h;
    TCanvas *pc=0;
    TString hname;

    for (UInt_t i=0; i<vec.size(); i++)
    {
        h = vec[i];
        if (h!=NULL)
        {
            if (pad_index%(fNRows*fNCols)==0)
            {
                pc = GetCanvas(TString::Format("%s_%i",canname.Data(),canvas_count));
                if (!pc)
                {
                    if (specificCanvas)
                    {
                        std::clog << canname.Data() << " did not exist."
                                << "Creating new canvas: " << canname.Data() << std::endl;
                    }

                    fCanvases->Add(new TCanvas(Form("%s_%i",canname.Data(),canvas_count),
                                               Form("%s_%i",canname.Data(),canvas_count),
                                               10+xoff*canvas_count,
                                               10+xoff*canvas_count,
                                               800,
                                               800));
                    pc = dynamic_cast<TCanvas*>(fCanvases->FindObject(Form("%s_%i",canname.Data(),canvas_count)));
                    pc->Clear();
                    pc->Divide(fNCols,fNRows,0.001,0.001);
                }
                if (drawLogy)
                {
                    SetLogy(pc->GetName(),drawLogy);
                }
                pad_index=0;
                canvas_count++;
            }
            pc->cd(pad_index+1);
#ifdef DEBUG_VIS
            std::cout << "canvas=" << pc->GetMother()->GetName() << " pad=" << pad_index+1 << std::endl;
#endif
            h->SetLineColor(color);
            h->Draw(options.Data());
            pad_index++;
        }
        else
        {
#ifdef DEBUG_VIS
            std::cerr << "Object doesn't exist" << std::endl;
#endif
            failcount++;
        }
        hist_index++;
    }

    if (pad_index==0 && canvas_count==0)
    {
        std::clog << "Found no objects" << std::endl;
    }
}

template<class T>
void Visualizer::DrawObjects(std::vector<T *>& vec, Int_t low_index, Int_t hi_index, TString options) const
{
    if (vec.size()==0)
    {
        std::cerr << "Visualizer::DrawObject:::> Empty vector argument" << std::endl;
        return;
    }

    TString basename("object");
    TString canname;
    Bool_t drawLogy=0;
    Bool_t specificCanvas=0;
    Int_t  color=1;
    options.Strip();
    options.ToLower();
    options.Append(" H");

    drawLogy = SetAndRemoveLogy(options);
    color    = SetAndRemoveColor(options);

    Int_t hist_index=low_index, failcount=0, maxfails=2;
    Int_t pad_index=0, canvas_count=0, xoff=20;

    TDirectory *dir = gDirectory;
    TString RunNumberStr = FormRunNumberExtension(GuessRunNumberFromName(dir->GetName()));

    T *h;
    TCanvas *pc=0;
    TString hname;

    canname  = SetAndRemoveSpecifiedCanvas(options);
    if (!canname.IsNull()) specificCanvas = kTRUE;
    else
    {
        canname = TString::Format("%s%s",basename.Data(),RunNumberStr.Data());
        canname.ReplaceAll("%i","");
    }

    canvas_count = GetCanvasIndexFromCanvasName(canname);
    canname      = RemoveCanvasIndexFromCanvasName(canname);

    for (UInt_t i=low_index; i<=hi_index && i<vec.size(); i++)
    {
        h = vec[i];
        if (h!=NULL)
        {
            if (pad_index%(fNRows*fNCols)==0)
            {
                pc = GetCanvas(TString::Format("%s_%i",canname.Data(),canvas_count));
                if (!pc)
                {
                    if (specificCanvas)
                    {
                        std::clog << canname.Data() << " did not exist."
                                << "Creating new canvas: " << canname.Data() << std::endl;
                    }

                    fCanvases->Add(new TCanvas(Form("%s_%i",canname.Data(),canvas_count),
                                               Form("%s_%i",canname.Data(),canvas_count),
                                               10+xoff*canvas_count,
                                               10+xoff*canvas_count,
                                               800,
                                               800));
                    pc = dynamic_cast<TCanvas*>(fCanvases->FindObject(Form("%s_%i",canname.Data(),canvas_count)));
                    pc->Clear();
                    pc->Divide(fNCols,fNRows,0.001,0.001);
                }

                if (drawLogy)
                {
                    SetLogy(pc->GetName(),drawLogy);
                }
                pad_index=0;
                canvas_count++;
            }
            pc->cd(pad_index+1);
#ifdef DEBUG_VIS
            std::cout << "canvas=" << pc->GetMother()->GetName() << " pad=" << pad_index+1 << std::endl;
#endif
            h->SetLineColor(color);
            h->Draw(options.Data());
            pad_index++;
        }
        else
        {
#ifdef DEBUG_VIS
            std::cerr << "Object doesn't exist" << std::endl;
#endif
            failcount++;
        }
        hist_index++;
    }

    if (pad_index==0 && canvas_count==0)
    {
        std::clog << "Found no objects" << std::endl;
    }

}

#endif
