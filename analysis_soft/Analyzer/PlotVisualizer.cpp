// PlotVisualizer.cpp
//
// Author : Jeromy Tompkins
// Date   : 4/10/2010
//
// Purpose: To plot and manage histograms in an intelligent manner.

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include "TH1.h"
#include "TCanvas.h"
#include "TObject.h"
#include "TString.h"
#include "TSeqCollection.h"
#include "TDirectory.h"
#include "PlotVisualizer.h"

ClassImp(PlotVisualizer)

PlotVisualizer::PlotVisualizer()
  : fDetMap(), fConfigFilename("")
{
  
  fIsValidMap = kTRUE;
}

PlotVisualizer::PlotVisualizer(const TString& configfile) 
  : fDetMap(), fConfigFilename(configfile)
{
  fIsValidMap = kTRUE;

  SetDetMapFromFile(fConfigFilename);
}

PlotVisualizer::~PlotVisualizer()
{}

PlotVisualizer::PlotVisualizer(PlotVisualizer const& obj)
{
  if ( this != &obj)
    {
      optr = obj.optr;
      cptr = obj.cptr;
      hptr = obj.hptr;
      fDetMap = obj.fDetMap;
      fIsValidMap = obj.fIsValidMap;
      fConfigFilename = obj.fConfigFilename;
    }
}

PlotVisualizer&
PlotVisualizer::operator=(PlotVisualizer const& obj)
{
  if ( this != &obj)
    {
      optr = obj.optr;
      cptr = obj.cptr;
      hptr = obj.hptr;
      fDetMap = obj.fDetMap;
      fIsValidMap = obj.fIsValidMap;
      fConfigFilename = obj.fConfigFilename;
    }
  return *this;
}

void
PlotVisualizer::Draw(const TString& basename, TString options)
{
  if (!fConfigFilename.IsNull())
    ResetMap();
  
  if (! fIsValidMap ) 
    {
      std::cout << "Cannot draw! Map is not valid" << std::endl;
      return;
    }

  TString canname;
  Bool_t drawLogy=0;
  Bool_t specificCanvas=0;
  options.Strip();
  options.ToLower();

  drawLogy = SetAndRemoveLogy(options);
  canname  = SetAndRemoveSpecifiedCanvas(options);
  if (!canname.IsNull()) specificCanvas = kTRUE;

  Int_t hist_index=0, failcount=0, maxfails=2;
  Int_t pad_index=0, canvas_count=0, xoff=20;

  TDirectory *dir = gDirectory;
  TString RunNumberStr = FormRunNumberExtension(GuessRunNumberFromName(dir->GetName()));
  
  TH1 *h;
  TCanvas *pc=0;

  do 
    {
      h = 0;
      h = dynamic_cast<TH1*>(dir->FindObjectAny(Form("%s%i",basename.Data(),hist_index)));
      if (h!=0)
        {
	if (pad_index%(fNRows*fNCols)==0)
	  {
	    if (specificCanvas)
	      pc = GetCanvas(TString::Format("%s_%i",canname.Data(),canvas_count));
	    else
	      pc = dynamic_cast<TCanvas*>(fCanvases->FindObject(Form("%s%s_%i",basename.Data(),RunNumberStr.Data(),canvas_count)));
	    if (!pc)
	      {
	        if (specificCanvas) 
		  std::cout << canname.Data() << " did not exist."
			    << "Creating new canvas: " << basename.Data() << std::endl;
		
	        fCanvases->Add(new TCanvas(Form("%s%s_%i",basename.Data(),RunNumberStr.Data(),canvas_count),
					   Form("%s%s_%i",basename.Data(),RunNumberStr.Data(),canvas_count),
					   10+xoff*canvas_count,
					   10+xoff*canvas_count,
					   800,
					   800));
	        pc = dynamic_cast<TCanvas*>(fCanvases->FindObject(Form("%s%s_%i",basename.Data(),RunNumberStr.Data(),canvas_count)));
		pc->Clear();
	        pc->Divide(fNCols,fNRows,0.001,0.001);
	        pad_index=0;
	      }
	    if (drawLogy)
	      {
	        SetLogy(pc->GetName(),drawLogy);
	        pc->Update();
	      }
	    canvas_count++;
	  }

	if (fDetMap.size() < hist_index-failcount+1)
	  fDetMap[hist_index-failcount]=pad_index+1;
	 
	pc->cd(fDetMap[hist_index-failcount]);
	h->Draw(options.Data());
	pad_index++;
        }
      else
        {
	  std::cout << Form("%s%i", basename.Data(), hist_index) << " doesn't exist" << std::endl;
	  failcount++;
        }
      hist_index++;
    } while (failcount < maxfails);
}

void
PlotVisualizer::DrawAll(void)
{
  DrawTDC();
  DrawADC();
}

void 
PlotVisualizer::DrawTDC(TString opt)
{
  Draw("tdc", opt);
}

void 
PlotVisualizer::DrawADC(TString opt)
{
  Draw("adc", opt);
}

void
PlotVisualizer::PrintMap(void)
{
  std::map<Int_t,Int_t>::iterator it;

  for (it=fDetMap.begin(); it!=fDetMap.end(); it++)
    {
      std::cout 
	<< std::setw(5) << (*it).first 
	<< std::setw(5) << (*it).second
	<< std::endl;
    }
  std::cout << "size=" << fDetMap.size() << std::endl;
}

Bool_t
PlotVisualizer::IsAValidMapping(void)
{

  Bool_t validity = kTRUE;
  Int_t max_key=0, max_map=0;
  Int_t max = fDetMap.size();

  std::map<Int_t,Int_t>::iterator it;

  // Verify there are no out-of-bound values
  for (it=fDetMap.begin(); it!=fDetMap.end(); it++)
    {
      if ((*it).first >= max || (*it).second > max)
	validity = kFALSE;
      if ((*it).first < 0 || (*it).second < 0)
	validity = kFALSE;

      if (!validity) 
	std::cout 
	  << "Invalid entry\n"
	  << std::setw(5) << (*it).first 
	  << std::setw(5) << (*it).second
	  << std::endl;
    }
 
  // BELOW IS NOT A VALID TEST IF MORE THAN 1 CANVAS !
  // Verify that no pads are repeat values
  //   std::map<Int_t, Int_t>::iterator it2;
  //   for (it=fDetMap.begin(); it!=fDetMap.end(); it++)
  //     for (it2=fDetMap.begin(); it2!=fDetMap.end(); it2++)
  //       {
  // 	if ( (*it).first != (*it2).first
  // 	     && (*it).second == (*it2).second )
  // 	  validity = kFALSE;
  //       } 
  
  return validity;
}

void
PlotVisualizer::ResetMap(void)
{
  fDetMap.clear();
  SetDetMapFromFile(fConfigFilename);
}

void
PlotVisualizer::SetDetMapFromFile(const TString& configfile)
{
  fConfigFilename = configfile;
  std::ifstream file(configfile.Data(), std::ifstream::in);
  if (!file.is_open()) 
    {
      std::cout << "file " << configfile.Data() << " doesn't exist" << std::endl;
      return;
    }

  Int_t key_val, mapped_val;
  while(1)
    {
      file >> key_val >> mapped_val;
      if (file.eof()) break;
      if (file.bad() || file.fail())
	{
	  std::cout << "unexpected stop: file state=" << file.rdstate() << std::endl;
	  break;
	}

      fDetMap[key_val] = mapped_val;

    }

  fIsValidMap = IsAValidMapping();
  
}
