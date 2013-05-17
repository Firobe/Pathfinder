#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "main.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <AntTweakBar.h>

#include <cstring>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "registry.h"
#include "scene.h"
#include "coords.h"
#include "coords3d.h"
#include "Matrix.h"
#include "verticesArrays.h"
#include "shaders.h"
#include "postproc.h"

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
};

struct s_status
 {
     bool running;
     bool restart;
     int maxDiff;
 };

class outPut
{
    public:
    outPut(Matrix& array);
    ~outPut();

    void display();
    void init_outPut();
    void reInit();
    void setColor(int x, int y, int value);
    s_status getStatus();
    void centerView();

    void setScene();
    void drawScene();
    void drawResult(vector<Node>* list = NULL);
    void drawTerrain(bool reinit = false);
    std::vector<int*> choosePoints();

    private:

    registry _reg;
    scene _scene3d;

    opShader _sNolight;
    lightShading _sLight;
    GLuint uid_maxCost, uid_defaultColor;
    opShader _postProcess;
    GLuint uid_pp_colors, uid_pp_offset, uid_pp_effect;
    unsigned int _effect;

    fbo frameBuffer;

    s_status _status;
    TwBar *b_scene, *b_reglages, *b_postprocess, *b_pathfind;

    rude::Config _config;
    int** _data;
    vector <coords<int> > _points;

    struct changeValue
    {
        int x;
        int y;
        int value;
    };
    vector<changeValue> _valueChanges;
    int _maxValue;

    coords<int> _dimensions;

    void drawNormals(bool reinit = false);
    void drawAxis();
    void drawLight();
    void drawPoints();

    void init_Tw();
    void init_Bars();
    void loadConfig();

    void gen_verticesMap();
    template<typename TT>
    coords3d<TT> getVertex(int x, int y);
    void gen_normalMap();
};

template<typename TT>
coords3d<TT> outPut::getVertex(int x, int y)
{
    x = clamp(x, 0, _dimensions.x-1);
    y = clamp(y, 0, _dimensions.y-1);

    return _scene3d.verticesMap[x][y];
}

void ConvertQuaternionToMatrix(const float *quat, float *mat);
void catchError(char* operation);

#endif // FUNCTIONS_H_INCLUDED
