#include <cmath>
#include "coords3d.h"

///Fonction de retour de coordonnées
coords3d coords3d::retournercoords3d(double t_x, double t_y, double t_z)
{
    coords3d xyz;
    xyz.x = t_x;
    xyz.y = t_y;
    xyz.z = t_z;

    return xyz;
}

coords3d::coords3d()
{
    x=0;
    y=0;
    z = 0;
}

coords3d::coords3d(double t_x, double t_y, double t_z)
{
    x=t_x;
    y=t_y;
    z = t_z;
}

void coords3d::normalize()
{
    double length = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    x /= length;
    y /= length;
    z /= length;
}

coords3d coords3d::cross(coords3d a, coords3d b)
{
    coords3d n;
    n.x = (a.y * b.z) - (a.z * b.y);
    n.y = (a.z * b.x) - (a.x * b.z);
    n.z = (a.x * b.y) - (a.y * b.x);
    return n;
}

///FONCTIONS DE SURCHARGE DES OPERATEURS

coords3d& coords3d::operator+=(const coords3d &b)
{
    x += b.x;
    y += b.y;
    z += b.z;

    return *this;
}

coords3d& coords3d::operator-=(const coords3d &b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;

    return *this;
}

coords3d& coords3d::operator*=(const coords3d &b)
{
    x *= b.x;
    y *= b.y;
    z *= b.z;

    return *this;
}

coords3d& coords3d::operator/=(const coords3d &b)
{
    x /= b.x;
    y /= b.y;
    z /= b.z;

    return *this;
}

coords3d operator+(coords3d const& a, coords3d const& b)
{
    coords3d retourne(a);
    retourne += b;

    return retourne;
}

coords3d operator-(coords3d const& a, coords3d const& b)
{
    coords3d retourne(a);
    retourne -= b;

    return retourne;
}

coords3d operator*(coords3d const& a, coords3d const& b)
{
    coords3d retourne(a);
    retourne *= b;

    return retourne;
}

coords3d operator/(coords3d const& a, coords3d const& b)
{
    coords3d retourne;
    retourne /= b;

    return retourne;
}
coords3d operator%(coords3d const& a, coords3d const& b)
{
    coords3d retourne;
    retourne.x = fmod(a.x, b.x);
    retourne.y = fmod(a.y, b.y);
    retourne.z = fmod(a.z, b.z);

    return retourne;
}
