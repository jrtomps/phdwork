// DoubleClickEnlarge.C
//
// Author : Jeromy Tompkins
// Date   : 12/5/2010
//
// Descrip: A toolpad to enlarge subpads for closer viewing.

#include <iostream>
#include <iomanip>
#include "TROOT.h"
#include "TSystem.h"
#include "TObject.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TPaveText.h"
#include "TButton.h"
#include "TList.h"
#include "TObjArray.h"
#include "TSeqCollection.h"
using namespace std;

TPad *vp;
TPad *newpad;
TMarker *mark;
TH1 *graph0, *graph1;
TCanvas *toolcanvas;
TCanvas *canvas;
Bool_t notDeleted;
TButton *enlarge, *bclose;
TObjArray *b;
Double_t w1;
Double_t h1;
TPaveText *pt,*pt1;
Int_t toolw, toolh;
TCanvas *display;


void
CloseNewPad()
{
  toolcanvas->cd();
  toolcanvas->GetListOfPrimitives()->Remove(bclose);
  enlarge->Draw();

  if (gROOT->GetListOfCanvases()->FindObject(display)!=0) display->Close();
}

TCanvas*
SafeConstructCanvas(const Char_t* canvas_name)
{
  TCanvas* cp=0;
  cp = dynamic_cast<TCanvas* >(gROOT->GetListOfCanvases()->FindObject(canvas_name));
  if (!cp) cp = new TCanvas(canvas_name,canvas_name);
  else cp->Clear();
  
  return cp;
}

void
Enlarge(Int_t i)
{
  
  //  toolcanvas->cd();
  TObject *o;
  TList *ll = toolcanvas->GetListOfPrimitives();
  for (Int_t ii=ll->GetEntries()-1; ii>=0; ii--)
    {
      o = ll->At(ii);
      ll->Remove(o);
    }
  toolcanvas->SetWindowSize(toolw-28, toolh-8);
  toolcanvas->cd();
  bclose->Draw();

  //  canvas->cd();
  vp = (TPad*)canvas->GetListOfPrimitives()->At(i);
  if (!vp) return;
  // else
  //   cout << "selected pad name " << vp->GetName() << endl;

  TList *alist;
  if (vp->GetListOfPrimitives()->GetEntries() > 1)
    {
      graph0 = (TH1*) vp->GetListOfPrimitives()->At(1);
      //      graph1 = (TH1*) graph0->Clone("graph1");
      alist = vp->GetListOfPrimitives();
    }
  else
    return;
  
  Char_t opt[50];
  strcpy(opt,graph0->GetDrawOption());

  //  display->Draw();
  display = SafeConstructCanvas("display");
  display->Draw();
  display->cd();
  gROOT->SetSelectedPad(display);

  // std::cout << "______Drawing Pad______\n\t" 
  // 	  << gPad->GetName() << std::endl;

  TString option;
  TH1* anh, *anhh;
  TObject *oo;
  for (Int_t i=1; i<alist->GetEntries(); i++)
    {
      oo = alist->At(i);

      if (oo->InheritsFrom(TH1::Class()))
        {
	anh = static_cast<TH1*>(oo);
	option = anh->GetDrawOption();
	option += "HE";
	if (i>1)
	  option += "same";

	if (oo->InheritsFrom(TH2::Class()))
	  anhh = anh->DrawCopy("colz");
	else
	  anhh = anh->DrawCopy(option.Data());
	anhh->SetFillStyle(0);
	anhh->SetLineColor(anh->GetLineColor());
	cout << anhh->GetName() << " with option=\"" 
	     << option.Data() << "\"" 
	     << " lcolor=" << anhh->GetLineColor()
	     << " fcolor=" << anhh->GetFillColor()
	     << " fstyle=" << anhh->GetFillStyle()
	     << flush << endl;
	display->Modified();
	display->Update();
	//	gSystem->Sleep(5000);
	
	//	anhh->SetFillColor(anh->GetFillColor());
	//	anhh->SetFillStyle(anh->GetFillStyle());
        }
      else if (i<alist->GetEntries()-1)
        oo->Draw();
    }
  display->Modified();
  display->Update();

  
}

void
SelectPad()
{
  notDeleted = kTRUE;

  toolcanvas->cd();
  // Clear buttons from toolcanvas
  TObject *o;
  TList *ll = toolcanvas->GetListOfPrimitives();
  for (Int_t ii=ll->GetEntries()-1; ii>=0; ii--)
    {
      o = ll->At(ii);
      ll->Remove(o);
    }

  Double_t buttonheight = 50;
  TList *li = canvas->GetListOfPrimitives();
  Int_t nentries = li->GetEntries();

  if (nentries==1)
    return;

  toolcanvas->SetWindowSize(toolw,(nentries+1)*buttonheight);
  toolcanvas->Modified();
  toolcanvas->Update();

  pt->SetY1NDC(1.0-1.0/(nentries+1));
  pt->Draw();

  toolcanvas->Update();


  //  toolcanvas->cd();
  if (b) {delete b;}
  
  Int_t count=0;
  b = new TObjArray(nentries);
  for (Int_t i=0; i<nentries; i++)
    {
      b->Add(new TButton(Form("pad%i",i+1),Form("Enlarge(%i);",i),0,(1-(count+1.)/(nentries+1)),1,(1-(count+2.)/(nentries+1))));
      b->At(i)->Draw();
      count++;
    }
  toolcanvas->Modified();
  toolcanvas->Update();
  //  if (display) {display->Modified(); display->Update();}
}


void
Pick(Int_t canvas_index)
{
  TObject *o;
  TCanvas* c = ((TCanvas*) gROOT->GetListOfCanvases()->At(canvas_index));
  c->cd();
  // std::cout << "______PICKED______\n\t" 
  // 	  << c->GetName() << " canvas_index=" << canvas_index << std::endl;
  //  ((TCanvas*) gROOT->GetListOfCanvases()->At(canvas_index))->cd();
  

  TList *ll = toolcanvas->GetListOfPrimitives();
  // std::cout << ll->GetEntries() << std::endl;
  for (Int_t ii=ll->GetEntries()-1; ii>=0; ii--)
    {
      o = ll->At(ii);
      // std::cout << ii << " removing " << ll->At(ii)->GetName() << std::endl;
      ll->Remove(o);
    }

  toolcanvas->cd();
  toolcanvas->SetWindowSize(toolw-28, toolh-8);
  //  toolcanvas->SetCanvasSize(toolw, toolh);
  pt->Draw();
  toolcanvas->Modified();
  toolcanvas->Update();
  canvas = ((TCanvas*) gROOT->GetListOfCanvases()->At(canvas_index));
  //Enlarge();
  SelectPad();
}

void
SelectCanvas()
{
  notDeleted = kTRUE;

  toolcanvas->cd();

  Double_t buttonheight = 50;
  TSeqCollection *li = gROOT->GetListOfCanvases();
  Int_t nentries = li->GetEntries();
  Int_t count=0;

  if (nentries==1)
    return;

  toolcanvas->GetListOfPrimitives()->Remove(enlarge);
  toolcanvas->SetWindowSize(toolw,nentries*buttonheight);
  toolcanvas->Modified();
  toolcanvas->Update();

  pt1->SetY1NDC(1.0-1.0/nentries);
  pt1->Draw();

  toolcanvas->Update();
  
  b = new TObjArray(nentries-1);
  for (Int_t i=0; i<nentries; i++)
    {
      if (strcmp(li->At(i)->GetName(),"tools")!=0)
        {
	b->Add(new TButton(li->At(i)->GetName(),Form("Pick(%i);",i),0,(1-(count+1.)/nentries),1,(1-(count+2.)/nentries)));
	b->At(count)->Draw();
	count++;

        }
    }

  toolcanvas->Modified();
  toolcanvas->Update();
}

void
DoubleClickEnlarge()
{
  toolw = 225;
  toolh = 65;
  toolcanvas = new TCanvas("tools","",1200,0,toolw,toolh);
  
  if (toolcanvas->GetShowEditor())
    toolcanvas->ToggleEditor();
  if (toolcanvas->GetShowEventStatus())
    toolcanvas->ToggleEventStatus();
  if (toolcanvas->GetShowToolBar())
    toolcanvas->ToggleToolBar();

  toolcanvas->Draw();
  toolcanvas->cd();

  enlarge = new TButton("Click to enlarge a pad","SelectCanvas();",0,0,1,1);
  enlarge->Draw();
 
  bclose = new TButton("Close","CloseNewPad();",0,0,1,1);

  pt = new TPaveText(0,0,1,1,"NDC");
  pt->AddText("Select a pad");

  pt1 = new TPaveText(0,0,1,1,"NDC");
  pt1->AddText("Select a canvas");
}
