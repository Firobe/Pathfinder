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

struct opShader
{
opShader();
~opShader();
static GLchar* loadSource(const char*filename);
static GLuint loadShader(GLenum type, const char*filename);

void loadProgram(const char* vsFile, const char* fsFile);
void linkProgram();
void delShader();
void loadVs(const char*filename);
void loadFs(const char*filename);

GLuint vs;
GLuint fs;
GLuint program;
};

#endif // SHADERS_H_INCLUDED
