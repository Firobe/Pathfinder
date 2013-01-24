#ifndef MATRIX_H
#define MATRIX_H
#define COST_FOR 5
#define COST_DIAG 7
#include <iostream>

using namespace std;


class Matrix
{
public:
    Matrix();
    ~Matrix();
    bool GetValue(int, int, int, int);
    void LoadData();
    int GetX();
    int GetY();
protected:
private:
    void DataFile();
    void FinalData();
    int _x;
    int _y;
    int ****_array;
};

#endif // MATRIX_H
