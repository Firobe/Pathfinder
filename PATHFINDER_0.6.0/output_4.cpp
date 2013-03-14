#include "output.h"

vector<int*> outPut::choosePoints()
{
    vector<int*> bombList;
    int beg[5]={0, 0, 0, 2, 2};
    int med[5]={0, 0, 0, 50, 50};
    int end[5]={0, 0, 0, 249, 249};
    bombList.push_back(beg);
    bombList.push_back(med);
    bombList.push_back(end);
    return bombList;
}
