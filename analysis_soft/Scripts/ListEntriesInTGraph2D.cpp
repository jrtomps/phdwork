#include "Rtypes.h"
#include "TGraph2DErrors.h"
#include <iostream>
#include <iomanip>

void ListEntriesInTGraph2D(TGraph2DErrors& gr)
{
    Double_t *x = gr.GetX();
    Double_t *y = gr.GetY();
    Double_t *z = gr.GetZ();
    Double_t *ex = gr.GetEX();
    Double_t *ey = gr.GetEY();
    Double_t *ez = gr.GetEZ();

    std::cout << std::setiosflags(std::ios::scientific)
            << std::setprecision(2);
    for (UInt_t i=0; i<gr.GetN(); i++)
    {
        std::cout << "\n"
                << std::setw(4) << i
                << std::setw(10) << x[i]
                << std::setw(10) << y[i]
                << std::setw(10) << z[i]
                << std::setw(10) << ex[i]
                << std::setw(10) << ey[i]
                << std::setw(10) << ez[i];
    }

    std::cout << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);
    std::cout << std::endl;

}
