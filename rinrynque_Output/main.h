#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <GL/glfw.h>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <new>
#include <AntTweakBar.h>

#include "config.h"

#include "coords.h"
#include "functions.h"

#define CASE_H SCREEN_H/20
#define CASE_L SCREEN_L/20

///Partie de Virgile (Aah berk)

#include <fstream>
#include <cmath>
#include <ctime>
#include "Matrix.h"

struct Node
{
    int Px;
    int Py;
    int Pg;
    double G; //Depuis départ
    double H; //Vol d'oiseau jusqu'ŕ arrivée
    double F; //Coűt total = G+H
    int x;
    int y;
};
typedef struct Node Node;

const bool Valid(const int, const Node, Matrix);
void New(int, const Node, vector<Node>&, const Node, const Matrix);
const Node Affect(const int, const int, const int, const int, const int, const double, const double, const double);
const Node SetCurrent(vector<Node>&, Matrix&);

#endif // MAIN_H_INCLUDED
