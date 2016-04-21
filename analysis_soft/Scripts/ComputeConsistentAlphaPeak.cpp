
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include "TF1.h"
#include "TH1.h"
#include "TDirectory.h"
#include "ROOTUtils.h"

TF1* TH1ToTF1(TH1* hist)
{
    std::string fname = hist->GetName();
    fname.append("_func");

    std::ostringstream funcstr;
    funcstr << hist->GetName() << "->Interpolate(x)";
    TF1* f = new TF1(fname.data(),
                     funcstr.str().data(),
                     hist->GetXaxis()->GetXmin(),
                     hist->GetXaxis()->GetXmax());
    return f;
}

std::vector<TF1*> ConvertAllTH1ToTF1(std::vector<TH1*>& h)
{
    std::vector<TF1*> funcs;
    funcs.resize(h.size());

    for (UInt_t i=0; i<h.size(); i++)
    {
        std::cout << h[i]->GetName() << std::endl;
        funcs[i] = TH1ToTF1(h[i]);
    }

    return funcs;
}


void ComputeConsistentAlphaPeaks(TDirectory* dir)
{
    std::vector<TH1*> hists = ROOTUtils::GetAllTH1InDirectory(dir);
    std::cout << "found " << hists.size() << " hists" << std::endl;

    ROOTUtils::EnforceProperOrdering(hists);

    hists[8]->Draw("hist");
}
