#include "functions.h"

std::vector < std::vector <int> > openHeightMap(char carte[])
{
    FILE* f_heightMap = NULL;

    f_heightMap = fopen(carte,"r");

    assert(f_heightMap != NULL);

    int x,y;
    fscanf(f_heightMap, "%d\n", &x);
    fscanf(f_heightMap, "%d\n", &y);

    std::vector < std::vector < int > > heightMap(x);

    for(int i = 0; i < heightMap.size(); i++)
    {
        for(int j = 0; j < y; j++)
        {
            heightMap[i].push_back(0);
            //std::cout << heightMap[i][j] << ";";
        }
        // std::cout << std::endl;
    }

    // std::cout << "----------------" << std::endl;

    for(int i = y-1; i >=0; i--)
    {
        for(int j = 0; j < x; j++)
        {
            fscanf(f_heightMap, "%d*", &(heightMap[j][i]));
            //         std::cout << heightMap[i][j] << ";";
        }
        //     std::cout << std::endl;
    }
    return heightMap;
}

