#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "main.h"
#include <vector>
#include <cstdlib>
#include <cstdio>

class outPut
{
    public:
    outPut();
    ~outPut();

    void openHeightMap(char carte[]  = "Ressources\grey_lvl.pdt");
    void display();

    private:
    std::map<std::string, int> o_registry;
    std::vector < std::vector <int> >  _heightMap;
};

//openHeightMap(char carte[]  = "Ressources\grey_lvl.pdt");
#endif // FUNCTIONS_H_INCLUDED
