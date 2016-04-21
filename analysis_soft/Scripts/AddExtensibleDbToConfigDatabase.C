#include <iostream>
#include <iomanip>
#include <string>
#ifndef __CINT__
#include "ExtensibleDb.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#endif
using namespace std;

void
AddExtensibleDbToConfigDatabase(void)
{
  string str;
  ConfigManager cm;
  cout << "Enter name of config database : ";
  getline(cin,str,'\n');
  if (str.empty()) str = "configdatabase.dat";
  
  cm.LoadDbFromFile(str.data());

  cm.ListAllRuns();
  
  Int_t r0, r1;
  Char_t ans='y';
  while (ans=='y' || ans=='Y')
    {
      DataElement de;
      cout << "Enter first run : ";
      cin  >> r0;
      cout << "Enter last run  : ";
      cin  >> r1;
      cm.AddDataElementForRuns(r0, r1, &de);
      cout << "Continue? (y/n) : ";
      cin  >> ans;
      
    }

  Int_t index;
  cout << "Save database? : " << endl;
  cout << "0) No " << endl;
  cout << "1) Save" << endl;
  cout << "2) Save As" << endl;
  cin  >> index;
  if (index==1)
    cm.Save();
  else if (index==2)
    {
       cout << "Enter name : ";
       string str_answer;
       cin  >> str_answer;
       cm.SaveAs(str_answer.data());
    }
   else
     cout << "Exiting without saving work." << endl;

}
