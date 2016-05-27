#include "map"
#include "vector"
#include "TString.h"
#include "TString.h"
#ifdef __CINT__ 
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#pragma link C++ class map<TString,bool,less<TString>,allocator<pair<const TString,bool> > >+;
#pragma link C++ class map<TString,bool,less<TString>,allocator<pair<const TString,bool> > >::*;
#pragma link C++ operators map<TString,bool,less<TString>,allocator<pair<const TString,bool> > >::iterator;
#pragma link C++ operators map<TString,bool,less<TString>,allocator<pair<const TString,bool> > >::const_iterator;
#pragma link C++ operators map<TString,bool,less<TString>,allocator<pair<const TString,bool> > >::reverse_iterator;
#endif
