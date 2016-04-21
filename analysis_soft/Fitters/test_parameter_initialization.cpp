#include "Parameters.h"
#include <iostream>



Int_t Main()
{
    Parameter par;

    std::cout << par.GetIndex() << std::endl;
    std::cout << par.GetValue() << std::endl;

    return 0;
}

#ifndef __CINT__
Int_t main()
{
    return Main();
}

#endif
