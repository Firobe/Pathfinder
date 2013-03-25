<<<<<<< HEAD:rinrynque_Output/Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#define COST_FOR 5
#define COST_DIAG 7
#include <iostream>
#include <cmath>
#define MAX_DIFF 50
#define DECAL_XY {{0,0,0},{0,1,COST_FOR},{1,1,COST_DIAG},{1,0,COST_FOR},{1,-1,COST_DIAG},{0,-1,COST_FOR},{-1,-1,COST_DIAG},{-1,0,COST_FOR},{-1,1,COST_DIAG}}


using namespace std;

class Matrix
{
public:
    Matrix();
    ~Matrix();
    const int GetValue(const int, const int, const int);
    void CloseCell(const int, const int, const int, const int);
    const int GetX();
    const int GetY();
    void DataFile();
protected:
private:
    void FinalData();
    int _x;
    int _y;
    int ***_array;
};

#endif // MATRIX_H
=======
#ifndef MATRIX_H
#define MATRIX_H
#define COST_FOR 100
#define COST_DIAG 141
#include <iostream>
#include <cmath>
#define MAX_DIFF 3
#define DECAL_XY { {0,0,0},{0,1,COST_FOR},{1,1,COST_DIAG},{1,0,COST_FOR},{1,-1,COST_DIAG},{0,-1,COST_FOR},{-1,-1,COST_DIAG},{-1,0,COST_FOR},{-1,1,COST_DIAG} }

const int decalXY[][3] = DECAL_XY;

using namespace std;

class Matrix
{
public:
    Matrix();
    ~Matrix();
    const int GetValue(const int, const int, const int);
    const int GetX();
    const int GetY();
    void DataFile();
    void calcWalls();
protected:
private:
    void FinalData();
    int _x;
    int _y;
    int ***_array;
};

#endif // MATRIX_H
>>>>>>> 838e2667b8553148d6dab95f931404a0fe8bb5ca:Pathfinder 0.7/Matrix.h
