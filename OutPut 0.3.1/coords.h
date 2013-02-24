#ifndef COORDS_H_INCLUDED
#define COORDS_H_INCLUDED

struct coords
{
    double x;
    double y;

    //Fonction static pour pouvoir l'appeler à tt moment
    static coords retournerCoords(double t_x, double t_y);

    coords();
    coords(double t_x, double t_y);

    //Fonctions de surcharge d'opérateurs raccourcis
    coords& operator+=(const coords &b);
    coords& operator-=(const coords &b);
    coords& operator*=(const coords &b);
    coords& operator/=(const coords &b);
};

//Fonctions de surcharge d'opérateurs
coords operator+(coords const& a, coords const& b);
coords operator-(coords const& a, coords const& b);
coords operator*(coords const& a, coords const& b);
coords operator/(coords const& a, coords const& b);

coords operator%(coords const& a, coords const& b);


#endif // COORDS_H_INCLUDED
