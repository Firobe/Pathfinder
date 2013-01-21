#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

using namespace std;


class Matrix
{
public:
    Matrix();
    ~Matrix();
    void Load(const char*);
    bool GetValue(int, int);
    int GetX();
    int GetY();
protected:
private:
    int _x;
    int _y;
    bool **_array;
};

#endif // MATRIX_H
