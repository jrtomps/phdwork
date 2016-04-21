#ifndef CONVERTERMULTIDIM_H
#define CONVERTERMULTIDIM_H

#include <vector>
#include "Math/IFunction.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost {

    struct shared_ptr<ROOT::Math::IBaseFunctionOneDim>;
}
#endif

//namespace std
//{
//  struct vector< std::shared_ptr<ROOT::Math::IBaseFunctionOneDim> >;
//}

typedef std::vector< std::shared_ptr<ROOT::Math::IBaseFunctionOneDim> > ConverterMultiDim;

#endif // CONVERTERMULTIDIM_H
