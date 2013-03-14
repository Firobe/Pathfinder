#ifndef PATHFINDER_H_INCLUDED
#define PATHFINDER_H_INCLUDED

#include <vector>
#include <ctime>
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
typedef struct Node Node;

class Pathfinder
{
public:
    Pathfinder();
    ~Pathfinder();
    vector<Node> find(vector<int*>);
    Matrix getMatrix();
private:
    int calcDist(Node, Node);
    const bool Valid(const int);
    void New(int);
    const Node SetCurrent();
    Matrix matrix;
    vector<Node> openList;
    vector<Node> resultList;
    Node begNode, endNode, currentNode;
    int exCount, count, actTime, exTime;
};

const Node Affect(const int, const int, const int, const int, const int, const double, const double, const double);
#endif // PATHFINDER_H_INCLUDED
