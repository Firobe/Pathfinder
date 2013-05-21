#ifndef MATRIX_H
#define MATRIX_H
#define COST_FOR 100
#define COST_DIAG 141
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#define DECAL_XY { {0,0,0},{0,1,COST_FOR},{1,1,COST_DIAG},{1,0,COST_FOR},{1,-1,COST_DIAG},{0,-1,COST_FOR},{-1,-1,COST_DIAG},{-1,0,COST_FOR},{-1,1,COST_DIAG} }
#ifdef _WIN32
#include <windows.h>
#define SYS 1
#else
#define SYS 0
#endif
#include <string>
#include <ctime>

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
    void RandMap();
    void OpenMap();
    void calcWalls(int);
protected:
private:
    int _x;
    int _y;
    int ***_array;
};

#endif // MATRIX_H
