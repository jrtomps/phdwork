#ifndef FLUXCALCULATOR_H
#define FLUXCALCULATOR_H

#include <string>
#include <algorithm>
#include "SScalerTool.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<SScalerTool>;
    struct shared_ptr<ConfigManager>;
}
#endif

#include "ConfigManager.h"

class FluxCalculator
{
private:
    Double_t fConvFactor;
    std::shared_ptr<SScalerTool> fSclrTool;
    std::shared_ptr<ConfigManager> fConfigM;

public:
    FluxCalculator(std::string fname_config_cfname, Double_t conv_factor);

    FluxCalculator(std::string sclr_fname,
                   std::string config_fname,
                   std::string db_fname,
                   Double_t conv_factor = 225.0);
    FluxCalculator(const FluxCalculator& obj);
    FluxCalculator& operator=(FluxCalculator obj);
    friend void swap(FluxCalculator& lhs, FluxCalculator& rhs) throw();

    ~FluxCalculator();

    Double_t GetConversionFactor(void) const;
    void SetConversionFactor(Double_t conv_factor);

    Double_t ComputeFlux(std::string target, Double_t en, std::string pol, UInt_t ch_index);
    Double_t ComputeFlux(std::string target, Double_t en, std::string pol, std::string ch_name);

    Double_t ComputeFlux(Int_t run_first, Int_t run_last, UInt_t ch_index);
    Double_t ComputeFlux(Int_t run_first, Int_t run_last, std::string ch_name);

};

inline
void swap(FluxCalculator& lhs, FluxCalculator& rhs) throw()
{
    std::swap(lhs.fConvFactor,rhs.fConvFactor);
    lhs.fSclrTool.swap(rhs.fSclrTool);
    lhs.fConfigM.swap(rhs.fConfigM);
}

#endif // FLUXCALCULATOR_H
