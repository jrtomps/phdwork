


#include <dirent.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "TApplication.h"
#include "TH2.h"
#include "TColor.h"
#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "RegionIntegratorMultiDim.h"

void ListAllFilesInDirOfType(std::string dirname, std::string fsuffix)
{

    ::DIR* dp = ::opendir(dirname.data());

    ::dirent* dir = 0;
    while ( (dir = ::readdir(dp)) )
    {
        std::string name(dir->d_name);
        if (name.rfind(fsuffix)!=std::string::npos)
        {
            std::cout << "\n\t" << name;
        }
    }

    std::cout << std::endl;
}


std::string GetRegionFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".bndry");
    std::cout << "\nEnter location of regions : ";
    std::getline(std::cin, fname);
    return fname;
}

Double_t Unity (const Double_t*) { return 1.0;}

std::vector<Int_t> colors(50);

void
SetupPrettyPalette()
{
   const Int_t NRGBs = 5;
   const Int_t NCont = colors.size();

   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   Int_t fI = TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);

   for (UInt_t i=0; i<colors.size(); i++)
   {
       colors[i] = fI + i;
   }

}

Int_t Main()
{
    TApplication theapp("theapp",0,0);

    std::string fname;
    std::cout << "\nFor the regions ";
    ListAllFilesInDirOfType (".",".bndry");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, fname);

    std::ifstream ifile(fname.data(),std::ios::in);
    RegionFileLoader rfl(ifile);
    ifile.close();
    std::cout << "Found " << rfl.GetRegions().size() << " boundaries" << std::endl;

    SetupPrettyPalette();

    TH2C *hbg;

    char answer;
    std::cout << "Plot in a) degrees or b) radians?";
    std::cin >> answer;
    if (answer == 'a')
        hbg = new TH2C("hbg",";#theta (deg);#phi (deg)",100,0,180,100,-180,180);
    else if (answer == 'b')
        hbg = new TH2C("hbg",";#theta (rad);#phi (rad)",100,0,CLHEP::pi,100,-1.0*CLHEP::pi,CLHEP::pi);
    else
    {
        std::cout << "bad choice, exiting" << std::endl;
        return -1;
    }

    hbg->SetStats(false);
    hbg->GetXaxis()->SetTitleSize(0.06);
    hbg->GetXaxis()->SetLabelSize(0.05);
    hbg->GetXaxis()->SetTitleOffset(0.76);

    hbg->GetYaxis()->SetTitleSize(0.06);
    hbg->GetYaxis()->SetLabelSize(0.05);
    hbg->GetYaxis()->SetTitleOffset(0.69);
    hbg->Draw();

    for (UInt_t i=0; i<rfl.GetRegions().size(); i++)
    {
        Regions reg(rfl.GetRegions()[i]);

        TCutG mycut = regions_cast<TCutG>(reg);

        std::ostringstream ss;
        ss << "cut" << i;
        mycut.SetName(ss.str().data());

        mycut.SetLineColor(colors[(i*3)%50]);
        mycut.SetMarkerColor(colors[(i*3)%50]);
        mycut.SetMarkerStyle(8);
        mycut.SetMarkerSize(0.5);
        TCutG* cut = static_cast<TCutG*>(mycut.DrawClone("LP"));

    }



    theapp.Run();
    return 0;
}

#ifndef __CINT__
int main()
{
    return Main();
}

#endif
