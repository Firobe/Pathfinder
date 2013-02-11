#ifndef MATRIX_H
#define MATRIX_H
#define COST_FOR 5
#define COST_DIAG 7
#define DECAL_XY { {0,1,COST_FOR},{1,1,COST_DIAG},{1,0,COST_FOR},{1,-1,COST_DIAG},{0,-1,COST_FOR},{-1,-1,COST_DIAG},{-1,0,COST_FOR},{-1,1,COST_DIAG} }
const int decalXY[][3] = DECAL_XY;
#include <iostream>
#include <cmath>

using namespace std;


class Matrix
{
public:
    Matrix();
    ~Matrix();
    int GetValue(int, int, int, int);
    int GetX();
    int GetY();
    void DataFile();
protected:
private:
    void FinalData();
    int _x;
    int _y;
    int ****_array;
};

#endif // MATRIX_H
