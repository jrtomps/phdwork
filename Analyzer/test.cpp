
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "ExtensibleDb.h"
#include "Rtypes.h"
using namespace std;

void
Print(Float_t val)
{
	cout << val << endl;
}

int
main()
{
  ConfigManager cm;
  cm.LoadDbFromFile("../../configdatabase.dat");

  ExtensibleDb *edb = cm.GetRun(684)->GetExtensibleDb();
//  edb->Write(std::cout);

  vector<Int_t> myvec;
  cout << "Printing UpperIntBound values" << endl;
  edb->GetValues("UpperIntBound",myvec);

  for_each(myvec.begin(), myvec.end(), Print);  

  vector<Float_t> myvec2;
  cout << "Printing LowCut values" << endl;
  edb->GetValues("LowCut",myvec2);

  for_each(myvec2.begin(), myvec2.end(), Print);  

//  cm.ListAllRuns();	
  return 0;
}
