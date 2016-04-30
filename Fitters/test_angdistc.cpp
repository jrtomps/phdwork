

#include <iostream>
#include <iomanip>
#include <string>
#include <CLHEP/Units/SystemOfUnits.h>
#include "AngDist.h"
#include "AngDistC.h"
#include "TMath.h"
#include "Rtypes.h"

std::vector<Double_t> parse_args(Int_t argc, Char_t* argv[])
{
    std::vector<Double_t> vec(2,0);
    Double_t unit  = CLHEP::rad;

    if (argc>1)
    {
        UInt_t i=1;
        while (i<argc)
        {
            std::string arg = argv[i];

            if (arg == "--unit" || arg == "--units")
            {
                std::string nextarg(argv[i+1]);
                if (nextarg == "deg")
                {
                    unit = CLHEP::deg;
                }
                else if (nextarg == "rad")
                {
                    unit = CLHEP::rad;
                }
            }
            else
            {
                Double_t theta, phi;
                std::stringstream ss;
                ss << argv[i] << " " << argv[i+1];
                ss >> theta >> phi;
                if (!ss.fail())
                {
                    vec[0] = theta;
                    vec[1] = phi;
                }
            }
            i+=2;
        } // end while loop

        vec[0] *= unit;
        vec[1] *= unit;
    }
    else
    {
        vec[0] = 1.5*unit;
        vec[1] = 3.0*unit;
    }

    return vec;
}

int main(int argc, char *argv[])
{
    std::vector<Double_t> x = parse_args(argc, argv);

    AngDist w(0.5,0.5,0.5,0.5);
    AngDistC wc(0.5,0.5,0.5,0.5);

    std::cout.precision(2);
    std::cout.setf(std::ios::scientific);
    std::cout << "AngDist ("
            << std::setw(10) << x[0] << ","
            << std::setw(10) << x[1] << ") = "
            << w(x.data()) << std::endl;

    x[0] = TMath::Cos(x[0]);
    std::cout << "AngDistC("
            << std::setw(10) << x[0] << ","
            << std::setw(10) << x[1] << ") = "
            << wc(x.data()) << std::endl;

    return 0;
}

