#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <dirent.h>
#include "Rtypes.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCutG.h"
#include "TObject.h"
#include "TGraph.h"
#include "TApplication.h"

void ListAllFilesInDirOfType(std::string d, std::string suffix)
{
    std::cout << "\n";
    ::DIR* dp = ::opendir(d.data());
    ::dirent * dir = 0;
    while ( (dir = ::readdir(dp)) )
    {
        std::string name(dir->d_name);
        if (name.rfind(suffix)!=std::string::npos)
        {
            std::cout << "\n\t" << name;
        }
    }
    std::cout << std::endl;

    ::closedir(dp);
}

TCutG* GetBoundary(TFile& f, UInt_t index)
{
    TCutG* bndry = 0;
    f.cd();

    TObject *o;
    TDirectory *dir = f.GetDirectory("BoundaryCreatorOutput");
    if (dir!=0)
    {
        std::ostringstream bndry_name("Boundary",std::ios::out|std::ios::app);
        bndry_name << index;
        o = dir->Get(bndry_name.str().data());
        if (o)
        {
            bndry = dynamic_cast<TCutG*>(o->Clone());
        }

    }
    return bndry;
}

TGraph* GetStripGraph(TFile& f, UInt_t index)
{
    TGraph* graph = 0;
    f.cd();

    TObject *o;
    TDirectory *dir = f.GetDirectory("SiStripGeneratorOutput");
    if (dir!=0)
    {
        std::ostringstream graph_name("det",std::ios::out|std::ios::app);
        graph_name << index/16 << "_strip" << index%16;
        o = dir->Get(graph_name.str().data());
        if (o)
        {
            graph = dynamic_cast<TGraph*>(o->Clone());
        }
    }
    return graph;
}


Int_t CompareStripBoundaryToStripData()
{

    std::string fname;
    std::cout << "Choose one of the following files :";
    ListAllFilesInDirOfType(".",".root");
    std::cout << "\nEnter filename : ";
    if (std::cin.peek()=='\n') std::cin.get();
    std::getline(std::cin, fname);

    UInt_t index;
    std::cout << "\nEnter index of strip : ";
    std::cin >> index;

    TFile *f = new TFile(fname.data());
    TCutG* boundary = GetBoundary(*f, index);
    TGraph* graph = GetStripGraph(*f, index);


    if (graph==0)
    {
        std::cerr << "Graph doesn't exist" << std::endl;
    }
    else if (boundary==0)
    {
        std::cerr << "boundary doesn't exist" << std::endl;
    }
    else
    {
        graph->Draw("AP");
        boundary->SetLineWidth(2);
        boundary->SetLineColor(kOrange);
        boundary->SetMarkerStyle(20);
        boundary->SetMarkerColor(kOrange+2);
        boundary->Draw("PL");
    }

    return 0;
}
