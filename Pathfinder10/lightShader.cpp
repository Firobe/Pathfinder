#include "shaders.h"
#include "output.h"
#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

void lightShading::initCommunication()
{
    _light.uid_position = glGetUniformLocation(_program, "lighting.position");
    catchError(S__LINE__);
    _light.uid_ambient = glGetUniformLocation(_program, "lighting.ambient");
    catchError(S__LINE__);
    _light.uid_diffuse = glGetUniformLocation(_program, "lighting.diffuse");
    catchError(S__LINE__);
    _light.uid_specular = glGetUniformLocation(_program, "lighting.specular");
    catchError(S__LINE__);
    _uid_camPos = glGetUniformLocation(_program, "camPos");
    catchError(S__LINE__);

    _material.uid_shininess = glGetUniformLocation(_program, "material.shininess");
    catchError(S__LINE__);
    _material.uid_ambient = glGetUniformLocation(_program, "material.ambient");
    catchError(S__LINE__);
    _material.uid_diffuse = glGetUniformLocation(_program, "material.diffuse");
    catchError(S__LINE__);
    _material.uid_specular = glGetUniformLocation(_program, "material.specular");
    catchError(S__LINE__);
}

void lightShading::setPos(float lightPos[])
{
    glUseProgram(_program);
    glUniform4fv(_light.uid_position, 1, lightPos);
    catchError(S__LINE__);
}
void lightShading::setLightMaterial(float ambient[], float diffuse[], float specular[])
{
    glUseProgram(_program);
    glUniform4fv(_light.uid_ambient, 1, ambient);
    catchError(S__LINE__);
    glUniform4fv(_light.uid_diffuse, 1, diffuse);
    catchError(S__LINE__);
    glUniform4fv(_light.uid_specular, 1, specular);
    catchError(S__LINE__);
}
void lightShading::setLightMaterial(float ambient, float diffuse, float specular)
{

}

void lightShading::setMaterial(float ambient[], float diffuse[], float specular[], float shininess)
{
    glUseProgram(_program);
    glUniform4fv(_material.uid_ambient, 1, ambient);
    catchError(S__LINE__);
    glUniform4fv(_material.uid_diffuse, 1, diffuse);
    catchError(S__LINE__);
    glUniform4fv(_material.uid_specular, 1, specular);
    catchError(S__LINE__);
    glUniform1f(_material.uid_shininess, shininess);
    catchError(S__LINE__);

}
void lightShading::setMaterial(float ambient, float diffuse, float specular, float shininess)
{

}
