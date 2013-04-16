/*#include <cmath>
#include "coords.h"*/

///Fonction de retour de coordonnées
template<typename TT>
coords<TT> coords<TT>::retournerCoords(TT t_x, TT t_y)
{
    coords<TT> xy;
    xy.x = t_x;
    xy.y = t_y;

    return xy;
}

template<typename TT>
coords<TT>::coords()
{
    x=0;
    y=0;
}

template<typename TT>
coords<TT>::coords(TT t_x, TT t_y)
{
    x=t_x;
    y=t_y;
}

///FONCTIONS DE SURCHARGE DES OPERATEURS

template<typename TT>
coords<TT>& coords<TT>::operator+=(const coords<TT> &b)
{
    x += b.x;
    y += b.y;

    return *this;
}

template<typename TT>
coords<TT>& coords<TT>::operator-=(const coords<TT> &b)
{
    x -= b.x;
    y -= b.y;

    return *this;
}

template<typename TT>
coords<TT>& coords<TT>::operator*=(const coords<TT> &b)
{
    x *= b.x;
    y *= b.y;

    return *this;
}

template<typename TT>
coords<TT>& coords<TT>::operator/=(const coords<TT> &b)
{
    x /= b.x;
    y /= b.y;

    return *this;
}

template<typename TT>
coords<TT> operator+(coords<TT> const& a, coords<TT> const& b)
{
    coords<TT> retourne(a);
    retourne += b;

    return retourne;
}

template<typename TT>
coords<TT> operator-(coords<TT> const& a, coords<TT> const& b)
{
    coords<TT> retourne(a);
    retourne -= b;

    return retourne;
}

template<typename TT>
coords<TT> operator*(coords<TT> const& a, coords<TT> const& b)
{
    coords<TT> retourne(a);
    retourne *= b;

    return retourne;
}

template<typename TT>
coords<TT> operator/(coords<TT> const& a, coords<TT> const& b)
{
    coords<TT> retourne;
    retourne /= b;

    return retourne;
}

template<typename TT>
coords<TT> operator%(coords<TT> const& a, coords<TT> const& b)
{
    coords<TT> retourne;
    retourne.x = fmod(a.x, b.x);
    retourne.y = fmod(a.y, b.y);

    return retourne;
}
