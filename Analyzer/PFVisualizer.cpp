// PFVisualizer.cpp
//
// Jeromy Tompkins
// 6/15/2011
//

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include "TROOT.h"
#include "TCanvas.h"
#include "TSeqCollection.h"
#include "TObject.h"
#include "TString.h"
#include "TH1.h"
#include "TFile.h"
#include "TPRegexp.h"
#include "TDirectory.h"
#include "PFVisualizer.h"

ClassImp(PFVisualizer)

        PFVisualizer::PFVisualizer  (const Int_t run)
            : fIsGoodCh(), fRun(run), Visualizer()
{
    fFileName = GuessBase();

    SetupFileName();

    OpenFile();

    LoadChannelInfo();

}

PFVisualizer::PFVisualizer  (const Char_t *fname, const Int_t run)
    : fIsGoodCh(), fRun(run), fFileName(fname), Visualizer()
{
    //  SetupFileName();
    fFileName = TString::Format("%s%i.root",fname,run);
    OpenFile();
}

PFVisualizer::~PFVisualizer (void)
{}

PFVisualizer::PFVisualizer  (PFVisualizer const& obj)
{
    if (this != &obj)
    {
        fFileName = obj.fFileName;
        fFile     = obj.fFile;
        fRun      = obj.fRun;

        fIsGoodCh = obj.fIsGoodCh;
        //  The file is already open. Nothing left to do.
        //      OpenFile();
    }
}

PFVisualizer& PFVisualizer::operator=(PFVisualizer const& obj)
                                     {
    if (this != &obj)
    {
        fFileName = obj.fFileName;
        fFile     = obj.fFile;
        fRun      = obj.fRun;
        fIsGoodCh = obj.fIsGoodCh;
    }
    return *this;
}

TString PFVisualizer::GuessBase()
{
    // modified code from
    // http://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/
    TString rstr;
    TString dir(".");
    std::vector<TString> files = GetDir(dir);

    Int_t nmatches        = 0;
    Int_t min_nmatches    = 0;
    Int_t min_match_score = 5;

    TPRegexp pre("[0-9]+.root$");
    TPRegexp pre2("[0-9]+to[0-9]+.root$");
    Ssiz_t in;

    std::vector<TString>::iterator it1;
    for (it1=files.begin(); it1!=files.end(); ++it1)
    {
        in = (*it1).Index(pre);
        if (in != -1)
	{
            if ((*it1).Index(pre2) != -1)
                continue;
            else
	    {
                rstr = (*it1)(0,in);
                break;
	    }
	}
        // nmatches = 0;
        // for (it2=files.begin(); it2!=files.end(); ++it2)
        //   {
        // 	if ( (*it1).CompareTo((*it2)) == 0 ) continue;
        // 	else
        // 	  {
        // 	    if ( (*it1).CompareTo((*it2)) >= min_match_score )
        // 	      nmatches++;

        // 	    if (nmatches >= min_nmatches) break;
        // 	  }
        //   }
    }
    return rstr;
}


std::vector<TString> PFVisualizer::GetDir(TString& dir)
{
    struct dirent *dirp;
    std::vector<TString> files = std::vector<TString>();

    DIR *dp  = opendir(dir.Data());

    if (dp == NULL)
    {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
    }
    else
    {
        while ((dirp = readdir(dp)) != NULL)
        {
            files.push_back(TString(dirp->d_name));
        }
        closedir(dp);
    }

    return files;
}


void PFVisualizer::SetupFileName()
{
    Char_t resp;
    fFileName += fRun;
    fFileName += ".root";
    std::cout << "Is the file of interest named " << fFileName.Data() << "? (y/n)" << std::endl;
    std::cin >> resp;

    if (resp == 'y' || resp == 'Y')
    {
        return;
    }
    else
    {
        Char_t file[256];
        std::cout << "Enter file name" << std::endl;
        std::cin >> file;
        fFileName = file;
        return;
    }

}

void PFVisualizer::OpenFile()
{
    fFile = new TFile (fFileName.Data(), "READ");
}

void PFVisualizer::CloseFile()
{
    TSeqCollection *sc = gROOT->GetListOfFiles();
    TObject *obj = sc->FindObject(fFileName.Data());

    if (obj)
    {
        dynamic_cast<TFile*>(obj)->Close();
        delete obj;
    }
    else
    {
        std::cout << "Couldn't find " << fFileName.Data() << std::endl;
    }
}

void PFVisualizer::LoadChannelInfo()
{
    std::ifstream cifile("channelinfo.dat",std::ifstream::in);

    if(!cifile.is_open())
    {
        std::cout << "Error: channelinfo.dat does not exist" << std::endl;
    }


    cifile.close();
}

void PFVisualizer::Draw(const TString &basename, TString options) const
{
    fFile->cd();
    Visualizer::Draw(basename,options);
}

void PFVisualizer::Draw(const TString &basename, const Int_t low_index,
                        const Int_t hi_index, TString options) const
{
    fFile->cd();
    Visualizer::Draw(basename,low_index,hi_index,options);
}
