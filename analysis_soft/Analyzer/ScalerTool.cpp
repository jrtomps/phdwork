// ScalerTool.cpp
//
// Written by: Jeromy Tompkins
// Date: 3/18/09
// Purpose:
//      This class creates a tool object that can perform operations on
//      a scaler output file from CODA
//
//---------------------------------------------------------------

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "ScalerTool.h"

ClassImp(ScalerTool);

// Default Constructor
ScalerTool::ScalerTool() 
  : TObject(), sclrvals(32,0), runloaded(0), 
    ch(-1), run(-1)
{}

// Constructor - create object that is linked with scaler file
ScalerTool::ScalerTool (std::string scaler_filename) 
  : TObject(), sclrvals(32,0), runloaded(0), run(-1) 
{  scalerfile.open (scaler_filename.data());}


ScalerTool::ScalerTool(ScalerTool const& obj) 
  : TObject(obj), sclrvals(obj.sclrvals), runloaded(obj.runloaded),
    run(obj.run)
{
  scalerfile.std::ios::rdbuf(obj.scalerfile.rdbuf());
}

ScalerTool&
ScalerTool::operator=(ScalerTool const& obj)
{
  if (this!=&obj)
    {
      sclrvals  = obj.sclrvals;
      runloaded = obj.runloaded;
      ch        = obj.ch;
      run       = obj.run;
    }
  return *this;
}

// Destructor
ScalerTool::~ScalerTool ()
{  scalerfile.close ();}

Int_t
ScalerTool::GetValueByIndex (Int_t run_num, UInt_t index)
{
 
  if (!(runloaded && run==run_num))
    LoadRun (run_num);
  
  return sclrvals[index];
}

void
ScalerTool::LoadRun(Int_t runnum)
{
  FindRun(runnum);
  LoadRunIntoBuffer();
}

Int_t
ScalerTool::GetRun()
{ return run;}


// skip to the desired run # output, runnum
void
ScalerTool::FindRun (Int_t runnum)
{

  scalerfile.clear();
  scalerfile.seekg(0,std::ios_base::beg);
  
  std::string temp, tmp;
  Int_t tmprun;
  temp = SkipToNextRun();
  while(1)
    {
      tmprun = ParseRunNumberFromString(temp);
#ifdef DEBUG_SCLRTOOL
      std::cout << "Desired run: " << std::setw(6) << tmprun << "Found run: " << std::setw(8) << runnum << std::endl;
#endif
      
      //      tmprun = ParseRunNumberFromString(temp);
            
      if (!isOkayToProceed())
	break;

      if (tmprun!=runnum)
	temp = SkipToNextRun();
      else
	break;
    }
  run = runnum;
}

Bool_t 
ScalerTool::isOkayToProceed() const
{
  Bool_t flag=true;
  if (scalerfile.eof())
    {
      std::cout 
	<< "Reached end of file" 
	<< std::endl;
      flag = false;
    }
  if (scalerfile.rdstate()!=0)
    {
      std::cout 
	<< "Warning! fstream has become corrupted" 
	<< std::endl;
      flag = false;
    }
  return flag;  
}

void
ScalerTool::LoadRunIntoBuffer() 
{
  std::string tstr;
  bool found_data=false;
  // Skip over run header
  //  for (Int_t i=0; i<3+1; i++)  
    // the +1 is to ensure the clearing of the '\n'
  while (!found_data)
    {
      getline(scalerfile, tstr);
      if (tstr.find("0")==0)
	{
	  found_data=true;
	  break;
	}
      else if (tstr.find("Run:")!=std::string::npos)
	{
	  found_data=false;
	  std::cerr << "Warning! No data was found!" << std::endl;
	}
#ifdef DEBUG_SCLRTOOL
      std::cout << tstr << std::endl;
#endif
    }

  if (found_data)
    {
      for (Int_t i=0; i<32; i++) 
	{
	  std::stringstream ss(tstr);
	  ss >> ch >> sclrvals[i];    
#ifdef DEBUG_SCLRTOOL
	  std::cout << std::left  << std::setw(4)  << ch
		    << std::right << std::setw(10) << sclrvals[i]
		    << std::endl;
#endif
	  getline(scalerfile,tstr);
	}
      runloaded=true;
    }
  else
    {
      for (Int_t i=0; i<32; i++) sclrvals[i] = 0;
      runloaded=false;
    }
}

std::string
ScalerTool::SkipToNextRun(void)
{
#ifdef DEBUG_SCLRTOOL
  std::cout << "Skipping to next run" << std::endl;
#endif

  std::string temp;
  while (1)
    {
      getline(scalerfile, temp);
      if (temp.find("Run") != std::string::npos)  break;
    }
  return temp;
}

Int_t
ScalerTool::ParseRunNumberFromString(std::string str)
{
  Int_t runnumber;
  std::stringstream ss(str);
  ss >> str >> runnumber;
  return runnumber;
}     

Int_t ScalerTool::SumValueForRunsByIndex(Int_t run_first, Int_t run_last, UInt_t index)
{
    Int_t sum = 0;
    for (UInt_t run=run_first; run<=run_last; run++)
    {
        sum += GetValueByIndex(run,index);
    }
    return sum;
}
