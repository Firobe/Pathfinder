#include "shaders.h"
#include "output.h"
#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

void lightShading::initCommunication()
{
    _light.uid_position = glGetUniformLocation(_program, "lighting.position");
    _light.uid_ambient = glGetUniformLocation(_program, "lighting.ambient");
    _light.uid_diffuse = glGetUniformLocation(_program, "lighting.diffuse");
    _light.uid_specular = glGetUniformLocation(_program, "lighting.specular");
    _uid_camPos = glGetUniformLocation(_program, "camPos");

    _material.uid_shininess = glGetUniformLocation(_program, "material.shininess");
    _material.uid_ambient = glGetUniformLocation(_program, "material.ambient");
    _material.uid_diffuse = glGetUniformLocation(_program, "material.diffuse");
    _material.uid_specular = glGetUniformLocation(_program, "material.specular");
    catchError("initialisation de la communicatio au glsl");
}

void lightShading::setPos(float lightPos[])
{
    catchError("Avant envoi de position de lumiere");
    glUseProgram(_program);
    glUniform4fv(_light.uid_position, 1, lightPos);
    catchError("Envoi de position de lumiere");
}
void lightShading::setLightMaterial(float ambient[], float diffuse[], float specular[])
{
    glUseProgram(_program);
    glUniform4fv(_light.uid_ambient, 1, ambient);
    glUniform4fv(_light.uid_diffuse, 1, diffuse);
    glUniform4fv(_light.uid_specular, 1, specular);
    catchError("Envoi de parametres de lumiere");
}
void lightShading::setLightMaterial(float ambient, float diffuse, float specular)
{

}

void lightShading::setMaterial(float ambient[], float diffuse[], float specular[], float shininess)
{
    glUseProgram(_program);
    glUniform4fv(_material.uid_ambient, 1, ambient);
    glUniform4fv(_material.uid_diffuse, 1, diffuse);
    glUniform4fv(_material.uid_specular, 1, specular);
    glUniform1f(_material.uid_shininess, shininess);
    catchError("Envoi de donnees de material");

}
void lightShading::setMaterial(float ambient, float diffuse, float specular, float shininess)
{

}
