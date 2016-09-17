// FNameManager.cpp
// 
// Jeromy Tompkins
// 7/21/2011
//
// 

#include <iostream>
#include <string>
#include <fstream>
#include "FNameManager.h"

ClassImp(FNameManager)

        // FNameManager::FNameManager ()
        //   : fBaseDir(), fSclrDir(), fRunTreeDir(),
        //   fHistFileDir(), fSclrFile(), fHistFileBase(),
        //   fAnalDir()
        // {
        //   std::string tmp;
        //   std::cout
        //     << "Enter the name of the file with the configuration defined"
        //     << std::endl;
        //   std::cin >> tmp;

        //   LoadConfig(tmp);
        // }

        FNameManager::FNameManager (const Char_t* configfile)
            : fBaseDir(), fSclrDir(), fRunTreeDir(),
            fHistFileDir(), fSclrFile(), fHistFileBase(),
            fAnalDir(), fConfigFile(), fResultsDir()
{
    LoadConfig(std::string()=configfile);
}

FNameManager::~FNameManager ()
{}

FNameManager::FNameManager (FNameManager const& obj)
    : fBaseDir(obj.fBaseDir), fSclrDir(obj.fSclrDir),
    fRunTreeDir(obj.fRunTreeDir), fHistFileDir(obj.fHistFileDir), 
    fSclrFile(obj.fSclrFile), fHistFileBase(obj.fHistFileBase),
    fAnalDir(obj.fAnalDir), fConfigFile(obj.fConfigFile),
    fResultsDir(obj.fResultsDir)
{}

FNameManager& FNameManager::operator=(FNameManager const& obj)
                                     {
    if (this!=&obj)
    {
        fBaseDir      = obj.fBaseDir;
        fSclrDir      = obj.fSclrDir;
        fRunTreeDir   = obj.fRunTreeDir;
        fHistFileDir  = obj.fHistFileDir;
        fSclrFile     = obj.fSclrFile;
        fHistFileBase = obj.fHistFileBase;
        fAnalDir      = obj.fAnalDir;
        fConfigFile   = obj.fConfigFile;
        fResultsDir   = obj.fResultsDir;
    }
    return *this;
}


std::string FNameManager::GetBaseDir(void)     const
{
    return fBaseDir;
}

std::string FNameManager::GetSclrDir(Bool_t full_path)     const
{
    if (full_path)
        return fBaseDir+"/"+fSclrDir;
    else
        return fSclrDir;
}

std::string FNameManager::GetRunTreeDir(Bool_t full_path)  const
{
    if (full_path)
        return fBaseDir+"/"+fRunTreeDir;
    else
        return fRunTreeDir;
}

std::string FNameManager::GetHistFileDir(Bool_t full_path) const
{
    if (full_path)
        return fBaseDir+"/"+fHistFileDir;
    else
        return fHistFileDir;
}

std::string FNameManager::GetSclrFileName(Bool_t full_path) const
{
    if (full_path)
        return fBaseDir+"/"+fSclrDir+"/"+fSclrFile;
    else
        return fSclrFile;
}

std::string FNameManager::GetHistFileBase(Bool_t full_path) const
{
    if (full_path)
        return GetHistFileDir()+"/"+fHistFileBase;
    else
        return fHistFileBase;
}

std::string FNameManager::GetAnalysisDir(Bool_t full_path) const
{
    if (full_path)
        return fBaseDir+"/"+fAnalDir;
    else
        return fAnalDir;
}

std::string FNameManager::GetConfigFileName(Bool_t full_path) const
{
    if (full_path)
        return fBaseDir+"/"+fAnalDir+"/"+fConfigFile;
    else
        return fConfigFile;
}

std::string FNameManager::GetResultsDir(Bool_t full_path) const
{
    if (full_path)
        return fBaseDir+"/"+fAnalDir+"/"+fResultsDir;
    else
        return fResultsDir;
}

void FNameManager::SetBaseDir(std::string const& name)
{
    fBaseDir = name;
}

void FNameManager::SetSclrDir(std::string const& name)
{
    fSclrDir = name;
}

void FNameManager::SetRunTreeDir(std::string const& name)
{
    fRunTreeDir = name;
}

void FNameManager::SetHistFileDir(std::string const& name)
{
    fHistFileDir = name;
}

void FNameManager::SetSclrFileName(std::string const& name)
{
    fSclrFile = name;
}

void FNameManager::SetHistFileBase(std::string const& name)
{
    fHistFileBase = name;
}

void FNameManager::SetAnalysisDir(std::string const& name)
{
    fAnalDir = name;
}

void FNameManager::SetConfigFileName(std::string const& name)
{
    fConfigFile = name;
}

void FNameManager::SetResultsDir(std::string const& name)
{
    fResultsDir = name;
}

void FNameManager::LoadConfig(std::string const& name)
{
    std::string tmp;
    std::ifstream f(name.data(), std::ifstream::in);
    if (!f.is_open())
    {
        std::cout
                << "Enter the name of the file with the configuration defined"
                << std::endl;
        std::cin >> tmp;
        f.open(tmp.data());
    }

    if (ConfigFileIsGood(f))
    {
        getline(f,tmp);
        SetBaseDir(ExtractFName(tmp));
        getline(f,tmp);
        SetSclrDir(ExtractFName(tmp));
        getline(f,tmp);
        SetRunTreeDir(ExtractFName(tmp));
        getline(f,tmp);
        SetHistFileDir(ExtractFName(tmp));
        getline(f,tmp);
        SetSclrFileName(ExtractFName(tmp));
        getline(f,tmp);
        SetHistFileBase(ExtractFName(tmp));
        getline(f,tmp);
        SetAnalysisDir(ExtractFName(tmp));
        getline(f,tmp);
        SetConfigFileName(ExtractFName(tmp));
        getline(f,tmp);
        SetResultsDir(ExtractFName(tmp));
    }
    f.close();
}

Bool_t FNameManager::ConfigFileIsGood(std::ifstream& f) const
{
    Bool_t returnval=true;
    if (!f.is_open())
    {
        std::cout << "Config file doesn't exist" << std::endl;
        returnval=false;
    }
    else if (f.fail())
    {
        std::cout << "Warning! Filestream failbit set" << std::endl;
        returnval=false;
    }
    else if (f.bad())
    {
        std::cout << "Warning! Filestream badbit set" << std::endl;
        returnval=false;
    }
    else if (f.eof())
    {
        std::cout << "Warning! Filestream end of file reached." << std::endl;
        returnval=false;
    }
    return returnval;
}

std::string FNameManager::ExtractFName(std::string const& text) const
{
    // This looks for a semicolon (:) separator between the id
    // and the filename

    std::string tmp = text;
    size_t in = text.find(':',0);
    if (in==std::string::npos) std::cout << "Warning! No descriptor found in line" << std::endl;
    else
    {
        tmp = text.substr(in+1,text.length()-in-1);
        // trim both ends
        in = tmp.find_first_not_of(' ');
        tmp = tmp.substr(in,tmp.length()-in);
        in = tmp.find_last_not_of(" \n");
        tmp = tmp.substr(0,in+1);
    }

    return tmp;
}
