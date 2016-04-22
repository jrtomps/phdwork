#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cctype>
#include "Rtypes.h"
#include "Math/IntegratorMultiDim.h"
#include "Math/IFunction.h"
#include "RegionIntegratorMultiDim.h"



RegionFileLoader::RegionFileLoader()
    : fTheRegions()
{}


RegionFileLoader::RegionFileLoader(std::ifstream &ifile)
    : fTheRegions()
{
    LoadDataFromFile(ifile);
}

const std::vector<Regions>& RegionFileLoader::GetRegions() const
{
    return fTheRegions;
}

void RegionFileLoader::LoadDataFromFile(std::ifstream &file)
{
    if (file.fail())
    {
        std::cerr << "Error <RegionFileLoader::LoadDataFromFile> File is unreadable" <<  std::endl;
        return;
    }

    Double_t sa;
    std::string aline;
    std::stringstream ss;
    while(1)
    {
        std::getline(file,aline);
        if ( FileIsNotOkay(file) )
            break;
        else if ( LineShouldBeSkipped(aline) )
            continue;
        else if ( BeginningOfRegions(aline) )
        {
//            std::cout << aline << std::endl;
            Regions regions = ReadRegions(file);
            if (regions.size()>0)
                fTheRegions.push_back(regions);
        }
    }

    file.close();

}

Bool_t RegionFileLoader::BeginningOfRegions(std::string& line)
{
    return (line.find("det")!=std::string::npos);
}

Regions RegionFileLoader::ReadRegions(std::ifstream& f)
{
    Regions regions;
    std::vector<Double_t> vec;
    Char_t next_char;
    std::stringstream ss;
    Double_t sa;

    std::string aline;
    while (1)
    {
        f >> std::ws;
        next_char = f.peek();
        // check to see if the next is a number,
        // if so, this function is done break
        if ( ! IsNumber(next_char) )
        {
            break;
        }

        std::getline(f,aline);
        ss.clear(); ss.str("");
        vec.clear();

        ss << aline;
        ss >> sa;
        while(!ss.fail())
        {
            vec.push_back(sa);
            ss >> sa;
        }

//        std::cout << vec.size() << std::endl;
        if (vec.size() == 8)
        {
            Region region;
            region.grid_xlow = vec[0];
            region.cntr_xlow = vec[1];

            region.grid_xhigh = vec[2];
            region.cntr_xhigh = vec[3];

            region.grid_ylow = vec[4];
            region.cntr_ylow = vec[5];

            region.grid_yhigh = vec[6];
            region.cntr_yhigh = vec[7];

//            std::cout << "filling current region" << std::endl;
            regions.push_back(region);
        }
    }

    return regions;
}

Bool_t RegionFileLoader::IsNumber(Char_t c)
{
    return ( ::isdigit(c) || c=='-' || c=='+');
}

Bool_t RegionFileLoader::FileIsNotOkay(std::ifstream& f)
{
    Bool_t notokay = false;
    std::ios::iostate state = f.rdstate();

    if (f.eof())
    {
        notokay = true;
    }
    else if (f.fail())
    {
        std::cerr << std::hex << std::showbase
                << "fstream is corrupted (badbit=" << (state&std::ios_base::badbit)
                << ",failbit=" << (state&std::ios_base::failbit) << ")"
                << std::dec << std::noshowbase
                << std::endl;
        notokay = true;
    }

    return notokay;
}

Bool_t RegionFileLoader::LineShouldBeSkipped(std::string& line)
{
    if ( (line.length()==0) || (line.find_first_not_of(" \t") == std::string::npos) )
        return true;
    else
        return false;
}


//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..ooOOoo....ooOOoo..
//________________________________________________________________________________________
//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..ooOOoo....ooOOoo..

RegionIntegratorMultiDim::RegionIntegratorMultiDim(ROOT::Math::IntegrationMultiDim::Type type, double absTol, double relTol, unsigned int ncall)
    : ROOT::Math::IntegratorMultiDim(type,absTol,relTol,ncall),
    fUseCenters(false)
{}

Double_t RegionIntegratorMultiDim::RegionIntegral(const Regions &reg)
{
    Double_t result = 0;
    Double_t xlow[2], xhigh[2];
    for (UInt_t i=0; i<reg.size(); i++)
    {
        xlow[1] = reg[i].grid_ylow;
        xhigh[1] = reg[i].grid_yhigh;

        if (fUseCenters == true)
        {
            xlow[0] = reg[i].cntr_xlow;
            xhigh[0] = reg[i].cntr_xhigh;
        }
        else
        {
            xlow[0] = reg[i].grid_xlow;
            xhigh[0] = reg[i].grid_xhigh;
        }

        result += Integral(xlow,xhigh);
    }

    return result;
}

Double_t RegionIntegratorMultiDim::RegionIntegral(const ROOT::Math::IMultiGenFunction &f, const Regions &reg)
{
    Double_t result = 0;
    Double_t xlow[2], xhigh[2];
    for (UInt_t i=0; i<reg.size(); i++)
    {
        if (fUseCenters == true)
        {
            xlow[0] = reg[i].cntr_xlow;
            xlow[1] = reg[i].cntr_ylow;

            xhigh[0] = reg[i].cntr_xhigh;
            xhigh[1] = reg[i].cntr_yhigh;
        }
        else
        {
            xlow[0] = reg[i].grid_xlow;
            xlow[1] = reg[i].grid_ylow;

            xhigh[0] = reg[i].grid_xhigh;
            xhigh[1] = reg[i].grid_yhigh;
        }

        result += Integral(f, xlow, xhigh);
    }

    return result;
}


