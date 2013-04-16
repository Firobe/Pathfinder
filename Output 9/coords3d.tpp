#include <cmath>
#include "coords3d.h"

///Fonction de retour de coordonnées
template<typename TT>
coords3d<TT> coords3d<TT>::retournercoords3d(TT t_x, TT t_y, TT t_z)
{
    coords3d xyz;
    xyz.x = t_x;
    xyz.y = t_y;
    xyz.z = t_z;

    return xyz;
}

template<typename TT>
coords3d<TT>::coords3d()
{
    x=0;
    y=0;
    z = 0;
}

template<typename TT>
coords3d<TT>::coords3d(TT t_x, TT t_y, TT t_z)
{
    x=t_x;
    y=t_y;
    z = t_z;
}

template<typename TT>
void coords3d<TT>::normalize()
{
    double length = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    x /= length;
    y /= length;
    z /= length;
}

template<typename TT>
coords3d<TT> coords3d<TT>::cross(coords3d a, coords3d b)
{
    coords3d n;
    n.x = (a.y * b.z) - (a.z * b.y);
    n.y = (a.z * b.x) - (a.x * b.z);
    n.z = (a.x * b.y) - (a.y * b.x);
    return n;
}

///FONCTIONS DE SURCHARGE DES OPERATEURS

template<typename TT>
coords3d<TT>& coords3d<TT>::operator+=(const coords3d<TT> &b)
{
    x += b.x;
    y += b.y;
    z += b.z;

    return *this;
}

template<typename TT>
coords3d<TT>& coords3d<TT>::operator-=(const coords3d<TT> &b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;

    return *this;
}

template<typename TT>
coords3d<TT>& coords3d<TT>::operator*=(const coords3d<TT> &b)
{
    x *= b.x;
    y *= b.y;
    z *= b.z;

    return *this;
}

template<typename TT>
coords3d<TT>& coords3d<TT>::operator/=(const coords3d<TT> &b)
{
    x /= b.x;
    y /= b.y;
    z /= b.z;

    return *this;
}

template<typename TT>
coords3d<TT> operator+(coords3d<TT> const& a, coords3d<TT> const& b)
{
    coords3d<TT> retourne(a);
    retourne += b;

    return retourne;
}

template<typename TT>
coords3d<TT> operator-(coords3d<TT> const& a, coords3d<TT> const& b)
{
    coords3d<TT> retourne(a);
    retourne -= b;

    return retourne;
}

template<typename TT>
coords3d<TT> operator*(coords3d<TT> const& a, coords3d<TT> const& b)
{
    coords3d<TT> retourne(a);
    retourne *= b;

    return retourne;
}

template<typename TT>
coords3d<TT> operator/(coords3d<TT> const& a, coords3d<TT> const& b)
{
    coords3d<TT> retourne;
    retourne /= b;

    return retourne;
}

template<typename TT>
coords3d<TT> operator%(coords3d<TT> const& a, coords3d<TT> const& b)
{
    coords3d<TT> retourne;
    retourne.x = fmod(a.x, b.x);
    retourne.y = fmod(a.y, b.y);
    retourne.z = fmod(a.z, b.z);

    return retourne;
}
