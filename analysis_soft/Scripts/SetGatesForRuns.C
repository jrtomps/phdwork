#include <string>
#include <iostream>
#include <iomanip>
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "DataElement.h"
#include "ExtensibleDb.h"
#include "FNameManager.h"
#include "ReadGatesFromFile.C"
using namespace std;

string PromptForString(const string& prompt)
{
    string cfname;
    cout << left
            << setw(30) << prompt << " : ";
    cin  >> cfname;
    return cfname;
}

UInt_t PromptForUInt_t(const string& prompt)
{
    UInt_t val;
    cout << left
            << setw(30) << prompt << " : ";
    cin  >> val;
    return val;
}


void SetGatesForRuns(Bool_t ask_for_names=true, string lo_name="", string hi_name="")
{
    string cfname  = PromptForString("Enter name of cut file");
    if (ask_for_names)
    {
        lo_name = PromptForString("Enter name of column 1");
        hi_name = PromptForString("Enter name for column 2");
    }

    vector<Float_t> lo_gates;
    vector<Float_t> hi_gates;

//    gROOT->ProcessLine(".L ReadGatesFromFile.C+");
    ReadFloatGatesFromFile(cfname.data(),lo_gates, hi_gates);

    DataElement *de_lo = DataElement::Construct(lo_name,lo_gates);
    DataElement *de_hi = DataElement::Construct(hi_name,hi_gates);

    cout << "End of gate loading";
    cout << "\n\nBegin insertion routine" << endl;

    UInt_t first_run, last_run;
    first_run = PromptForUInt_t("Enter first run");
    last_run = PromptForUInt_t("Enter last run");

    FNameManager fnm("fnameconfig.dat");
    string dbfname = fnm.GetConfigFileName(true);
//    string dbfname("/home/jrtomps/pffrag2/analysis/configdatabase.dat");
    ConfigManager cm;
    cm.LoadDbFromFile(dbfname.data());
    cout << "Found " << cm.GetDb().size() << " entries." << endl;
    ConfigEntry *ce;
    for (UInt_t run=first_run; run<=last_run; run++)
    {
        ce = cm.GetRun(run);
        if (ce==NULL) continue;
        else
        {
            ce->GetExtensibleDb()->AddDataElement(de_lo);
            ce->GetExtensibleDb()->AddDataElement(de_hi);
        }
    }

    string response = PromptForString("Do you want to save? (y/n)");
    if (response[0]=='y' || response[0]=='Y')
    {
        cm.Save();
    }

    delete de_lo;
    delete de_hi;
}
