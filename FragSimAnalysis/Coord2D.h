#ifndef COORD2D_H
#define COORD2D_H

#include "Rtypes.h"

class Coord2D
{
private:
    Double_t fX;
    Double_t fY;

public:
    Coord2D();
    Coord2D(Double_t x, Double_t y);
    Coord2D(const Coord2D& obj);
    Coord2D& operator=(Coord2D obj);
    friend void swap(Coord2D& lhs, Coord2D& rhs);

    Double_t GetX() const;
    Double_t GetY() const;
    void SetX(const Double_t x);
    void SetY(const Double_t y);
};

#include <algorithm>
void swap(Coord2D &lhs, Coord2D &rhs)
{
    std::swap(lhs.fX, rhs.fX);
    std::swap(lhs.fY, rhs.fY);
}

#endif // COORD2D_H
