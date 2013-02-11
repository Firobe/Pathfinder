#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "main.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "Matrix.h"

class outPut
{
    public:
    outPut(Matrix array);
    ~outPut();

    void display();

    private:
    GLuint _dispListMap;
    std::map<std::string, int> o_registry;

    rude::Config _config;
    Matrix _data;

    void genList();
};

//openHeightMap(char carte[]  = "Ressources\grey_lvl.pdt");
#endif // FUNCTIONS_H_INCLUDED
