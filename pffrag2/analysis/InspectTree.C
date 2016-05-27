#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "TTree.h"

using namespace std;

string
FormPatternSection(const Int_t i, const Char_t* moduleid="ADC2")
{
  string pattern(moduleid);
  stringstream spattern(pattern);
  spattern.seekp(spattern.str().length());
  spattern << setiosflags(ios::fixed);
  spattern << setfill('0') << setw(2) << setprecision(0) << i;
  return spattern.str();
}

string
FormCutSection(const Int_t i, const Char_t* moduleid="ADC2")
{
  string pattern(moduleid);
  stringstream spattern(pattern);
  spattern.seekp(spattern.str().length());
  spattern << setiosflags(ios::fixed);
  spattern << setfill('0') << setw(2) << setprecision(0) << i 
	   << ">0";
  return spattern.str();
}

void
InspectADC2(const Int_t i0, const Int_t i1)
{
  if ((i1-i0)>=7)
    cerr << "Warning! Range of channels too large" << endl;
  
  string pattern("");
  string cutpattern("");
  for (Int_t i=i0; i<=i1; i++)
    {
      pattern.append(FormPatternSection(i,"ADC2"));
      if (i != i1) pattern.append(":");

      cutpattern.append(FormCutSection(i,"ADC2"));
      if (i != i1) cutpattern.append("||");
    }

  cout << "Scanning branches :: " << pattern.data() << endl;
  cout << "Cut pattern       :: " << cutpattern.data() << endl;
  flat_tree->Scan(pattern.data(),cutpattern.data());
};

void
SelectiveScan(const Int_t i0, const Int_t i1,const Char_t* moduleid="ADC0")
{
  if ((i1-i0)>=7)
    cerr << "Warning! Range of channels too large" << endl;
  
  string pattern("");
  string cutpattern("");
  for (Int_t i=i0; i<=i1; i++)
    {
      pattern.append(FormPatternSection(i,moduleid));
      if (i != i1) pattern.append(":");

      cutpattern.append(FormCutSection(i,moduleid));
      if (i != i1) cutpattern.append("||");
    }

  cout << "Scanning branches :: " << pattern.data() << endl;
  cout << "Cut pattern       :: " << cutpattern.data() << endl;
  flat_tree->Scan(pattern.data(),cutpattern.data());
};
