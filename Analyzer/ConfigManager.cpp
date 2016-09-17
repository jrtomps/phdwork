// ConfigManager.cpp
//
// Author : Jeromy Tompkins
// Date   : 8/23/2010
//
// Descrip: To manage the loading and saving of the following:
//          - gflash
//          - adc_edge
//          - thresh
//          - det_dist
//          - ped
//          - deut_en
//          - angle

#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include "ConfigEntry.h"
#include "TObject.h"
#include "ConfigManager.h"

using namespace std;

ConfigManager::ConfigManager () 
    : fDb(),
    listofruns(),
    fFName(""),
    fComment(""),
    fSpacer(' ')
{};


// Should be deprecated
ConfigManager::ConfigManager (Int_t ndets) 
    : fNDetectors(ndets),
    fDb(),
    listofruns(),
    fFName(""),
    fComment(""),
    fSpacer(' ')
{ //  cout << fDb.size() << endl;
};

ConfigManager::~ConfigManager (void)
{
#ifdef DEBUG
    cout << "Begin ConfigManager destructor" << endl; cout.flush();
#endif
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        delete fDb[i];
    }
};

ConfigManager::ConfigManager(const ConfigManager& obj) 
    : fNDetectors(obj.fNDetectors),
    fDb(obj.fDb),
    listofruns(obj.listofruns),
    fFName(obj.fFName),
    fComment(obj.fComment),
    fSpacer(obj.fSpacer)
{
    if (this != &obj)
    {
        // fDb = obj.fDb;
        // listofruns = obj.listofruns;
        // fFName = obj.fFName;
        // fComment = obj.fComment;
        // fSpacer = obj.fSpacer;
    }
}

ConfigManager& ConfigManager::operator=(ConfigManager const& obj)
                                       {
    if (this != &obj)
    {
        fNDetectors = obj.fNDetectors;
        fDb = obj.fDb;
        listofruns = obj.listofruns;
        fFName = obj.fFName;
        fComment = obj.fComment;
        fSpacer = obj.fSpacer;
    }
    return *this;
}

ConfigEntry* ConfigManager::GetRun(const Int_t runnum) const
{
    bool stillsearching = true;
    unsigned int i=0;
    while (stillsearching && i<fDb.size())
    {
        if (fDb[i]->GetRun() == runnum)
        {
            return fDb[i];
        }
        else
            i++;
    }

    //  cout << "Could not find " << runnum << endl;

    return NULL;

}

Bool_t ConfigManager::RunIsStoredInDb(const Int_t runnum)
{
    ConfigEntry *ce = GetRun(runnum);
    if (ce != 0) return kTRUE;
    else  return kFALSE;

}

void ConfigManager::PrintTargetRuns(const Char_t* tarname)
{
    vector<Int_t> runs;
    runs = GetListOfTargetRuns(tarname);

    if (runs.size()==0)
        cout
                << "No runs exist in database for target "
                << tarname
                << endl;
    else
    {
        cout
                << tarname << " runs \n"
                << "-----------------"
                << endl;

        for (UInt_t i=0; i<runs.size(); i++)
        {
            cout << runs.at(i) << endl;
        }
    }
}

string ConfigManager::GetComment(void)
{ return fComment; }

const string  ConfigManager::GetComment(void) const
{ return fComment; }

vector<Int_t> ConfigManager::GetListOfRuns(void)
{

    listofruns.clear();

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        listofruns.push_back(fDb.at(i)->GetRun());
    }
    return listofruns;

}

vector<Int_t> ConfigManager::GetListOfTargetRuns(const Char_t *targetname)
{

    string tarstr(targetname);
    listofruns.clear();

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        const Char_t *tarintree = fDb.at(i)->GetTarget();
        if (tarstr.compare(tarintree) == 0)
        {
            listofruns.push_back(fDb.at(i)->GetRun());
        }
    }
    return listofruns;

};

vector<Int_t> ConfigManager::GetListOfTargetRunsWithEnergy(const Char_t *targetname, const Float_t energy)
{

    string tarstr(targetname);
    listofruns.clear();

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        const Char_t *tarintree = fDb.at(i)->GetTarget();
        if (tarstr.compare(tarintree) == 0
        && fabs(fDb.at(i)->GetEnergy()-energy)<0.0001)
        {
            listofruns.push_back(fDb.at(i)->GetRun());
        }
    }
    return listofruns;

};

vector<Int_t> ConfigManager::GetListOfTargetRunsWithEnergyAndPol(const Char_t *targetname, const Float_t energy, const Char_t* pol)
{

    string tarstr(targetname);
    string polstr(pol);
    listofruns.clear();

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        const Char_t *tarintree = fDb.at(i)->GetTarget();
        const Char_t *polintree = fDb.at(i)->GetPol();
        if (tarstr.compare(tarintree) == 0
        && fabs(fDb.at(i)->GetEnergy()-energy)<0.0001
        && polstr.compare(polintree)==0)
        {
            listofruns.push_back(fDb.at(i)->GetRun());
        }
    }
    return listofruns;
}

vector<ConfigEntry*> ConfigManager::GetListOfEntries(void)
{

    vector<ConfigEntry*> avec;

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        avec.push_back(fDb.at(i));
    }
    return avec;

}

vector<ConfigEntry*> ConfigManager::GetListOfTargetEntries(const Char_t *targetname)
{

    string tarstr(targetname);
    vector<ConfigEntry*> avec;

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        const Char_t *tarintree = fDb.at(i)->GetTarget();
        if (tarstr.compare(tarintree) == 0)
        {
            avec.push_back(fDb.at(i));
        }
    }
    return avec;

};

vector<ConfigEntry*> ConfigManager::GetListOfTargetEntriesWithEnergy(const Char_t *targetname, const Float_t energy)
{

    string tarstr(targetname);
    vector<ConfigEntry*> avec;

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        const Char_t *tarintree = fDb.at(i)->GetTarget();
        if (tarstr.compare(tarintree) == 0
        && fabs(fDb.at(i)->GetEnergy()-energy)<0.0001)
        {
            avec.push_back(fDb.at(i));
        }
    }
    return avec;

};

vector<ConfigEntry*> ConfigManager::GetListOfTargetEntriesWithEnergyAndPol(const Char_t *targetname, const Float_t energy, const Char_t* pol)
{

    string tarstr(targetname);
    string polstr(pol);
    vector<ConfigEntry*> avec;

    //  cout << "The following runs match" << endl;
    for (UInt_t i=0; i<fDb.size(); i++)
    {
        const Char_t *tarintree = fDb.at(i)->GetTarget();
        const Char_t *polintree = fDb.at(i)->GetPol();
        if (tarstr.compare(tarintree) == 0
        && fabs(fDb.at(i)->GetEnergy()-energy)<0.0001
        && polstr.compare(polintree)==0)
        {
            avec.push_back(fDb.at(i));
        }
    }
    return avec;
}


void ConfigManager::LoadDbFromFile(const string fname)
{
    if (fDb.size()!=0)
    {
        if (UserWantsToReload())
        {
            fDb.clear();
        }
    }

    ifstream ifile (fname.data(), ifstream::in);

    fFName = fname;

    //  cout << "Start reading " << fFName << endl;
    if (ifile.is_open())
    {
        // Ignore the column names and headings
        ifile >> ws;
        getline(ifile, fComment);
        cout << fComment << endl;

        if (ifile.good())
        {
            ConfigEntry *cc;
            while(1)
        {

                if (ifile.eof())
        {
#ifdef DEBUG
                    cout << "End of file reached" << endl; cout.flush();
#endif
                    //		  delete cc;
                    break;
        }
                if (ifile.bad())
        {
#ifdef DEBUG	
                    cout << "Unexpected termination reading from " << fname << endl;
#endif
                    //		  delete cc;
                    break;
        }

                cc = new ConfigEntry(ifile);

                if (cc==0)
            {
                    cout << "ConfigEntry failed to construct" << endl;
                    cout.flush();
                    break;
            }
                else
                    AddEntry(cc);
        }

        }
    }
    ifile.close();
    //   cout << "file closed" << endl; cout.flush();
}

void ConfigManager::PrintRun(const Int_t runnumber)
{
    ConfigEntry *ce = GetRun(runnumber);

    if (ce)
        ce->PrintRunInfo();
    else
        cout << "Run " << runnumber << " does not exist" << endl;
}

void ConfigManager::ListAllRuns() const
{
    Int_t wint=5, wstr=16, wfloat=10;
    vector<ConfigEntry*>::const_iterator it;

    cout << left;
    cout << setw(wint)   << "Run"
            << setw(wstr)   << "Target"
            << setw(6) << "Energy"
            << setw(6)   << "Pol"
            << endl;

    for (it=fDb.begin(); it!=fDb.end(); it++)
    {
        (*it)->PrintRunInfo(wint,wstr,wfloat);
    }
}

void ConfigManager::ListRuns(const Int_t first, const Int_t last) const
{
    Int_t wint=5, wstr=16, wfloat=10;

    cout << left;
    cout << setw(wint)   << "Run"
            << setw(wstr)   << "Target"
            << setw(6) << "Energy"
            << setw(6)   << "Pol"
            << endl;

    ConfigEntry *ce;
    for (Int_t run=first; run<=last; run++)
    {
        ce = GetRun(run);
        if (ce!=NULL)
            ce->PrintRunInfo(wint,wstr,wfloat);
        else
            continue;
    }
}

void ConfigManager::PrintRunInfo(ConfigEntry* ce)
{  ce->PrintRunInfo();}

void ConfigManager::ListAllTargetRuns(const Char_t *targetname)
{
    vector<ConfigEntry*> vec = GetListOfTargetEntries(targetname);
    for_each(vec.begin(), vec.end(), PrintRunInfo);
}

void ConfigManager::ListAllTargetRunsWithEnergy(const Char_t *targetname, const Float_t energy)
{
    vector<ConfigEntry*> vec = GetListOfTargetEntriesWithEnergy(targetname,energy);
    for_each(vec.begin(), vec.end(), PrintRunInfo);
}

void ConfigManager::ListAllTargetRunsWithEnergyAndPol(const Char_t *targetname, const Float_t energy, const Char_t *pol)
{
    vector<ConfigEntry*> vec = GetListOfTargetEntriesWithEnergyAndPol(targetname,energy,pol);
    for_each(vec.begin(), vec.end(), PrintRunInfo);
}

void ConfigManager::AddNewEntry(const Int_t runnum)//   const Float_t en,
        //				   const Char_t *target, const Char_t *pol)
{

    ConfigEntry *ce = 0;


    if ( FindRun(runnum) != fDb.end() ) // run already exists
    {
        cout << "Entry for " << runnum << " already exists "
        << "\nand has not been edited!"
        << endl;
    }
    else if(fDb.size()==0) // run doesn't exist and no runs to copy
    {
        Char_t answer;
        cout << "Warning! No entries exist in the database to serve as a template" << endl;
        cout << "1. Do you want to set one up? (y/n)" << endl;
        cin  >> answer;
        if (answer=='y' || answer=='Y')
    {
            cout << "Enter description of database: " << endl;
            cin >> ws;
            getline(cin, fComment);

            ce = new ConfigEntry();
            ce->InteractivelyConstructDbEntry(runnum);
            AddEntry(ce);
            SaveAs(fFName.data(), fComment.data(), fSpacer);
    }
        else
            cout << "Nothing has been added to the database!" << endl;
    }
    else // run does't exist but there is a run to copy
    {
        //cout << "Creating new entry" << endl; cout.flush();

        ce = new ConfigEntry(*(fDb.back()));
        ce->InteractivelySetupRunInfo(runnum, false);
        //      ce->SetRun(runnum);
        //      ce->SetEnergy(en);
        //      if (strlen(target)>0) ce->SetTarget(target);
        //      if (strlen(pol)>0) ce->SetPol(pol);

        AddEntry (ce);

        SaveAs(fFName.data(), fComment.data(), fSpacer);
    }

}

vector<ConfigEntry*>::iterator ConfigManager::FindRun(const Int_t runnum)
{
    vector<ConfigEntry*>::iterator it;
    for ( it=fDb.begin(); it!=fDb.end(); ++it)
    {
        if ((*it)->GetRun() == runnum)
            break;
    }

    return it;
}


void ConfigManager::AddEntry(ConfigEntry* ce)
{
#ifdef DEBUG
    cout << "Begin adding Entry "; cout.flush();
#endif
    vector<ConfigEntry*>::iterator it;

    Int_t run = ce->GetRun();

    // Find the appropriate place to insert the new entry
    for(it=fDb.begin(); it!=fDb.end(); it++)
        if (run <= (*it)->GetRun()) break;

    // Run should be added at the end of the database.
    if (it == fDb.end())
        fDb.push_back(ce);
    else  // Run either exists already or needs insertion
    {

        // Run already exists
        if ( run == (*it)->GetRun() )
            cout << "Run already exists" << endl;
        else // Run needs to be inserted
    {
            cout << "New entry before run " << (*it)->GetRun() << endl;
            fDb.insert(it,ce);
    }
    }

    //  SaveAs(fFName, fComment, fSpacer);
}

void ConfigManager::RemoveEntry(const Int_t runnum)
{
#ifdef DEBUG
    cout << "Begin removing entry "; cout.flush();
#endif
    vector<ConfigEntry*>::iterator it;

    // Find the appropriate place to insert the new entry
    for(it=fDb.begin(); it!=fDb.end(); it++)
        if (runnum == (*it)->GetRun()) break;

    // Run should be added at the end of the database.
    if (it == fDb.end())
    {
        cout << "Couldn't find run " << runnum << endl;
        return;
    }
    else  // Run either exists already or needs insertion
    {
        fDb.erase(it);
        SaveAs(fFName.data(), fComment.data(), fSpacer);
    }

}

void ConfigManager::Save ()
{
    if (fDb.size()==0)
    {
        cout << "No database loaded." <<endl;
        return;
    }

    ofstream ofile (fFName.data(), ofstream::out);

    ofile << fComment << endl;

    for (UInt_t i=0; i<fDb.size(); i++)
    {
        fDb.at(i)->WriteToFile(ofile,fSpacer);
    }

    ofile.close();

};

void ConfigManager::SaveAs (const Char_t* fname, const Char_t* /*comment*/, Char_t spacer)
{

    if (fDb.size()==0)
    {
        cout << "No database loaded." <<endl;
        return;
    }

    fSpacer = spacer;

    ofstream ofile (fname, ofstream::out);

    ofile << fComment << endl;

    for (UInt_t i=0; i<fDb.size(); i++)
    {
        fDb.at(i)->WriteToFile(ofile,fSpacer);
    }

    ofile.close();

};

void ConfigManager::SetArrayForRuns(const Char_t *id, Float_t *array, Int_t firstrun, Int_t lastrun)
{
    Int_t run = firstrun;
    ConfigEntry *entry;

    vector<Float_t> vec;
    for (Int_t i=0; i<fNDetectors; i++)
    {
        vec.push_back(*(array+i));
    }

    while (run <= lastrun)
    {
        entry = GetRun(run);
        if (entry) entry->SetMember(id, vec);
        else cout << "bad run = " << run << endl;
        run++;
    }
}

void ConfigManager::SetArrayForRuns(const Char_t *id, Float_t *array, vector<Int_t>& runlist)
{
    ConfigEntry *entry;
    vector<Float_t> vec;
    for (Int_t i=0; i<fNDetectors; i++)
    {
        vec.push_back(*(array+i));
    }

    for (UInt_t i=0; i<runlist.size(); i++)
    {
        entry = GetRun(runlist[i]);
        if (entry) entry->SetMember(id, vec);
    }
}  

void ConfigManager::SetValueForRuns(const Char_t *id, Float_t val, Int_t firstrun, Int_t lastrun)
{
    Int_t run = firstrun;
    ConfigEntry *entry;

    vector<Float_t> vec;
    vec.push_back(val);

    while (run <= lastrun)
    {
        entry = GetRun(run);
        if (entry) entry->SetMember(id, vec);
        run++;
    }
}

void ConfigManager::SetValueForRuns(const Char_t *id, Float_t val, vector<Int_t>& runlist)
{
    ConfigEntry *entry;

    vector<Float_t> vec;
    vec.push_back(val);

    for (UInt_t i=0; i<runlist.size(); i++)
    {
        entry = GetRun(runlist[i]);
        if (entry) entry->SetMember(id, vec);
    }
}

void ConfigManager::SetValueForRuns(const Char_t *id, const Char_t* name, Int_t firstrun, Int_t lastrun)
{
    Int_t run = firstrun;
    ConfigEntry *entry;

    while (run <= lastrun)
    {
        entry = GetRun(run);
        if (entry) entry->SetMember(id, name);
        run++;
    }
}

void ConfigManager::SetValueForRuns(const Char_t *id, const Char_t* name, vector<Int_t>& runlist)
{
    ConfigEntry *entry;

    for (UInt_t i=0; i<runlist.size(); i++)
    {
        entry = GetRun(runlist[i]);
        if (entry) entry->SetMember(id, name);
    }
}

void ConfigManager::SetExtensibleDbForRuns(const Int_t firstrun, const Int_t lastrun,
                                           const ExtensibleDb* edb)
{
    Int_t run = firstrun;
    ConfigEntry *entry;

    while (run <= lastrun)
    {
        entry = GetRun(run);
        if (entry) entry->SetExtensibleDb(edb);
        run++;
    }
}

void ConfigManager::SetExtensibleDbForRuns(vector<Int_t>& runlist,
                                           const ExtensibleDb* edb)
{
    ConfigEntry *entry;
    vector<Int_t>::iterator it;

    for (it = runlist.begin(); it!= runlist.end(); it++)
    {
        entry = GetRun(*it);
        if (entry) entry->SetExtensibleDb(edb);
    }
}

void ConfigManager::AddDataElementForRuns(const Int_t firstrun, const Int_t lastrun,
                                          DataElement* de)
{
    Int_t run = firstrun;
    ConfigEntry *entry;

    while (run <= lastrun)
    {
        entry = GetRun(run);
        if (entry)
    {
            UInt_t original_size = entry->GetExtensibleDb()->size();
            entry->GetExtensibleDb()->insert(pair<string,DataElement*>(de->GetHeader(), new DataElement(*de)));
            if (original_size == entry->GetExtensibleDb()->size() )
        {
                cout << "DataElement already exists with this header." << endl;
                cout << "Do you want to overwrite? (y/n) : ";
                Char_t ans;
                cin  >> ans;
                if (ans == 'y' || ans=='Y')
        {
                    entry->GetExtensibleDb()->erase(de->GetHeader());
                    entry->GetExtensibleDb()->insert(pair<string, DataElement*>(de->GetHeader(),de));
        }
        }
    }
        run++;
    }
}

void ConfigManager::AddDataElementForRuns(vector<Int_t>& runlist,
                                          DataElement* de)
{
    ConfigEntry *entry;
    vector<Int_t>::iterator it;

    for (it = runlist.begin(); it!= runlist.end(); it++)
    {
        entry = GetRun(*it);
        if (entry)
    {
            UInt_t original_size = entry->GetExtensibleDb()->size();
            entry->GetExtensibleDb()->insert(pair<string,DataElement*>(de->GetHeader(), new DataElement(*de)));
            if (original_size == entry->GetExtensibleDb()->size() )
        {
                cout << "DataElement already exists with this header." << endl;
                cout << "Do you want to overwrite? (y/n) : ";
                Char_t ans;
                cin  >> ans;
                if (ans == 'y' || ans=='Y')
        {
                    entry->GetExtensibleDb()->erase(de->GetHeader());
                    entry->GetExtensibleDb()->insert(pair<string, DataElement*>(de->GetHeader(),de));
        }
        }
    }
    }
}

const vector<ConfigEntry*> ConfigManager::GetDb() const
{return fDb;}

Bool_t ConfigManager::UserWantsToReload(void)
{
    Bool_t choice = false;
    Char_t answer;
    cout << "Data has already been loaded into database."
            << "\nDo you want to clear it and then reload? (y/n)";
    cin >> answer;
    if (answer=='y' || answer=='Y')
        choice = true;

    return choice;
}

