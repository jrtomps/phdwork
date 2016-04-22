#include "Coord2D.h"

Coord2D::Coord2D() : fX(0), fY(0) {}

Coord2D::Coord2D(Double_t x, Double_t y) : fX(x), fY(y) {}

Coord2D::Coord2D(const Coord2D &obj) : fX(obj.fX), fY(obj.fY) {}

Coord2D& Coord2D::operator=(Coord2D obj)
{
    swap(*this, obj);
    return *this;
}

Double_t Coord2D::GetX() const { return fX;}

Double_t Coord2D::GetY() const { return fY;}

void Coord2D::SetX(const Double_t x) { fX = x; }

void Coord2D::SetY(const Double_t y) { fY = y; }
