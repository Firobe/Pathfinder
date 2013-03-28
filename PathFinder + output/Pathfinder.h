#ifndef PATHFINDER_H_INCLUDED
#define PATHFINDER_H_INCLUDED

#include <vector>
#include <ctime>
#include <algorithm>
#include "Matrix.h"

using namespace std;

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

class Pathfinder
{
public:
    Pathfinder();
    ~Pathfinder();
    vector<Node> find(vector<int*>);
    Matrix getMatrix();
private:
    const bool Valid(const int);
    void New(int, Node);
    const Node SetCurrent();
    vector<Node> path(Node,Node);
    void addResult(vector<Node>);
    Matrix matrix;
    int*** cells;
    vector<Node> finalList;
    Node currentNode;
    vector<Node> openList;
    int exCount, count, actTime, exTime;
};

const Node Affect(const int, const int, const int, const int, const int, const double, const double, const double);
unsigned int factorial(unsigned int);
#endif // PATHFINDER_H_INCLUDED
