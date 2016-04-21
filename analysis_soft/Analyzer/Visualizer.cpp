// Visualizer.cpp
//
// Jeromy Tompkins
// 3/9/2011
//
// Base class for all visualizer classes.

#include <iostream>
#include <fstream>
#include <vector>
#include "TROOT.h"
#include "TString.h"
#include "TRegexp.h"
#include "TSeqCollection.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TObject.h"
#include "TString.h"
#include "TLine.h"
#include "Visualizer.h"

ClassImp(Visualizer);

Visualizer::Visualizer() : TObject()
{
    fCanvases = gROOT->GetListOfCanvases();

    fNRows = 4;
    fNCols = 4;
}

Visualizer::Visualizer(const TString& configfile) : TObject()
{
    fCanvases = gROOT->GetListOfCanvases();

    SetParametersFromFile(configfile);

}

Visualizer::Visualizer(const Visualizer& obj) : TObject()
{
    if (this != &obj)
    {
        fCanvases = obj.fCanvases;
        fNRows = obj.fNRows;
        fNCols = obj.fNCols;
    }
}

Visualizer& Visualizer::operator=(const Visualizer& obj)
                                 {
    if (this != &obj)
    {
        fCanvases = obj.fCanvases;
        fNRows = obj.fNRows;
        fNCols = obj.fNCols;
    }
    return *this;
}

Visualizer::~Visualizer()
{}

void Visualizer::Draw(const TString &basename, TString options) const
{
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

    TH1 *h;
    TCanvas *pc=0;
    TString hname;
    do
    {
        h = 0;
        hname = FormHistName(basename,hist_index);
        h = dynamic_cast<TH1*>(dir->FindObjectAny(hname.Data()));
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
            std::cout << hname.Data() << " doesn't exist" << std::endl;
#endif
            failcount++;
        }
        hist_index++;
    } while (failcount < maxfails);

    if (pad_index==0 && canvas_count==0)
    {
        std::clog << "Found no objects" << std::endl;
    }
}

void Visualizer::Draw(const TString &basename,
                      const Int_t low_index,
                      const Int_t hi_index,
                      TString options) const
{
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

    TH1 *h;
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

    do
    {
        h = 0;
        hname = FormHistName(basename,hist_index);

        h = dynamic_cast<TH1*>(dir->FindObjectAny(hname.Data()));
        if (h!=NULL)
        {
            if (pad_index%(fNRows*fNCols)==0)
	    {

                pc = GetCanvas(TString::Format("%s_%i",canname.Data(),canvas_count));
                if (!pc)
		{
                    if (specificCanvas)
                        std::clog << canname.Data() << " did not exist."
                                << "Creating new canvas: " << canname.Data() << std::endl;

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
            std::cout << hname.Data() << " doesn't exist" << std::endl;
#endif
            failcount++;
        }
        hist_index++;
    } while (failcount < maxfails && hist_index<=hi_index);

    if (pad_index==0 && canvas_count==0)
    {
        std::clog << "Found no objects" << std::endl;
    }

}

void Visualizer::DrawLines(std::vector<TLine *> &vec, TString options) const
{
    DrawObjects(vec, options);
}

void Visualizer::DrawLines(std::vector<TLine *> &vec, Int_t low_index, Int_t hi_index, TString options) const
{
    DrawObjects(vec, low_index, hi_index, options);
}

TString Visualizer::FormRunNumberExtension(const Int_t runnumber) const
{
    return (runnumber!=0) ? TString::Format("_run%i",runnumber) : TString("");
}

Int_t Visualizer::GuessRunNumberFromName(const TString& dirname) const
{
    TRegexp re("[0-9]+.root$");
    Int_t index = dirname.Index(re);
    if (index == -1)
        return 0;
    else
        return TString(dirname(index,dirname.Length()-index-5)).Atoi();
}

TString Visualizer::FormHistName(const TString& name, const Int_t hist_index) const
{
    TString str=name;
    Ssiz_t in = str.Index("%i");
    if (in!=-1)
    {
        str.Replace(in,2,Form("%i",hist_index));
    }
    else
    {
        str.Append(Form("%i",hist_index));
    }
    //  std::cout << name.Data() << std::endl;
    return str;
    
}

TCanvas* Visualizer::GetCanvas(const TString& canvasname) const
{
    TCanvas *can = dynamic_cast<TCanvas*>(fCanvases->FindObject(canvasname.Data()));
#ifdef DEBUG_VIS
    if (!can)
        std::cerr << "No canvas " << canvasname.Data() << std::endl;
    else
        std::cout << "Found canvas " << canvasname.Data() << std::endl;
#endif
    return can;
}



Bool_t Visualizer::SetAndRemoveLogy(TString& options) const
{
    Bool_t drawLogy=kFALSE;
    if (options.Contains("logy"))
    {
        drawLogy=kTRUE;
        Int_t in = options.Index("logy",4,0,TString::kIgnoreCase);
        options = options.Remove(in,4);
    }
#ifdef DEBUG_VIS
    if (drawLogy) std::cout << "Found logy" << std::endl;
    std::cout << options.Data() << std::endl;
#endif
    return drawLogy;
}

TString Visualizer::SetAndRemoveSpecifiedCanvas(TString& options) const
{
    TString canname;
    if (options.Contains("canvas="))
    {
        Int_t in = options.Index("canvas=",0,TString::kIgnoreCase);
        Int_t end = options.Index(" ",in+1,TString::kIgnoreCase);
        if (end==-1) end = options.Length();
        canname = options(in+7,end-in-7);
        options = options.Remove(in,end-in);
    }
#ifdef DEBUG_VIS
    if (!canname.IsNull()) std::cout << "Found " << canname.Data() << std::endl;
    std::cout << options.Data() << std::endl;
#endif
    return canname;
}

Int_t Visualizer::SetAndRemoveColor(TString& options) const
{
    Int_t colorname=1;
    if (options.Contains("color="))
    {
        Int_t in = options.Index("color=",0,TString::kIgnoreCase);
        Int_t end = options.Index(" ",in+1,TString::kIgnoreCase);
        if (end==-1) end = options.Length();
        colorname = TString(options(in+6,end-in-6)).Atoi();
        options = options.Remove(in,end-in);
    }
#ifdef DEBUG_VIS
    if (colorname!=1) std::cout << "Setting color to " << colorname << std::endl;
    std::cout << options.Data() << std::endl;
#endif
    return colorname;

}

void Visualizer::SetParametersFromFile(const TString& filename)
{
    std::ifstream ifile(filename.Data(),std::ios::in);
    if (! ifile.is_open())
    {
        std::cout << filename.Data() << " not found" << std::endl;
        fNRows = 3;
        fNCols = 4;
    }
    else
    {
        ifile.ignore(100000,'\n');
        for (Int_t i=0; i<2; i++)
        {
            ifile >> fNRows >> fNCols;
        }
        if (fNRows>5)
        {
            std::cout << "Warning! fNRows exceeded 5" << std::endl;
            fNRows = 3;
        }
        if (fNCols>5)
        {
            std::cout << "Warning! fNCols exceeded 5" << std::endl;
            fNCols = 4;
        }
    }
    std::cout << "fNRows=" << fNRows << "\tfNCols=" << fNCols << std::endl;

    ifile.close();
}

void Visualizer::SetLogy(const TString& canvasname, const Bool_t turnOn) const
{
    TCanvas *pc = dynamic_cast<TCanvas*>(fCanvases->FindObject(canvasname.Data()));
    if (pc)
    {
        for (Int_t i=0; i<pc->GetListOfPrimitives()->GetEntries(); i++)
        {
            (dynamic_cast<TPad*>(pc->GetListOfPrimitives()->At(i)))->SetLogy(turnOn);
        }
    }
    else
        std::cerr << canvasname.Data() << " doesn't exist" << std::endl;
}
void Visualizer::SetAxisLabelSizeOnAllCanvases(Float_t size, Char_t axis) const
{
    TCanvas* pc=0;
    for (Int_t i=0; i<fCanvases->GetEntries(); i++)
    {
        pc=dynamic_cast<TCanvas*>(fCanvases->At(i));
        if (pc!=0)
        {
            SetAxisLabelSize(pc, size, axis);
        }
    }
}

void Visualizer::SetAxisLabelSize(TCanvas* canvas, Float_t size, Char_t axis) const
{
    TPad *pd;
    TSeqCollection* col;
    TH1* h;
    if (canvas==0) return;

    for (Int_t i=0; i<canvas->GetListOfPrimitives()->GetEntries(); i++)
    {
        pd = dynamic_cast<TPad*>(canvas->GetListOfPrimitives()->At(i));
        if (pd!=NULL)
        {
            col = pd->GetListOfPrimitives();
            if (col->GetEntries()>1)
            {
                if (col->At(1)->InheritsFrom(TH1::Class()))
                {
                    h = static_cast<TH1*>(col->At(1));
                    if (axis=='x' || axis=='X')
                        h->GetXaxis()->SetLabelSize(size);
                    else if (axis=='y' || axis=='Y')
                        h->GetYaxis()->SetLabelSize(size);
                    else
                        h->GetZaxis()->SetLabelSize(size);
                }
            }
            pd->Modified(true);
        }
    }
    canvas->Modified();
    canvas->Update();
}

void Visualizer::SetAxisLabelSize(const TString &canvasname, Float_t size, Char_t axis) const
{
    TCanvas *pc = dynamic_cast<TCanvas*>(fCanvases->FindObject(canvasname.Data()));
    if (pc)
        SetAxisLabelSize(pc,size,axis);
    else
        std::cerr << canvasname.Data() << " doesn't exist" << std::endl;
}

Int_t Visualizer::GetCanvasIndexFromCanvasName(const TString& str) const
{
    TRegexp pre("_[0-9]+$");
    Ssiz_t in = str.Index(pre);
    return TString(str(in+1,str.Length()-in-1)).Atoi();
}

TString Visualizer::RemoveCanvasIndexFromCanvasName(const TString& str) const
{
    TRegexp pre("_[0-9]+$");
    Ssiz_t in = str.Index(pre);
    return TString(str(0,in));
}

void Visualizer::SaveAllCanvases(const TString& dirfile, Bool_t separateFiles) const
{
    TString dirfilecpy=dirfile;
    TCanvas* pc=0;
    TFile *f;

    if (! separateFiles)
    {
        f = new TFile(dirfile.Data(),"UPDATE");
    }

    for (Int_t i=0; i<fCanvases->GetEntries(); i++)
    {
        pc=dynamic_cast<TCanvas*>(fCanvases->At(i));
        if (pc!=NULL)
        {
            if (separateFiles)
            {
                SaveCanvas(pc,TString::Format("%s/%s",dirfilecpy.Data(),pc->GetName()));
            }
            else
            {
                pc->Write("",TObject::kOverwrite);
            }
        }
    }
}

void Visualizer::SaveAllCanvases(TDirectory* f) const
{
    TCanvas* pc=0;

    if (f==NULL)
    {
        std::cerr << "Visualizer::SaveAllCanvases:::> NULL TDirectory argument" << std::endl;
        return;
    }

    f->cd();
    for (Int_t i=0; i<fCanvases->GetEntries(); i++)
    {
        pc=dynamic_cast<TCanvas*>(fCanvases->At(i));
        if (pc!=NULL)
        {
            pc->Write("",TObject::kOverwrite);
        }
    }
}

void Visualizer::SaveCanvas(TCanvas* c, const TString basename) const
{
    if (c==NULL) {std::cout << "Warning! Null TCanvas pointer" << std::endl; return;}
    c->SaveSource((basename+".C").Data());
    c->Print((basename+".root").Data());
    c->Print((basename+".ps").Data());
    c->Print((basename+".gif").Data());
}

void Visualizer::ScaleAllHistsInCanvasWithBaseName(const TString& canvas_name,
                                                   const TString& basename,
                                                   Double_t scale_val) const
{
    std::vector<TH1*> hvec = GetAllHistsInCanvasWithBaseName(canvas_name, basename);
    std::vector<TH1*>::iterator it;
    for (it=hvec.begin(); it!=hvec.end(); it++)
    {
        (*it)->Scale(scale_val);
    }
    TCanvas *c = GetCanvas(canvas_name);
    if (c==NULL) return;
    else
        c->Update();

}

std::vector<TH1*> Visualizer::GetAllHistsInCanvasWithBaseName(const TString& canvas_name,
                                                              const TString& basename) const
{

    std::vector<TH1*> vec;
    TCanvas *can = GetCanvas(canvas_name);
    if (can==NULL) return vec;
    TSeqCollection *cancol = can->GetListOfPrimitives();
    TSeqCollection *pcol   = 0;
    TObject *pobj=0, *obj=0;
    for (Int_t i=0; i<cancol->GetEntries(); i++)
    { // loop1
        pobj = cancol->At(i);
        if (pobj->InheritsFrom("TFrame"))
            pcol = cancol;
        else if (pobj->InheritsFrom("TVirtualPad"))
        {
            pcol = (dynamic_cast<TPad*>(pobj))->GetListOfPrimitives();

            if (!pcol && pcol->GetEntries()<2)
	    { 
                std::cout << canvas_name << "_" << i+1
			<< " contains no more than a TFrame" 
			<< std::endl;
                continue;
	    }
        }
        else
            break;

        obj = pcol->At(1);
        if   (obj==NULL) continue;
        else if (obj->InheritsFrom("TH1")
            && TString(obj->GetName()).Contains(basename))
            vec.push_back(dynamic_cast<TH1* >(obj));
    }
    return vec;
}

void Visualizer::SetNRows(const Int_t nrows)
{ fNRows = nrows; }

void Visualizer::SetNColumns(const Int_t ncols)
{ fNCols = ncols; }

Int_t Visualizer::GetNRows(void) const
{ return fNRows; }

Int_t Visualizer::GetNColumns(void) const
{ return fNCols; }


void Visualizer::ForceUpdate()
{
    for ( Int_t i=0; i<fCanvases->GetEntries(); i++)
        (dynamic_cast<TCanvas*>(fCanvases->At(i)))->ForceUpdate();
}

void Visualizer::CloseAll()
{
    for (Int_t i=0; i<fCanvases->GetEntries(); i++)
        dynamic_cast<TCanvas*>(fCanvases->At(i))->Close();
}

void Visualizer::IconifyAll()
{
    for (Int_t i=0; i<fCanvases->GetEntries(); i++)
        dynamic_cast<TCanvas*>(fCanvases->At(i))->Iconify();

}
