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
    outPut();
    ~outPut();

    void openHeightMap(char carte[]  = "Ressources\grey_lvl.pdt");
    void display(Matrix& array);

    private:
    GLuint _dispListMap;
    std::map<std::string, int> o_registry;
    std::vector < std::vector <int> >  _heightMap;
    rude::Config _config;

    void genList();
};

//openHeightMap(char carte[]  = "Ressources\grey_lvl.pdt");
#endif // FUNCTIONS_H_INCLUDED
