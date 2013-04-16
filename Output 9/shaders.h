#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

//#include "output.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstring>
#include <new>

class opShader
{
public:
    opShader();
    ~opShader();
    static GLchar* loadSource(const char*filename);
    static GLuint loadShader(GLenum type, const char*filename);

    void loadProgram(const char* vsFile, const char* fsFile);
    void linkProgram();
    void delShader();
    GLuint getProgramID();

protected :

    void loadVs(const char*filename);
    void loadFs(const char*filename);

    GLuint _vs;
    GLuint _fs;
    GLuint _program;
};

struct s_material
{
    float ambient[4], diffuse[4], specular[4];
    GLuint uid_ambient, uid_diffuse, uid_specular;
    float shininess;
    GLuint uid_shininess;
};
struct s_light
{
    float position[4];
    GLuint uid_position;
    float ambient[4], diffuse[4], specular[4];
    GLuint uid_ambient, uid_diffuse, uid_specular;
};

class lightShading : public opShader
{
    public:
        void initCommunication();
        void setPos(float lightPos[]);
        void setLightMaterial(float ambient[], float diffuse[], float specular[]);
        void setLightMaterial(float ambient, float diffuse, float specular);
        void setMaterial(float ambient[], float diffuse[], float specular[],float shininess);
        void setMaterial(float ambient, float diffuse, float specular,float shininess);
        /*lightShading();
        ~lightShading();*/
    protected:
        s_light _light;
        s_material _material;
        GLuint _uid_camPos;
};

#endif // SHADERS_H_INCLUDED
