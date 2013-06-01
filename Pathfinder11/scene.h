#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include "coords3d.h"

struct scene
{
    coords3d<float>** verticesMap;
    coords3d<float>** normalMap;
    float orientation[4];
    double distance;
    double zoom;
    float lightDir[3];
    float lightPos[4];
    coords3d<float> focus;

    glm::mat4 MVM;
    glm::mat4 PM;
    glm::mat4 MVPM;
    glm::mat4 MVNM;
};

#endif // SCENE_H_INCLUDED
