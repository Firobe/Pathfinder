#ifndef MATRIX_H
#define MATRIX_H
#define COST_FOR 5
#define COST_DIAG 7
#include <iostream>
#include <cmath>
#define MAX_DIFF 50
#define DECAL_XY { {0,0,0},{0,1,COST_FOR},{1,1,COST_DIAG},{1,0,COST_FOR},{1,-1,COST_DIAG},{0,-1,COST_FOR},{-1,-1,COST_DIAG},{-1,0,COST_FOR},{-1,1,COST_DIAG} }

const int decalXY[][3] = DECAL_XY;

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
    void calcWalls();
protected:
private:
    void FinalData();
    int _x;
    int _y;
    int ***_array;
};

#endif // MATRIX_H
