#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include "coords3d.h"

struct scene
{
    coords3d** verticesMap;
    coords3d** normalMap;
    float orientation[4];
    double distance;
    double zoom;
    float lightDir[3];
    float lightPos[4];
    coords3d focus;
};

#endif // SCENE_H_INCLUDED
