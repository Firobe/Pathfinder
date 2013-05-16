#ifndef FBO_H_INCLUDED
#define FBO_H_INCLUDED

#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstring>
#include <new>

#define aID_UV 1

struct fbo
{
public :
    fbo();
    ~fbo();
    void genFBO(unsigned int screen_width,unsigned int screen_height);

    GLuint buffer; //Notre framebuffer dans lequel sera dessiné ce que l'on veut.
    GLuint colors; //Une texture qui stockera les couleurs
    GLuint depths; //Un rbo (render buffer object) qui stocke les profondeurs
};

#endif // FBO_H_INCLUDED
