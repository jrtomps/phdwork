#include "Rtypes.h"
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include "ConverterMultiDim.h"
#include "TGraph2DErrors.h"
#include "TMath.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "RegionIntegratorMultiDim.h"
#include "ProgressBar.h"
#include "RegionIntegralChiSq.h"

UInt_t RegionIntegralChiSqNCalls=0;

RegionIntegralChiSq::RegionIntegralChiSq(TGraph2DErrors *gr,
                                   const ROOT::Math::IParametricFunctionMultiDim &func,
                                   const std::vector<Regions>& regions)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(func.Clone())),
    fConversion(),
    fIntegrator(new RegionIntegratorMultiDim()),
    fX(),
    fY(),
    fZ(),
    fEX(),
    fEY(),
    fEZ(),
    fTheRegions(regions),
    fPointToRegionMap()
{
    RegionIntegralChiSqNCalls=0;
    fIntegrator->SetFunction(*fFunc);
    CopyDataFromGraph(gr);
    RemoveZeroesFromData();

    // This assumes that the regions correspond to the
    // points in the TGraph2DErrors and thus the initialization
    // of fPointToRegionMap is done in the RemoveZeroesFromData
    // member function
}

RegionIntegralChiSq::RegionIntegralChiSq(const RegionIntegralChiSq &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFunc(dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(obj.fFunc->Clone())),
    fConversion(obj.fConversion),
    fIntegrator(obj.fIntegrator), // this now shares the pointer with obj
    fX(obj.fX),
    fY(obj.fY),
    fZ(obj.fZ),
    fEX(obj.fEX),
    fEY(obj.fEY),
    fEZ(obj.fEZ),
    fTheRegions(obj.fTheRegions),
    fPointToRegionMap(obj.fPointToRegionMap)
{
}

RegionIntegralChiSq& RegionIntegralChiSq::operator=(RegionIntegralChiSq obj)
{
    swap(*this, obj);
    return *this;
}

RegionIntegralChiSq::~RegionIntegralChiSq()
{}

void RegionIntegralChiSq::SetConversion(const ROOT::Math::IBaseFunctionOneDim &f)
{
    fConversion.reset(new ConverterMultiDim(fFunc->NDim(),std::shared_ptr<ROOT::Math::IBaseFunctionOneDim>(f.Clone())));
}

void RegionIntegralChiSq::SetConversion(const std::shared_ptr<ConverterMultiDim> &f)
{
    if (f->size() == fFunc->NDim())
        fConversion = f;
    else
    {
        std::cerr << "Not enough Conversion functions for the number of dimensions"
                << "\nConversion will not be stored" << std::endl;
    }
}

void RegionIntegralChiSq::ConvertData(const std::shared_ptr<ConverterMultiDim> &f)
{
    // Assumes that the conversions are simple scalings
    // and thus the errors can be converted in the same manner
    if (f->size() == fFunc->NDim())
    {
        ROOT::Math::IBaseFunctionOneDim& xconv = *(*f)[0];
        ROOT::Math::IBaseFunctionOneDim& yconv = *(*f)[1];

        for (Int_t i=0; i<fZ.size(); i++)
        {
            std::cout << "\nx["<<i<< "]=" << fX[i] << "\ty[" <<i << "]=" << fY[i];
            fX[i] = xconv(fX[i]);
            fY[i] = yconv(fY[i]);

            std::cout << "--> x["<<i<< "]=" << fX[i] << "\ty[" <<i << "]=" << fY[i];

            fEX[i] = xconv(fEX[i]);
            fEY[i] = yconv(fEY[i]);
        }
    }
    else
    {
        std::cerr << "Not enough Conversion functions for the number of dimensions"
                << "\nData will not be converted!"<< std::endl;
    }


}


RegionIntegralChiSq* RegionIntegralChiSq::Clone() const
{
    return new RegionIntegralChiSq(*this);
}

UInt_t RegionIntegralChiSq::NDim() const
{
    return fFunc->NPar();
}

std::shared_ptr<RegionIntegratorMultiDim>& RegionIntegralChiSq::GetIntegrator()
{
    return fIntegrator;
}

void RegionIntegralChiSq::SetIntegrator(const std::shared_ptr<RegionIntegratorMultiDim> &integrator)
{
    fIntegrator = integrator;
}

const std::map<UInt_t, Int_t>& RegionIntegralChiSq::GetPointToRegionMap() const
{
    return fPointToRegionMap;
}

void RegionIntegralChiSq::SetPointToRegionMap(const std::map<UInt_t, Int_t>& p2rmap)
{
    fPointToRegionMap = p2rmap;
}

Double_t RegionIntegralChiSq::DoEval(const double *pars) const
{
    fFunc->SetParameters(pars);
    Double_t sq_sum = 0;

    Double_t v = 0;

    std::cout << "Call=" << std::setw(3) << RegionIntegralChiSqNCalls << " p[0]=" << pars[0] << " p[1]=" << pars[1] << " p[2]=" << pars[2] << " p[3]=" << pars[3];
    std::cout << std::endl;
    ProgressBar pb(fZ.size(),std::cout, "\t");

    UInt_t count=0;
    std::map<UInt_t,Int_t>::const_iterator it;
    for (UInt_t i=0; i<fZ.size(); i++)
    {

        it = fPointToRegionMap.find(i);
        if (it->second<0) continue;

        Regions reg = fTheRegions[it->second];

        if (fConversion.use_count()!=0)
        {
            std::cout << "Using a conversion" << std::endl;
            if (fIntegrator->IsUsingCenter())
            {
                for (UInt_t j=0; j<reg.size(); j++)
                {
                    reg[j].cntr_xlow = (*(*fConversion)[0])(reg[j].cntr_xlow);
                    reg[j].cntr_xhigh = (*(*fConversion)[0])(reg[j].cntr_xhigh);
                    // reg[j].cntr_xlow = (*fConversion)(reg[j].cntr_xlow);
                    // reg[j].cntr_xhigh = (*fConversion)(reg[j].cntr_xhigh);

                    // Note the next two line purposefully use the grid values
                    reg[j].grid_ylow = (*(*fConversion)[1])(reg[j].grid_ylow);
                    reg[j].grid_yhigh = (*(*fConversion)[1])(reg[j].grid_yhigh);
                    // reg[j].grid_ylow = (*fConversion)(reg[j].grid_ylow);
                    // reg[j].grid_yhigh = (*fConversion)(reg[j].grid_yhigh);
                }
            }
            else
            {
                for (UInt_t j=0; j<reg.size(); j++)
                {
                    reg[j].grid_xlow = (*(*fConversion)[0])(reg[j].grid_xlow);
                    reg[j].grid_xhigh = (*(*fConversion)[0])(reg[j].grid_xhigh);
                    // reg[j].grid_xlow = (*fConversion)(reg[j].cntr_xlow);
                    // reg[j].grid_xhigh = (*fConversion)(reg[j].cntr_xhigh);

                    // Note the next two line purposefully use the grid values
                    reg[j].grid_ylow = (*(*fConversion)[1])(reg[j].grid_ylow);
                    reg[j].grid_yhigh = (*(*fConversion)[1])(reg[j].grid_yhigh);
                    // reg[j].grid_ylow = (*fConversion)(reg[j].grid_ylow);
                    // reg[j].grid_yhigh = (*fConversion)(reg[j].grid_yhigh);
                }
            }
        }

        v = fIntegrator->RegionIntegral(reg);

        pb.AdvanceProgress();

        std::cout << " Int=" << std::setw(10) << v
                << " fZ[" << std::setw(2) <<  i << "]=" << std::setw(10) << fZ[i]
                << std::flush;

        sq_sum += TMath::Power((fZ[i]-v)/fEZ[i],2.0);
        count++;
    }

    pb.Print();
    std::cout << " FCN=" << std::setw(10) << sq_sum/(count-1.0)
            << "                            "
            << std::endl;

    RegionIntegralChiSqNCalls++;
    return sq_sum/(count-1.0);
}

void RegionIntegralChiSq::CopyDataFromGraph(TGraph2DErrors* gr)
{
    if (gr==0)
    {
        std::cerr << "FCN_ChiSq::CopyDataFromGraph " << "NULL graph argument" << std::endl;
        throw 2002;
    }
    //assert(gr);

    fX.resize(gr->GetN());
    fY.resize(gr->GetN());
    fZ.resize(gr->GetN());

    fEX.resize(gr->GetN());
    fEY.resize(gr->GetN());
    fEZ.resize(gr->GetN());

    fX.assign(gr->GetX(),gr->GetX()+gr->GetN());
    fY.assign(gr->GetY(),gr->GetY()+gr->GetN());
    fZ.assign(gr->GetZ(),gr->GetZ()+gr->GetN());

    fEX.assign(gr->GetEX(),gr->GetEX()+gr->GetN());
    fEY.assign(gr->GetEY(),gr->GetEY()+gr->GetN());
    fEZ.assign(gr->GetEZ(),gr->GetEZ()+gr->GetN());
}

void RegionIntegralChiSq::RemoveZeroesFromData(void)
{

    UInt_t or_size = fZ.size();

    UInt_t incr = 0;
    for (UInt_t i=0; i<fZ.size(); i++)
    {
        if (fZ[i] == 0)
        {
            fX.erase(fX.begin()+i);
            fY.erase(fY.begin()+i);
            fZ.erase(fZ.begin()+i);

            fEX.erase(fEX.begin()+i);
            fEY.erase(fEY.begin()+i);
            fEZ.erase(fEZ.begin()+i);
            i--;
        }
        else
        {
            fPointToRegionMap[i] = incr;
        }
        incr++;
    }

    if ( (or_size-fZ.size())!=0 )
    {
        std::cout << "Removed "
                << or_size - fZ.size()
                << " elements with value zero" << std::endl;
    }

}


void RegionIntegralChiSq::InitializeMap()
{
#ifdef DEBUG_RegionIntegralChiSq
    std::cout << "Initializing map" << std::endl;
#endif
    Centers cntrs = ComputeCenters();
    for (UInt_t i=0; i<fX.size(); i++)
    {
        Int_t index = ClosestRegionIndex(fX[i],fY[i],cntrs);
        if (index>=0)
        {
#ifdef DEBUG_RegionIntegralChiSq
            std:: cout << "i=" << i << " : index=" << index;
#endif
            if ( RegionEncloses(index, fX[i], fY[i]) )
            {
#ifdef DEBUG_RegionIntegralChiSq
                std:: cout << " is inside";
#endif
                fPointToRegionMap[i] = index;
            }

        }
        else
            fPointToRegionMap[i] = -1;
#ifdef DEBUG_RegionIntegralChiSq
            std::cout << std::endl;
#endif

    }
}

RegionIntegralChiSq::Centers RegionIntegralChiSq::ComputeCenters()
{
#ifdef DEBUG_RegionIntegralChiSq
    std::cout << "Computing centers" << std::endl;
#endif
    Centers cntrs;
    for (UInt_t i=0; i<fTheRegions.size(); i++)
    {
        Point pt = ComputeCenter(fTheRegions[i]);
        cntrs.push_back(pt);
    }

    return cntrs;
}

RegionIntegralChiSq::Point RegionIntegralChiSq::ComputeCenter(Regions &regions)
{
    Point pt;
    pt.first=0;
    pt.second=0;

    UInt_t n=0;
    for (UInt_t i=0; i<regions.size(); i++)
    {
        pt.first += regions[i].cntr_xlow;
        pt.first += regions[i].cntr_xhigh;

        pt.second += regions[i].cntr_ylow;
        pt.second += regions[i].cntr_yhigh;

        n += 2;
    }

    if (n!=0)
    {
        pt.first /= n;
        pt.second /= n;
    }

#ifdef DEBUG_RegionIntegralChiSq
    std::cout << pt.first << "\t" << pt.second << std::endl;
#endif
    return pt;
}


Int_t RegionIntegralChiSq::ClosestRegionIndex(const Double_t x, const Double_t y, Centers &cntrs)
{
    Int_t index = -1;
    if (cntrs.size()!=0)
    {
        Double_t dx = cntrs.front().first-x;
        Double_t dy = cntrs.front().second-y;
        index = 0;

        Double_t r2 = dx*dx + dy*dy;
        for (UInt_t i=1; i<cntrs.size(); i++)
        {
            dx = cntrs[i].first-x;
            dy = cntrs[i].second-y;

            if ((dx*dx + dy*dy) < r2) index = i;
        }
    }
    return index;
}

Bool_t RegionIntegralChiSq::RegionEncloses(Int_t i, Double_t x, Double_t y)
{

    Bool_t flag = false;
    if (i<0) return flag;

    Regions& regs = fTheRegions[i];

    // Since the first and the last entries in the Regions vector
    // are the lowest and highest ybins, check that y lies in the
    // the range first
#ifdef DEBUG_RegionIntegralChiSq
    std::cout << "\t" << i << "\tpt(" << x << "," << y << ")\t";
    std::cout << "yreg(" << regs.front().grid_ylow << "," << regs.back().grid_yhigh << ")"
            << std::endl;
#endif
    if (y>regs.front().grid_ylow && y<regs.back().grid_yhigh)
    {
        for (UInt_t j=0; j<regs.size() && flag==false; j++)
        {
            if ( (x>regs[j].grid_xlow) && (x<regs[j].grid_xhigh) )
                flag = true;
        }
    }

    return flag;
}


