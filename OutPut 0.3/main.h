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
//#include "prevDeclarations.h"

#define CASE_H SCREEN_H/20
#define CASE_L SCREEN_L/20

///Partie de Virgile (Aah berk)

#include <fstream>
#include <cmath>
#include "Matrix.h"
#define NN 0
#define NE 1
#define EE 2
#define SE 3
#define SS 4
#define SO 5
#define OO 6
#define NO 7
#define DEC_X 0
#define DEC_Y 1
#define COST 2
#define STATE 3

struct Node
{
    int Px;
    int Py;
    int Pg;
    double G; //Depuis départ
    double H; //Vol d'oiseau jusqu'à arrivée
    double F; //Coût total = G+H
    int x;
    int y;
};
typedef struct Node Node;

bool Valid(int, Node, vector<Node>&, Matrix);
void New(int, Node, vector<Node>&, vector<Node>, Node, Matrix);
Node Affect(int, int, int, int, int, double, double, double);
Node SetCurrent(vector<Node>&, vector<Node>&);

#endif // MAIN_H_INCLUDED
