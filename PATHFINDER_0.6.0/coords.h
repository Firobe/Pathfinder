#ifndef COORDS_H_INCLUDED
#define COORDS_H_INCLUDED

struct coords
{
    double x;
    double y;

    //Fonction static pour pouvoir l'appeler � tt moment
    static coords retournerCoords(double t_x, double t_y);

    coords();
    coords(double t_x, double t_y);

    //Fonctions de surcharge d'op�rateurs raccourcis
    coords& operator+=(const coords &b);
    coords& operator-=(const coords &b);
    coords& operator*=(const coords &b);
    coords& operator/=(const coords &b);
};

//Fonctions de surcharge d'op�rateurs
coords operator+(coords const& a, coords const& b);
coords operator-(coords const& a, coords const& b);
coords operator*(coords const& a, coords const& b);
coords operator/(coords const& a, coords const& b);

coords operator%(coords const& a, coords const& b);

///intCoords, coordonn�es enti�res

struct intCoords
{
    int x;
    int y;

    //Fonction static pour pouvoir l'appeler � tt moment
    static intCoords retournerintCoords(int t_x, int t_y);

    intCoords();
    intCoords(int t_x, int t_y);

    //Fonctions de surcharge d'op�rateurs raccourcis
    intCoords& operator+=(const intCoords &b);
    intCoords& operator-=(const intCoords &b);
    intCoords& operator*=(const intCoords &b);
    intCoords& operator/=(const intCoords &b);
};

//Fonctions de surcharge d'op�rateurs
intCoords operator+(intCoords const& a, intCoords const& b);
intCoords operator-(intCoords const& a, intCoords const& b);
intCoords operator*(intCoords const& a, intCoords const& b);
intCoords operator/(intCoords const& a, intCoords const& b);

intCoords operator%(intCoords const& a, intCoords const& b);

#endif // COORDS_H_INCLUDED
