// SScalerTool.cpp
//
// Author : Jeromy Tompkins
// Date   : 8/6/2010
//
// Descrip: A "Super Scaler Tool" with ability to read scaler.config
//          or any generic input file.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include "TObject.h"
#include "ScalerTool.h"
#include "SScalerTool.h"

using namespace std;

ClassImp(SScalerTool);

SScalerTool::SScalerTool(string filename, string configfilename)
    : ScalerTool(filename),
    fChNames(32,"")

{
  ReadGenericScalerConfig(configfilename);
}

SScalerTool::SScalerTool(const SScalerTool &obj)
    : ScalerTool(obj),
    fChNames(obj.fChNames)
{}

SScalerTool::~SScalerTool() {}

void SScalerTool::ReadScalerDotConfig()
{
  Int_t ch;
  ifstream ifile("scaler.config",ifstream::in);

  string tmp;
  for (Int_t i=0; i<16; i++) {
    getline(ifile,tmp);
  }

  cout << "Found the following channels" << endl;
  for (Int_t i=0; i<32; i++) 
    {
      getline(ifile,tmp);
      cout << tmp.c_str() << endl;
      fChNames[i] = tmp.substr(3,tmp.length()-3);
    }
  
  ifile.close();

}

void SScalerTool::ReadGenericScalerConfig(string filename)
{
  // Read a generic input file
  // Ignores any lines that begin with $, /, or !.
  // Expects the format:
  // ## "Channel Name"

  ifstream ifile(filename.data(),ifstream::in);

  Int_t pos_space = 0;
  Int_t count = 0, channel;
  
  for (Int_t i=0; i<16; i++) ifile.ignore(10000,'\n');

  while (count < 32)
    {
      ifile >> channel >> tmp;

      if (ifile.rdstate()!=0) break;

      fChNames[channel] = tmp;

      count++;
    }
  
  if ( count!=31 ) 
    {
      cout << "WARNING:Found only " << count << " channels" << endl;
  
      for ( Int_t i=0; i<32; i++) 
        {
	  if (fChNames[i].empty())
        fChNames[i] = string("empty");
        }
    }
  
  ifile.close();
  
}

Int_t SScalerTool::GetValueByName(Int_t run, string ch_name)
{

  Int_t ch = FindChannel(ch_name);
  if (ch >= 0 )
    return ScalerTool::GetValueByIndex(run, ch);
  else
    return -1;
  
}

Int_t SScalerTool::FindChannel(string ch_name)
{
  Int_t len;
  for (Int_t i=0; i<32; i++) 
    {

      if( fChNames[i].compare(ch_name) == 0)
        return i;    
    }
  
  cout << "Warning " << ch_name << " doesn't exist" << endl;
  return -1;

}

void SScalerTool::PrintChannels(void) const
{
  for (Int_t i=0; i<32; i++) {
    cout.width(2);
    cout << i << " : " << fChNames[i].c_str() << endl;
  }

}

Int_t SScalerTool::SumValueForRunsByName(Int_t run_first, Int_t run_last, string ch_name)
{
    Int_t ret;

    Int_t ch = FindChannel(ch_name);

    if (ch!=-1)
        ret = SumValueForRunsByIndex(run_first,run_last,ch);
    else
        ret = -1;

    return ret;
}


