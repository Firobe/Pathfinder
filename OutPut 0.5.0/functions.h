#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "main.h"
#include <vector>
#include <cstdlib>
#include <cstdio>

#include "registry.h"
#include "scene.h"
#include "coords3d.h"
#include "Matrix.h"

class outPut
{
    public:
    outPut(Matrix array);
    ~outPut();

    void display();
    void init_outPut();
    bool getStatus();

    void setScene();
    void drawScene();
    void drawResult(const vector<Node>* list);

    void genList();

    private:
    GLuint _dispListMap;
    registry _reg;
    scene _scene3d;
    bool _running;
    TwBar *b_scene, *b_reglages;

    rude::Config _config;
    int** _data;
    intCoords _dimensions;

    void drawNormals();
    void drawAxis();
    void drawTerrain();
    void drawLight();

    void init_Tw();
    void init_Bars();
    void loadConfig();

    void gen_verticesMap();
    coords3d getVertex(int x, int y);
    void gen_normalMap();
};

void ConvertQuaternionToMatrix(const float *quat, float *mat);

//openHeightMap(char carte[]  = "Ressources\grey_lvl.pdt");
#endif // FUNCTIONS_H_INCLUDED
