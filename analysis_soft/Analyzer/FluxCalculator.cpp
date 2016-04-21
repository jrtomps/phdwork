
#include <string>
#include <boost/shared_ptr.hpp>
#include "ConfigManager.h"
#include "SScalerTool.h"
#include "FNameManager.h"
#include "FluxCalculator.h"

FluxCalculator::FluxCalculator(std::string fname_config_cfname, Double_t conv_factor)
    : fConvFactor(conv_factor),
    fSclrTool(),
    fConfigM(new ConfigManager())
{
    FNameManager fnm(fname_config_cfname.data());
    fSclrTool.reset(new SScalerTool(fnm.GetSclrFileName(true),(fnm.GetSclrDir()+"/scaler.config") ) );
    fConfigM->LoadDbFromFile(fnm.GetConfigFileName(true));
}

FluxCalculator::FluxCalculator(std::string sclr_fname,
                               std::string config_fname,
                               std::string db_fname,
                               Double_t conv_factor)
    : fConvFactor(conv_factor),
    fSclrTool(new SScalerTool(sclr_fname, config_fname)),
    fConfigM(new ConfigManager())
{
    fConfigM->LoadDbFromFile(db_fname);
}

FluxCalculator::FluxCalculator(const FluxCalculator &obj)
    : fConvFactor(obj.fConvFactor),
    fSclrTool(new SScalerTool(*obj.fSclrTool)),
    fConfigM(new ConfigManager(*obj.fConfigM))
{}

FluxCalculator& FluxCalculator::operator=(FluxCalculator obj)
{
    swap(*this,obj);
}

FluxCalculator::~FluxCalculator() {}

Double_t FluxCalculator::GetConversionFactor() const
{
    return fConvFactor;
}

void FluxCalculator::SetConversionFactor(Double_t conv_factor)
{
    fConvFactor = conv_factor;
}

Double_t FluxCalculator::ComputeFlux(std::string target, Double_t en, std::string pol, UInt_t ch_index)
{
    Int_t sum = 0;
    std::vector<Int_t> rlist = fConfigM->GetListOfTargetRunsWithEnergyAndPol(target.data(), en, pol.data());
    std::vector<Int_t>::iterator it;
    for (it=rlist.begin(); it!=rlist.end(); it++)
    {
        sum += fSclrTool->GetValueByIndex(*it,ch_index);
    }

    return sum*fConvFactor;
}

Double_t FluxCalculator::ComputeFlux(std::string target, Double_t en, std::string pol, std::string ch_name)
{
    Int_t sum = 0;
    std::vector<Int_t> rlist = fConfigM->GetListOfTargetRunsWithEnergyAndPol(target.data(), en, pol.data());
    std::vector<Int_t>::iterator it;
    for (it=rlist.begin(); it!=rlist.end(); it++)
    {
        sum += fSclrTool->GetValueByName(*it,ch_name);
    }

    return sum*fConvFactor;
}

Double_t FluxCalculator::ComputeFlux(Int_t run_first, Int_t run_last, UInt_t ch_index)
{
    return fSclrTool->SumValueForRunsByIndex(run_first,run_last,ch_index)*fConvFactor;
}

Double_t FluxCalculator::ComputeFlux(Int_t run_first, Int_t run_last, std::string ch_name)
{
    return fSclrTool->SumValueForRunsByName(run_first,run_last,ch_name)*fConvFactor;
}




