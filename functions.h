#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "main.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "registry.h"
#include "Matrix.h"
#include "coords3d.h"

class outPut
{
    public:
    outPut(Matrix array);
    ~outPut();

    void display();

    private:
    GLuint _dispListMap;
    registry _reg;

    rude::Config _config;
    Matrix _data;

    void genList();
    void init_outPut();
    void init_Tw();
};

void ConvertQuaternionToMatrix(const float *quat, float *mat);

//openHeightMap(char carte[]  = "Ressources\grey_lvl.pdt");
#endif // FUNCTIONS_H_INCLUDED
