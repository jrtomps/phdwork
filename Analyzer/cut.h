#ifndef CUT_H
#define CUT_H

#include <utility>
#include <set>

class LogicUnit : public std::pair<Double_t, Double_t>
{
public:
    LogicUnit(Double_t, Double_t);

};

class Cut : public std::set<LogicUnit>
{
public:
    Cut(Double_t min, Double_t max);

    Bool_t operator()(Double_t val) const;






};

#endif // CUT_H
