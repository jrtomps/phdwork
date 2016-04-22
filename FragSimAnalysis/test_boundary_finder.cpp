#include <iostream>
#include <iomanip>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include "Rtypes.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TRandom3.h"
#include "GridSorter.h"
#include "TCutG.h"
#include "TMath.h"
#include "GSBoundaryFinder.h"

int main(Int_t argc, Char_t* argv[])
{
    TApplication theapp("theapp",&argc, argv);

    Int_t npoints = 2000000;
    boost::shared_ptr<TGraph> gr( new TGraph(npoints));
    gr->SetMarkerStyle(22);
    gr->SetMarkerColor(kBlue);
    TRandom3 rand;

    Double_t x, y,r, th;
    boost::shared_ptr<GridSorter> sorter( new GridSorter(-4,4,-4,4,100));
    sorter->SetSparse(true);
    std::cout << sorter->GetXaxis().GetNbins() << std::endl;
    std::cout << "Begin filling" << std::endl;

    for (Int_t i=0; i<npoints; i++)
    {
        r = rand.Uniform(0,3.0);
        th = rand.Uniform(0,2*3.1415);

        x = r*TMath::Cos(th);
        y = r*TMath::Sin(th);

        sorter->InsertPoint(x,y);
        gr->SetPoint(i,x,y);
        if (i%(npoints/100)==0)
            std::cout << "\r" << std::setw(10) << i*100.0/npoints << "\%" << std::flush;
    }
    std::cout << "Finished filling" << std::endl;

//    sorter->Print();
    GSBoundaryFinder bndry_finder(sorter);
    TCutG* cut = bndry_finder.GetBoundary();
    if (cut==0) return -1;
    cut->SetLineColor(kRed);
    cut->SetLineWidth(2);
    cut->SetMarkerStyle(20);

    gr->Draw("PA");
    cut->Draw("CP");


    std::ofstream file("tbounds.out",std::ios::out|std::ios::app);
    bndry_finder.WriteBoundaryTo(file,"this is only a test!");
    file.close();

    theapp.Run();

    return 0;
}
