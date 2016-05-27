
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


static const Char_t* setcfdthresh_cmd = "$CODA_BIN/vxcmd crumpler \"v812setthreshold";

static const Char_t* setledthresh_cmd = "$CODA_BIN/vxcmd crumpler \"v895setthreshold";

void
SetAllCFDThresholds(Int_t mod_index, Int_t val)
{
  Int_t dummy;

  for (Int_t i=0; i<16; i++)
    {
      dummy = system (Form("%s(%i,%i,%i)\" ", setcfdthresh_cmd, mod_index, i, val));
    }


}


void
SetCFDThresholdsFromFile(const Char_t* fname, Int_t mod_index)
{
  Int_t ch, val;

  Int_t i=0;
  Int_t dummy;

  ifstream ifile(fname);
  if(!ifile.is_open())
    {
      std::cout << fname << " does not exist" << std::endl;
      return;
    }
  else
    {
      ifile.ignore(1000,'\n');
      while (i<16)
	{
	  ifile >> ch >> val;
	  if (ifile.eof()) return;	  
	  if (val <256)	
	    dummy = system (Form("%s(%i,%i,%i)\" ", setcfdthresh_cmd, mod_index, i, val));
	  else 
	    std::cout 
	      << "Not a valid value between 0 and 255" 
	      << std::endl;

	  i++;
	}
      
      ifile.close();
     }
}

void
SetAllLEDThresholds(Int_t mod_index, Int_t val)
{
  Int_t dummy;

  for (Int_t i=0; i<16; i++)
    {
      dummy = system (Form("%s(%i,%i,%i)\" ", setledthresh_cmd, mod_index, i, val));
    }


}


void
SetLEDThresholdsFromFile(const Char_t* fname, Int_t mod_index)
{
  Int_t ch, val;

  Int_t i=0;
  Int_t dummy;

  ifstream ifile(fname);
  if(!ifile.is_open())
    {
      std::cout << fname << " does not exist" << std::endl;
      return;
    }
  else
    {
      ifile.ignore(1000,'\n');
      while (i<16)
	{
	  ifile >> ch >> val;
	  if (ifile.eof()) return;	  
	  if (val <256)	
	    dummy = system (Form("%s(%i,%i,%i)\" ", setledthresh_cmd, mod_index, i, val));
	  else 
	    std::cout 
	      << "Not a valid value between 0 and 255" 
	      << std::endl;

	  i++;
	}
      
      ifile.close();
     }
}

void
SetAllThresholdsHigh(void)
{
  SetAllCFDThresholds(0,255);
  SetAllCFDThresholds(1,255);

  SetAllLEDThresholds(0,255);
  SetAllLEDThresholds(1,255);
}
