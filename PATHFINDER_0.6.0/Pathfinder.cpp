#include "Pathfinder.h"

Pathfinder::Pathfinder()
{
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
    matrix.CloseCell(openList[minNode].x, openList[minNode].y, openList[minNode].Px, openList[minNode].Py);
    if(openList.size()>1)
        openList[minNode] = openList[openList.size()-1];
    openList.pop_back();
    return openList[minNode];
}

void Pathfinder::New(int i)
{
    int eventualX, eventualY, eventualCost, isOpen=-1, H=0;
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
    int endX=bombList[bombList.size()-1][bombList.size()], endY=bombList[bombList.size()-1][bombList.size()+1];
    cout << "\nPATHFINDING\n\n";
    int possTot=1;
    for(unsigned int bombe=0; bombe<bombList.size(); bombe++)
    {
        bombList[bombe][bombe]=-1;
        for(unsigned int cible=0; cible<bombList.size(); cible++)
            if(bombList[bombe][cible]==0 &&  bombList[cible][bombe]==0 && bombe!=cible)
            {
                exCount=count=actTime=exTime=0;
                cout << "Calcul distance [" << bombe << "]-->[" << cible << "]..."<<endl;
                bombList[bombe][cible]=calcDist(Affect(bombList[bombe][bombList.size()], bombList[bombe][bombList.size()+1], 0, 0, 0, 0, 0, 0), Affect(bombList[cible][bombList.size()], bombList[cible][bombList.size()+1], 0, 0, 0, 0, 0, 0));
                bombList[cible][bombe]=bombList[bombe][cible];
                resultList.clear();
                openList.clear();
            }
    }
    //FACTORIELLE
    exCount=count=actTime=exTime=0;
    resultList.clear();
    openList.clear();
    /*begNode=Affect(bombList[0][bombList.size()], bombList[0][bombList.size()+1], 0, 0, 0, 0, 0, 0);
    endNode=Affect(endX, endY, 0, 0, 0, 0, 0, 0);*/
    begNode=Affect(1, 1, 0, 0, 0, 0, 0, 0);
    endNode=Affect(249, 249, 0, 0, 0, 0, 0, 0);
    openList.push_back(begNode);
    exTime = time(0);
    cout << "Debut du traitement...\n";
    while((!openList.empty() && !(currentNode.x==endNode.x && currentNode.y==endNode.y)))
    {
        actTime = time(0);
        count++;
        if(actTime-exTime>=1)
        {
            cout << "Traitement des noeuds en cours : " << count << " ( " << count-exCount << " nd/s )" <<endl;
            exCount= count;
            exTime = actTime;
        }
        currentNode = SetCurrent();
        for(int i=1; i<9; i++)
            if(Valid(i))
                New(i);
    }
    cout << "Fin du traitement\n";
    cout << "Reconstitution du trajet final..." << endl;
    if(!openList.empty())
    {
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
int Pathfinder::calcDist(Node begiNode, Node endiNode)
{
    cout << "["<<begiNode.x<< ";" << begiNode.y << "]-->[" << endiNode.x << ";" << endiNode.y << "]\n";
    double minNode = 0;
    Matrix copymat=matrix;
    openList.push_back(begiNode);
    exTime = time(0);
    while((!openList.empty() && !(currentNode.x==endiNode.x && currentNode.y==endiNode.y)))
    {
        actTime = time(0);
        count++;
        if(actTime-exTime>=1)
        {
            cout << "Traitement des noeuds : " << count << " ( " << count-exCount << " nd/s )" <<endl;
            exCount= count;
            exTime = actTime;
        }
    if(!openList.empty())
        for(int i=0; i<(int)openList.size(); i++)
            if(openList[i].F<openList[minNode].F)
                minNode = i;
    Node resultNode = openList[minNode];
    copymat.CloseCell(openList[minNode].x, openList[minNode].y, openList[minNode].Px, openList[minNode].Py);
    if(openList.size()>1)
        openList[minNode] = openList[openList.size()-1];
    openList.pop_back();
    currentNode = resultNode;
        for(int i=1; i<9; i++)
            if(!(copymat.GetValue(currentNode.x, currentNode.y, i)==1 || copymat.GetValue(currentNode.x+decalXY[i][0], currentNode.y+decalXY[i][1], 0)==-1))
                New(i);
    }
    if(!openList.empty())
    {
        while(!(currentNode.x == begiNode.x && currentNode.y == begiNode.y))
        {
            resultList.push_back(currentNode);
            currentNode.x = currentNode.Px;
            currentNode.y = currentNode.Py;
            currentNode.Px = matrix.GetValue(currentNode.x, currentNode.y, 9);
            currentNode.Py = matrix.GetValue(currentNode.x, currentNode.y, 10);
        }
        resultList.push_back(currentNode);
    }
    else
        return 0;
    cout << "--->"<<resultList.size() << endl;
    return resultList.size();
}
