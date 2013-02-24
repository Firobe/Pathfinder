#include "main.h"

using namespace std;

int main (int argc, char** argv)
{
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    Matrix matrix;
    matrix.DataFile();
    vector<Node> openList;
    vector<Node> resultList;
    Node begNode=Affect(1, 1, 0, 0, 0, 0, 0, 0), endNode=Affect(matrix.GetX()-1, matrix.GetY()-1, 0, 0, 0, 0, 0, 0), currentNode;
    bool exist;
    int exCount=0, count=0, actTime=0, exTime=0;
    outPut moteur(matrix);
    moteur.display();
    openList.push_back(begNode);
    exTime = time(0);
    while(!openList.empty() && !(currentNode.x==endNode.x && currentNode.y==endNode.y))
    {
        actTime = time(0);
        count++;
        if(actTime-exTime>=1)
        {
            cout << "Processing nodes : " << count << " ( " << count-exCount << " nd/s )" <<endl;
            exCount= count;
            exTime = actTime;
        }
        currentNode = SetCurrent(openList, matrix);
        for(int i=1; i<9; i++)
            if(Valid(i, currentNode, matrix))
                New(i, currentNode, openList, endNode, matrix);
    }
    cout << "Algorithm finished" << endl;
    if(!openList.empty())
    {
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
        cout << endl << "Writing result..." << endl;
        ofstream data("RESULT.txt");
        for(i=matrix.GetY()-1; i>=0; i--)
        {
            for(int j=0; j<matrix.GetX(); j++)
            {
                exist=false;
                for(int k=0; k<(int)resultList.size(); k++)
                    if(resultList[k].x == j && resultList[k].y == i)
                        exist=true;
                if(exist)
                    data << "O";
                else
                {
                    for(int k=1; k<9; k++)
                        if(matrix.GetValue(j, i, k)==1)
                            exist=true;
                    if(!exist)
                        data << " ";
                    else
                        data << "*";
                }
            }
            data << endl;
        }
        data.close();
        cout << "Pathfinding ended successfully" << endl;
    }
    else
    {
        cout << "ERROR" << endl << "The problem has no solution." << endl << "Operation aborted" << endl;
    }
    return 0;
}

const Node SetCurrent(vector<Node>& openList, Matrix& matrix)
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

void New(int i, const Node currentNode, vector<Node>& openList, const Node endNode, const Matrix matrix)
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

const bool Valid(const int i, const Node currentNode, Matrix matrix)
{
    if(matrix.GetValue(currentNode.x, currentNode.y, i)==1 || matrix.GetValue(currentNode.x+decalXY[i][0], currentNode.y+decalXY[i][1], 0)==-1)
        return false;
    return true;
}
