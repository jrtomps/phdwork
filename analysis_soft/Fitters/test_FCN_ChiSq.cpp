

#include <iostream>
#include <string>
#include <fstream>
#include "Parameters.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "FCN_ChiSq.h"

using namespace std;

int main()
{
    std::string par_fname = "params.dat";
    std::ifstream parfile(par_fname.data(), std::ios::in);
    Parameters pars(parfile);
    if (!pars.KeysAreSensible())
    {
        std::cerr << "Parameters loaded from " << par_fname << " have a problem."
                << "\nFitting will not be attempted."
                << std::endl;
        return -1;
    }

    TFile* f =new TFile("/home/jrtomps/pffrag2/analysis/results_narrow/ang_dists/angdists_238U_232Th_6.2_Lin.root");
    TGraph2DErrors *gr = dynamic_cast<TGraph2DErrors*>(f->Get("results/angdist_graph"));
    if (gr==NULL)
    {
        std::cerr << "Graph doesn't exist" << std::endl;
        return -1;
    }

    FCN_ChiSq fcn(gr);

    f->Close();
    return 0;
}
