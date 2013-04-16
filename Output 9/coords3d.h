#ifndef coords3d3D_H_INCLUDED
#define coords3d3D_H_INCLUDED
#include <cmath>

template<typename TT>
struct coords3d
{
    TT x;
    TT y;
    TT z;

    //Fonction static pour pouvoir l'appeler à tt moment
    static coords3d retournercoords3d(TT t_x, TT t_y, TT t_z);
    static coords3d cross(coords3d a, coords3d b);
    void normalize();

    coords3d();
    coords3d(TT t_x, TT t_y, TT t_z);

    //Fonctions de surcharge d'opérateurs raccourcis
    coords3d& operator+=(const coords3d &b);
    coords3d& operator-=(const coords3d &b);
    coords3d& operator*=(const coords3d &b);
    coords3d& operator/=(const coords3d &b);
};

//Fonctions de surcharge d'opérateurs
template<typename TT>
coords3d<TT> operator+(coords3d<TT> const& a, coords3d<TT> const& b);
template<typename TT>
coords3d<TT> operator-(coords3d<TT> const& a, coords3d<TT> const& b);
template<typename TT>
coords3d<TT> operator*(coords3d<TT> const& a, coords3d<TT> const& b);
template<typename TT>
coords3d<TT> operator/(coords3d<TT> const& a, coords3d<TT> const& b);

template<typename TT>
coords3d<TT> operator%(coords3d<TT> const& a, coords3d<TT> const& b);

#include "coords3d.tpp"

#endif // coords3d3D_H_INCLUDED
