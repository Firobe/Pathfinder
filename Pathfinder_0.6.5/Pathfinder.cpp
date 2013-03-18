#include "Pathfinder.h"

Pathfinder::Pathfinder()
{
    cout<<"\nOUVERTURE DE L'IMAGE (Valere)\n\n";
    matrix.DataFile();
    exCount=count=actTime=exTime=0;
}
const Node Pathfinder::SetCurrent()
{
    double minNode = 0;
    if(!openList.empty())
        for(int i=0; i<(int)openList.size(); i++)
            if(openList[i].F<openList[minNode].F)
                minNode = i;
    Node resultNode = openList[minNode];
    matrix.CloseCell(openList[minNode].x, openList[minNode].y, openList[minNode].Px, openList[minNode].Py);
    if(openList.size()>1)
        openList[minNode] = openList[openList.size()-1];
    openList.pop_back();
    return resultNode;
}

void Pathfinder::New(int i)
{
    int H=0;
    int isOpen=-1;
    int eventualX, eventualY, eventualCost;
    eventualX=currentNode.x+decalXY[i][0];
    eventualY=currentNode.y+decalXY[i][1];
    eventualCost=decalXY[i][2];
    if(!openList.empty())
        for(i=0; i<(int)openList.size(); i++)
            if(openList[i].x == eventualX && openList[i].y == eventualY)
                isOpen=i;
    if(isOpen>-1)
    {
        if(openList[isOpen].G>currentNode.Pg+eventualCost)
        {
            openList[isOpen].Pg = currentNode.G;
            openList[isOpen].Px = currentNode.x;
            openList[isOpen].Py = currentNode.y;
            openList[isOpen].G = currentNode.Pg+eventualCost;
            openList[isOpen].F = openList[isOpen].H+openList[isOpen].G;
        }
    }
    else
    {
        H= sqrt((endNode.x-eventualX)*(endNode.x-eventualX)+(endNode.y-eventualY)*(endNode.y-eventualY));
        openList.push_back(Affect(eventualX, eventualY, currentNode.x, currentNode.y, currentNode.G, currentNode.Pg+eventualCost,H,currentNode.Pg+eventualCost+H));
    }
}
const bool Pathfinder::Valid(const int i)
{
    if(matrix.GetValue(currentNode.x, currentNode.y, i)==1 || matrix.GetValue(currentNode.x+decalXY[i][0], currentNode.y+decalXY[i][1], 0)==-1)
        return false;
    return true;
}
Matrix Pathfinder::getMatrix()
{
    return matrix;
}

vector<Node> Pathfinder::find(vector<int*> bombList)
{
    cout<<"\nRECHERCHE DE CHEMIN (Virgile)\n\n";
    begNode=Affect(1, 1, 0, 0, 0, 0, 0, 0);
    endNode=Affect(matrix.GetX()-1, matrix.GetY()-1, 0, 0, 0, 0, 0, 0);
    bool exist;
    openList.push_back(begNode);
    exTime = time(0);
    cout<<"Debut du traitement..."<<endl;
    while(!openList.empty() && !(currentNode.x==endNode.x && currentNode.y==endNode.y))
    {
        actTime = time(0);
        count++;
        if(actTime-exTime>=1)
        {
            cout << "Traitement des noeuds : " << count << " ( " << count-exCount << " nd/s )" <<endl;
            exCount= count;
            exTime = actTime;
        }
        currentNode = SetCurrent();
        for(int i=1; i<9; i++)
            if(Valid(i))
                New(i);
    }
    cout << "Fin du traitement" << endl;
    if(!openList.empty())
    {
        cout << "Reconstitution du trajet final...";
        int i;
        while(!(currentNode.x == begNode.x && currentNode.y == begNode.y))
        {
            resultList.push_back(currentNode);
            currentNode.x = currentNode.Px;
            currentNode.y = currentNode.Py;
            currentNode.Px = matrix.GetValue(currentNode.x, currentNode.y, 9);
            currentNode.Py = matrix.GetValue(currentNode.x, currentNode.y, 10);
        }
        resultList.push_back(currentNode);
    }
    return resultList;
}
const Node Affect(const int x, const int y, const int Px, const int Py, const int Pg, const double G, const double H, const double F)
{
    Node result;
    result.x=x;
    result.y=y;
    result.Px=Px;
    result.Py=Py;
    result.Pg=Pg;
    result.G=G;
    result.H=H;
    result.F=F;
    return result;
}
Pathfinder::~Pathfinder()
{}
