// ConfigEntry.cpp
//
// Author : Jeromy Tompkins
// Date   : 8/20/2010
//
// Descrip: Derived class for configuration entry.

#include <fstream>			        
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include "TObject.h"
#include "ExtensibleDb.h"
#include "ConfigEntry.h"

ClassImp(ConfigEntry)

ConfigEntry::ConfigEntry (void) 
: TObject(), 
  fNDets(0), 
  fNADCChannels(0), 
  fNTDCChannels(0),
  fRunDuration(0),
  fLiveTime(0), 
  fTarget(""),
  fPol(""),
  fComment(""),
  fExtDb(0)
{ isInitialized = false; }

ConfigEntry::ConfigEntry (Int_t nentries) 
  : TObject(), 
    fNDets(nentries),
    fNADCChannels(nentries), 
    fNTDCChannels(nentries), 
    fRunDuration(0),
    fLiveTime(0), 
    fTarget(""),
    fPol(""),
    fComment(""),
    fExtDb(0)
{
  isInitialized = true;
  // Initialize all arrays
  fPed      = new Float_t[fNADCChannels];
  fThresh   = new Float_t[fNADCChannels];

  functionmap["angle"]        = 1;
  functionmap["adc_edge"]     = 2;
  functionmap["desiredmaxph"] = 3;
  functionmap["det_dist"]     = 4;
  functionmap["deut_en"]      = 5;
  functionmap["energy"]       = 6;
  functionmap["gflash"]       = 7;
  functionmap["ntof"]         = 8;
  functionmap["ped"]          = 9;
  functionmap["tdc_calib"]    = 10;
  functionmap["tdc_offset"]   = 11;
  functionmap["thresh"]       = 12;
  functionmap["target"]       = 13;
  functionmap["pol"]          = 14;
  functionmap["tofvspsdcutfilename"] = 15;
  functionmap["psdcutfilename"] = 16;
  
}

ConfigEntry::ConfigEntry (std::ifstream& str) 
  : TObject(),  
    fNDets(0),
    fNADCChannels(0), 
    fNTDCChannels(0), 
    fRunDuration(0),
    fLiveTime(0), 
    fTarget(""),
    fPol(""),
    fComment(""),
    fExtDb(0)
{
  isInitialized = true;

  // Set run info and the number of entries
  LoadRunInfo(str);

  // Initialize all arrays
  fPed      = new Float_t[fNADCChannels];
  fThresh   = new Float_t[fNADCChannels];
  
  LoadExtensibleDb(str);

  functionmap["angle"]        = 1;
  functionmap["adc_edge"]     = 2;
  functionmap["desiredmaxph"] = 3;
  functionmap["det_dist"]     = 4;
  functionmap["deut_en"]      = 5;
  functionmap["energy"]       = 6;
  functionmap["gflash"]       = 7;
  functionmap["ntof"]         = 8;
  functionmap["ped"]          = 9;
  functionmap["tdc_calib"]    = 10;
  functionmap["tdc_offset"]   = 11;
  functionmap["thresh"]       = 12;
  functionmap["target"]       = 13;
  functionmap["pol"]          = 14;
  functionmap["tofvspsdcutfilename"] = 15;
  functionmap["psdcutfilename"] = 16;

#ifdef DEBUG
  std::cout << "ConfigEntry constructor complete " << std::endl;
  std::cout.flush();
#endif
}

ConfigEntry::ConfigEntry (Int_t nentries, std::ifstream& str) 
  : TObject(), 
    fNDets(nentries),
    fNADCChannels(nentries), 
    fNTDCChannels(nentries), 
    fRunDuration(0),
    fLiveTime(0), 
    fTarget(""),
    fPol(""),
    fComment(""),
    fExtDb(0)
{
  isInitialized = true;
  LoadRunInfo(str);
  // Initialize all arrays
  
  fPed      = new Float_t[fNADCChannels];
  fThresh   = new Float_t[fNADCChannels];

  LoadExtensibleDb(str);

  functionmap["angle"]        = 1;
  functionmap["adc_edge"]     = 2;
  functionmap["desiredmaxph"] = 3;
  functionmap["det_dist"]     = 4;
  functionmap["deut_en"]      = 5;
  functionmap["energy"]       = 6;
  functionmap["gflash"]       = 7;
  functionmap["ntof"]         = 8;
  functionmap["ped"]          = 9;
  functionmap["tdc_calib"]    = 10;
  functionmap["tdc_offset"]   = 11;
  functionmap["thresh"]       = 12;
  functionmap["target"]       = 13;
  functionmap["pol"]          = 14;
  functionmap["tofvspsdcutfilename"] = 15;
  functionmap["psdcutfilename"] = 16;

#ifdef DEBUG
  std::cout << "constructor done " << std::endl;
  std::cout.flush();
#endif
}

ConfigEntry::~ConfigEntry (void) 
{
#ifdef DEBUG
  std::cout << "Destructing" << std::endl;
#endif 
  if (isInitialized) {
    delete [] fPed;
    delete [] fThresh;
  }

  if (fExtDb!=0) { delete fExtDb; fExtDb=0; }
}

ConfigEntry::ConfigEntry(ConfigEntry const& obj) 
  : TObject(obj),
    fNDets(obj.fNDets), 
    fNADCChannels(obj.fNADCChannels), 
    fNTDCChannels(obj.fNTDCChannels), 
    fRunDuration(obj.fRunDuration),
    fLiveTime(obj.fLiveTime), 
    fTarget(obj.fTarget),
    fPol(obj.fPol),
    fComment(obj.fComment)
{
  //  std::cout << "copying" << std::endl;
  if (this != &obj)
    {
      isInitialized = obj.isInitialized;
      fRun = obj.fRun;
      fEnergy = obj.fEnergy;
      fExtDb = new ExtensibleDb(*obj.fExtDb);

      // Initialize all arrays
      fPed      = new Float_t[fNADCChannels];
      fThresh   = new Float_t[fNADCChannels];
    
      // Copy arrays
      std::copy(obj.fPed,      obj.fPed+obj.fNADCChannels,      fPed);
      std::copy(obj.fThresh,   obj.fThresh+obj.fNADCChannels,   fThresh);
    }
}

ConfigEntry&
ConfigEntry::operator=(ConfigEntry const& obj)
{
  if (this != &obj)
    {
      fNDets = obj.fNDets;
      fNADCChannels = obj.fNADCChannels;
      fNTDCChannels = obj.fNTDCChannels;
      fLiveTime     = obj.fLiveTime;
      fRunDuration  = obj.fRunDuration;
      isInitialized = obj.isInitialized;
      fRun = obj.fRun;
      fTarget = obj.fTarget;
      fPol = obj.fPol;
      fEnergy = obj.fEnergy;
      fComment = obj.fComment;
      std::copy(obj.fPed,      obj.fPed+obj.fNADCChannels,      fPed);
      std::copy(obj.fThresh,   obj.fThresh+obj.fNADCChannels,   fThresh);

      (*fExtDb) = *(obj.fExtDb);
    }
  return *this;
}

Bool_t
ConfigEntry::operator==(ConfigEntry const& obj)
{  return (obj.GetRun() == fRun) ? true : false;}

void 
ConfigEntry::LoadRunInfo(std::ifstream& ifile)
{
  if (ifile.rdstate()!=0) return;
  
  ifile >> fRun;
  if (ifile.rdstate()!=0)
    {
      fRun = 0;
      
#ifdef DEBUG
      std::cout << "Unexpected end of file" << std::endl;
#endif 

      return;
    }
  ifile >> fTarget;
  ifile >> fEnergy;
  ifile >> fPol;
  ifile >> fRunDuration;
  ifile >> fLiveTime;
  ifile >> fNDets;
  ifile >> fNADCChannels;
  ifile >> fNTDCChannels;
  
  getline(ifile,fComment,'\"');
  getline(ifile,fComment,'\"');

#ifdef DEBUG
  std::cout.width(10);
  std::cout
    << fRun << " "
    << fTarget << " "
    << fEnergy << " "
    << fPol <<    " " 
    << "\"" << fComment << "\""
    << std::endl;
#endif

}

void 
ConfigEntry::LoadExtensibleDb (std::ifstream& ifile)
{
  if (ifile.rdstate()!=0) return;
  
#ifdef DEBUG
  std::cout << std::endl;
  std::cout << ifile.bad() << " " << ifile.eof() << " " << ifile.fail() << std::endl;
#endif
  

  Char_t ac='a';
  ifile >> std::noskipws;
  while (ac==' ' || (ac!='\n' && ac!='|')) 
    {
      ifile >> ac;
#ifdef DEBUG
      std::cout << ac;
#endif
    }
  ifile >> std::skipws;
  if (ac=='|') fExtDb = new ExtensibleDb(ifile);
  else std::cout << "No ExtensibleDb entries found" << std::endl;
}

void 
ConfigEntry::WriteToFile (std::ofstream& ofile, Char_t spacer1)
{

  if (ofile.is_open())
    {
      ofile << fRun    << spacer1;
      ofile << fTarget << spacer1;
      ofile << fEnergy << spacer1;
      ofile << fPol    << spacer1;
      ofile << fRunDuration  << spacer1;
      ofile << fLiveTime     << spacer1;

      ofile << fNDets  << spacer1;
      ofile << fNADCChannels << spacer1;
      ofile << fNTDCChannels << spacer1;

      ofile << "\"" << fComment << "\"" << spacer1; 

      if (fExtDb!=0) 
        {
	ofile << " | ";
	fExtDb->Write(ofile,spacer1);
        }

      ofile << std::endl;

    }

}

void
ConfigEntry::InteractivelyConstructDbEntry(const Int_t runnum)
{
  fRun = runnum; 
  InteractivelySetupRunInfo(runnum,true);

  if (! isInitialized)
    {
      fThresh = new Float_t[fNADCChannels];
      fPed    = new Float_t[fNADCChannels];
      isInitialized=true;
    }
  InteractivelySetupArray("Pedestal", fNADCChannels, fPed);
  InteractivelySetupArray("Threshold", fNADCChannels, fThresh);

  Char_t ans;
  std::cout << "Do you want to add an extensible database? (y/n) : ";
  std::cin  >> ans;
  if (ans=='y' || ans=='Y')
    {
      if (fExtDb==0)
        fExtDb = new ExtensibleDb;
      else
        fExtDb->InteractivelyBuildDb();
    }
}


void
ConfigEntry::InteractivelySetupArray(const Char_t *name, 
				     const Int_t nchannels, 
				     Float_t* arr)
{
  Char_t ans;
  Float_t val;
  std::cout << std::setw(30) <<  name << std::endl;
  std::cout.fill('-');
  std::cout << std::setw(30) << '-' << std::endl;
  std::cout.fill(' ');
  std::cout << "Enter values individually? (y/n) : ";
  std::cin  >> ans;
  if (ans=='n' || ans=='N')
    {
      std::cout << "Value = ";
      std::cin  >> val;
      for (Int_t j=0; j<nchannels; j++) 
	{ arr[j] = val;}
    }
  else
    {
      for (Int_t j=0; j<nchannels; j++)
	{
	  std::cout << "val[" << std::setw(3) << j << "] = ";
	  std::cin  >> val;
	  arr[j] = val;
	}
    }
}

void
ConfigEntry::InteractivelySetupRunInfo(const Int_t runnum, Bool_t extensive)
{ 
  using std::setw;
  using std::left;

  fRun = runnum;
  
  std::cout << setw(30) << left << "Enter energy " << ": ";
  std::cin  >> fEnergy;
  
  std::cout << setw(30) << left << "Enter target name " << ": ";
  std::cin  >> fTarget;
  
  std::cout << setw(30) << left << "Enter polarity (Lin/Circ) " << ": ";
  std::cin  >> fPol;
  
  if (extensive)
    {
      std::cout << setw(30) << left << "Enter run duration (seconds)" << ": ";
      std::cin  >> fRunDuration;
      
      std::cout << setw(30) << left << "Enter live time (seconds)" << ": ";
      std::cin  >> fLiveTime;
      
      
      std::cout << setw(30) << left << "Enter number of detectors" << ": ";
      std::cin  >> fNDets;
      
      std::cout << setw(30) << left << "Enter number of ADC channels" << ": ";
      std::cin  >> fNADCChannels;
      
      std::cout << setw(30) << left << "Enter number of TDC channels" << ": ";
      std::cin  >> fNTDCChannels;

      std::cin >> std::ws;
      std::cout << setw(30) << left << "Enter a comment" << ": ";
      getline(std::cin, fComment);

    }

  std::cout << std::endl;
}

// Getter functions
const Char_t*    
ConfigEntry::GetTarget (void) const
{return fTarget.data();}

const Char_t*    
ConfigEntry::GetPol (void) const
{return fPol.data();}

Float_t    
ConfigEntry::GetEnergy (void) const
{return fEnergy;}

Float_t    
ConfigEntry::GetLiveTime (void) const
{return fLiveTime;}

Int_t
ConfigEntry::GetNDets (void) const
{ return fNDets;}


Int_t
ConfigEntry::GetNADCChannels (void) const
{ return fNADCChannels;}


Int_t
ConfigEntry::GetNTDCChannels (void) const
{ return fNTDCChannels;}

Float_t*
ConfigEntry::GetPed (void) const
{return fPed;}

Int_t       
ConfigEntry::GetRun (void) const
{return fRun;}

Float_t 
ConfigEntry::GetRunDuration (void) const
{return fRunDuration;}

Float_t*  
ConfigEntry::GetThresh (void) const
{return fThresh;}

std::string
ConfigEntry::GetComment(void) const
{ return fComment;}

const ExtensibleDb* 
ConfigEntry::GetExtensibleDb(void) const
{  return fExtDb;}

ExtensibleDb* 
ConfigEntry::GetExtensibleDb(void)
{ return fExtDb;}

// Setter functions
void
ConfigEntry::SetTarget (const Char_t *newname)
{// strcpy(fTarget,newname);
  fTarget = newname;
}

void
ConfigEntry::SetPol (const Char_t *newpol)
{ //strcpy(fPol, newpol);
  fPol = newpol;
}
  

void
ConfigEntry::SetEnergy(const Float_t val)
{ fEnergy = val;}

void
ConfigEntry::SetLiveTime(const Float_t val)
{ fLiveTime = val;}

void ConfigEntry::SetNDets(const Int_t ndets)
{
    fNDets = ndets;
}

void ConfigEntry::SetNADCChannels(const Int_t ndets)
{
    fNADCChannels = ndets;
}

void ConfigEntry::SetNTDCChannels(const Int_t ndets)
{
    fNTDCChannels = ndets;
}

void
ConfigEntry::SetPed (const Int_t index, const Float_t val)
{
  if (fPed) fPed[index] = val;
  else if (index > fNDets)
    std::cout << "Index out of bounds" << std::endl;
  else std::cout << "fPed not initialized" << std::endl;
}

void   
ConfigEntry::SetRun (const Int_t run)
{fRun = run;}

void
ConfigEntry::SetRunDuration(const Float_t val) 
{fRunDuration = val;}

void   
ConfigEntry::SetThresh (const Int_t index, const Float_t val)
{
  if (fThresh) fThresh[index] = val; 
  else if (index > fNDets)
    std::cout << "Index out of bounds" << std::endl;
  else std::cout << "fThresh not initialized" << std::endl;
}


void   
ConfigEntry::SetPed        (std::vector<Float_t>& val)
{ 
  std::copy(val.begin(), val.end(), fPed);
}

void   
ConfigEntry::SetThresh     (std::vector<Float_t>& val)
{ 
  std::copy(val.begin(), val.end(), fThresh);
}

void
ConfigEntry::SetComment     (std::string const& comment)
{
  fComment = comment;
}

void
ConfigEntry::SetComment     (void)
{
  std::cout << "Enter comment : ";
  getline(std::cin,fComment);
}

void 
ConfigEntry::SetExtensibleDb(const ExtensibleDb* edb)
{
  if (fExtDb!=0) delete fExtDb;
  
  fExtDb = new ExtensibleDb(*edb);
}

void 
ConfigEntry::SetMember (const Char_t *id, std::vector<Float_t>& val)
{
  std::string sid(id);
  std::map<std::string, Int_t>::iterator it;
  
  it = functionmap.find(sid);
  switch(it->second) 
    {
    case 6:
      SetEnergy  (val[0]);
      std::cout << "Set energy to " << val[0] << std::endl;
      break;
    case 9:
      SetPed(val);
      std::cout << "Set ped" << std::endl;
      break;
    case 12:
      SetThresh(val);
      std::cout << "Set thresh" << std::endl;
      break;
    default:
      std::cout << "Requested id does not exist." << std::endl;
      std::cout << "Did you mean one of the following?" << std::endl;
      PrintFunctionKeys();
    }
}

void 
ConfigEntry::SetMember (const Char_t *id, const Char_t* name)
{
  std::string sid(id);
  std::map<std::string, Int_t>::iterator it;
  
  it = functionmap.find(sid);
  switch(it->second) 
    {
    case 13:
      SetTarget(name);
      std::cout << "Set target name to : ";
      std::cout << name << std::endl;
      break;
    case 14:
      SetPol(name);
      std::cout << "Set polarization to : ";
      std::cout << name << std::endl;
      break;
    default:
      std::cout << "Requested id does not exist." << std::endl;
      std::cout << "Did you mean one of the following?" << std::endl;
      PrintFunctionKeys();
      //      GuessKey(sid);
    }
}
 
void
ConfigEntry::Print(Option_t*) const
{
  std::cout.precision(3);
  PrintRunInfo();
  std::cout << "NDets     : " << fNDets << std::endl;
  std::cout << "ADC ch    : " << fNADCChannels << std::endl;
  std::cout << "TDC ch    : " << fNTDCChannels << std::endl;

  PrintThresh();
  PrintPed();

}


void 
ConfigEntry::PrintRunInfo(const Int_t wint, const Int_t wstr, const Int_t wfloat) const
{
  using std::setw;
  using std::setprecision;
  std::cout << std::left 
	    << std::setiosflags(std::ios::fixed); 
	    
  std::cout << setprecision(0)
	    << setw(wint) << fRun
	    << setw(wstr) << fTarget
	    << setprecision(1) 
	    << setw(6) << fEnergy
	    << setw(8) << fPol
	    << std::right
	    << setprecision(0)
	    << setw(wfloat) << fRunDuration
	    << setprecision(1)
	    << setw(wfloat) << fLiveTime;
  std::cout << std::left
	    << " " << fComment;
  
  std::cout << std::endl;

}

void
ConfigEntry::PrintFunctionKeys() 
{
  std::map<std::string, Int_t>::iterator it;
  for (it=functionmap.begin(); it!=functionmap.end(); it++)
    {
      std::cout << it->first << " " << it->second << std::endl;      
    }
}

void
ConfigEntry::GuessKey(std::string& str)
{
  std::cout << "Did you mean : ";
  std::map<std::string, Int_t>::iterator it;
      for (it=functionmap.begin(); it!=functionmap.end(); it++)
        if (str.compare(it->first)>-3 && str.compare(it->first)<3) std::cout << it->first << " ";
  std::cout << std::endl;
}


void  
ConfigEntry::PrintThresh() const
{
  std::cout << "Thresh    : ";
  for (Int_t i=0; i<fNADCChannels; i++)
    {
      //      std::cout.precision(2);
      std::cout.width(7);
      std::cout << fThresh[i];
    }
  std::cout << std::endl;
}


void  
ConfigEntry::PrintPed() const 
{
  std::cout << "Ped       : ";
  for (Int_t i=0; i<fNADCChannels; i++)
    {
      //      std::cout.precision(2);
      std::cout.width(7);
      std::cout << fPed[i];
    }
  std::cout << std::endl;
}

