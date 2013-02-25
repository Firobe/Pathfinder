#ifndef coords3d3D_H_INCLUDED
#define coords3d3D_H_INCLUDED
#include <cmath>

struct coords3d
{
    double x;
    double y;
    double z;

    //Fonction static pour pouvoir l'appeler à tt moment
    static coords3d retournercoords3d(double t_x, double t_y, double t_z);
    static coords3d cross(coords3d a, coords3d b);
    void normalize();

    coords3d();
    coords3d(double t_x, double t_y, double t_z);

    //Fonctions de surcharge d'opérateurs raccourcis
    coords3d& operator+=(const coords3d &b);
    coords3d& operator-=(const coords3d &b);
    coords3d& operator*=(const coords3d &b);
    coords3d& operator/=(const coords3d &b);
};

//Fonctions de surcharge d'opérateurs
coords3d operator+(coords3d const& a, coords3d const& b);
coords3d operator-(coords3d const& a, coords3d const& b);
coords3d operator*(coords3d const& a, coords3d const& b);
coords3d operator/(coords3d const& a, coords3d const& b);

coords3d operator%(coords3d const& a, coords3d const& b);



#endif // coords3d3D_H_INCLUDED
