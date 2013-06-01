#ifndef COORDS_H_INCLUDED
#define COORDS_H_INCLUDED
#include <cmath>

template<typename TT>
struct coords
{
    TT x;
    TT y;

    //Fonction static pour pouvoir l'appeler à tt moment
    static coords retournerCoords(TT t_x, TT t_y);

    coords();
    coords(TT t_x, TT t_y);

    //Fonctions de surcharge d'opérateurs raccourcis
    coords& operator+=(const coords &b);
    coords& operator-=(const coords &b);
    coords& operator*=(const coords &b);
    coords& operator/=(const coords &b);
};

//Fonctions de surcharge d'opérateurs
template<typename TT>
coords<TT> operator+(coords<TT> const& a, coords<TT> const& b);
template<typename TT>
coords<TT> operator-(coords<TT> const& a, coords<TT> const& b);
template<typename TT>
coords<TT> operator*(coords<TT> const& a, coords<TT> const& b);
template<typename TT>
coords<TT> operator/(coords<TT> const& a, coords<TT> const& b);

template<typename TT>
coords<TT> operator%(coords<TT> const& a, coords<TT> const& b);

#include "coords.tpp"

#endif // COORDS_H_INCLUDED
