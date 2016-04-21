
// PlotBoundaries.C
//
// Jeromy Tompkins
// 9/22/2011

#include <vector>
#include <string>
#include "ROOTUtils.h"
#include "TH2.h"
#include "TDirectory.h"
#include "TGraph.h"
#include "TColor.h"
#include "TFile.h"

using namespace ROOTUtils;
using namespace std;

void
PlotBoundaries(TDirectory *f)
{
    vector<Int_t> fColors(16,1);
    fColors[0] = 1; fColors[1] = 2; fColors[2] = 3; fColors[3] = 4;
    fColors[4] = 5; fColors[5] = 6; fColors[6] = 7; fColors[7] = 8;
    fColors[8] = kGreen; fColors[9] = kRed; fColors[10]= kOrange; fColors[11]= kViolet;
    fColors[12]= kGreen+2; fColors[13]= kRed+2; fColors[14]= kOrange+2; fColors[15]= kViolet+2;

    TH2F* hbg = new TH2F("hbg",";#theta (deg); #phi (deg) ",
                         200,0,180,
                         200,-180,180);
    hbg->SetStats(0);
    hbg->Draw();

    vector<TGraph*> graphs = GetAllTGraphsInDirectory(f);

    UInt_t i=0;
    vector<TGraph*>::iterator it;
    for (it = graphs.begin(); it!=graphs.end(); it++)
    {
        (*it)->SetLineColor(fColors[i%fColors.size()]);
        (*it)->SetMarkerColor(fColors[i%fColors.size()]);
        (*it)->SetMarkerStyle(8);
        (*it)->SetMarkerSize(0.5);
        (*it)->Draw("P");
        i++;
    }
}

void
PlotBoundaries(std::string fname)
{
    TFile *f = new TFile(fname.data(), "UPDATE");
    if (!f->IsOpen())
    {
        cerr << fname << " doesn't exist" << endl;
        return;
    }

    TDirectory *dir = f->GetDirectory("BoundaryCreatorOutput");
    if (dir!=0)
    {
        PlotBoundaries(dir);
    }
    else
    {
        cerr << fname << " has not directory named BoundaryCreatorOutput"
             << endl;
        return;
    }
}
