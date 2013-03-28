#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "main.h"
#include <vector>
#include <cstdlib>
#include <cstdio>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <cstring>
#include <algorithm>

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include "registry.h"
#include "scene.h"
#include "coords3d.h"
#include "Matrix.h"
#include "verticesArrays.h"

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
};

class outPut
{
    public:
    outPut(Matrix array);
    ~outPut();

    void display();
    void init_outPut();
    void reInit();
    bool getStatus();
    void centerView();

    void setScene();
    void drawScene();
    void drawResult(vector<Node>* list = NULL);
    void drawTerrain(bool reinit = false);
    std::vector<int*> choosePoints();

    private:
    registry _reg;
    scene _scene3d;
    bool _running;
    TwBar *b_scene, *b_reglages;

    rude::Config _config;
    int** _data;
    intCoords _dimensions;

    void drawNormals();
    void drawAxis();
    void drawLight();

    void init_Tw();
    void init_Bars();
    void loadConfig();

    void gen_verticesMap();
    coords3d getVertex(int x, int y);
    void gen_normalMap();
};

void ConvertQuaternionToMatrix(const float *quat, float *mat);

#endif // FUNCTIONS_H_INCLUDED
