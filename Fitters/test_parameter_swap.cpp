#include "Parameter.h"
#include "Parameters.h"
#include <iostream>


int main()
{
    FitParameter p0 (2,0.2,"0p2",0.2,0.2,0.2,0.2);
    FitParameter p1 (3,0.3,"0p3",0.3,0.3,0.3,0.3);

    std::cout << "Before"
            << "\n" << p0
            << "\n" << p1
            << std::endl;

    p0.swap(p1);

    std::cout << "After"
            << "\n" << p0
            << "\n" << p1
            << std::endl;


    Parameter p5 (5,0.5,"0p5",0.5);
    Parameter p6 (6,0.6,"0p6",0.6);

    std::cout << "Before"
            << "\n" << p5
            << "\n" << p6
            << std::endl;

    p5.swap(p6);

    std::cout << "After"
            << "\n" << p5
            << "\n" << p6
            << std::endl;

    std::cout << "////////////////////////" << std::endl;
    std::cout << "Parameters"  << std::endl;

    Parameters pars0;
    pars0.AddParameter(p0);
    pars0.AddParameter(p1);

    Parameters pars1;
    pars1.AddParameter(p5);
    pars1.AddParameter(p6);

    std::cout << "Before"
            << "\n" << pars0
            << "\n" << pars1
            << std::endl;

    pars0.swap(pars1);

    std::cout << "After"
            << "\n" << pars0
            << "\n" << pars1
            << std::endl;



    return 0;
}

