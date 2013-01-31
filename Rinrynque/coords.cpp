#include <cmath>
#include "coords.h"

///Fonction de retour de coordonnées
coords coords::retournerCoords(double t_x, double t_y)
{
    coords xy;
    xy.x = t_x;
    xy.y = t_y;

    return xy;
}

coords::coords()
{
    x=0;
    y=0;
}

coords::coords(double t_x, double t_y)
{
    x=t_x;
    y=t_y;
}

///FONCTIONS DE SURCHARGE DES OPERATEURS

coords& coords::operator+=(const coords &b)
{
    x += b.x;
    y += b.y;

    return *this;
}

coords& coords::operator-=(const coords &b)
{
    x -= b.x;
    y -= b.y;

    return *this;
}

coords& coords::operator*=(const coords &b)
{
    x *= b.x;
    y *= b.y;

    return *this;
}

coords& coords::operator/=(const coords &b)
{
    x /= b.x;
    y /= b.y;

    return *this;
}

coords operator+(coords const& a, coords const& b)
{
    coords retourne(a);
    retourne += b;

    return retourne;
}

coords operator-(coords const& a, coords const& b)
{
    coords retourne(a);
    retourne -= b;

    return retourne;
}

coords operator*(coords const& a, coords const& b)
{
    coords retourne(a);
    retourne *= b;

    return retourne;
}

coords operator/(coords const& a, coords const& b)
{
    coords retourne;
    retourne /= b;

    return retourne;
}
coords operator%(coords const& a, coords const& b)
{
    coords retourne;
    retourne.x = fmod(a.x, b.x);
    retourne.y = fmod(a.y, b.y);

    return retourne;
}
