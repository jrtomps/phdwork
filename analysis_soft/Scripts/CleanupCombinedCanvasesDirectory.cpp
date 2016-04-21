#include <iostream>
#include "TFile.h"
#include "TDirectory.h"

void CleanupCombinedCanvasesDirectory(TFile* f)
{
    if (f==0)
    {
        std::cerr << "NULL file argument!" << std::endl;
        return;
    }

    TDirectory *dir = f->GetDirectory("canvases");
    if (dir!=NULL)
    {
        dir->Delete("*;*");
    }

}

void CleanupCombinedCanvasesDirectory(TString fname)
{
    TFile *f = new TFile(fname,"UPDATE");
    if (!f->IsOpen())
    {
        std::cerr << fname << " doesn't exist!" << std::endl;
        return;
    }

    CleanupCombinedCanvasesDirectory(f);
    f->Close();
}
