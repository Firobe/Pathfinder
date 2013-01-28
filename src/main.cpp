#include "header.h"

using namespace std;

int main()
{
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    Matrix matrix;
    matrix.LoadData();
    vector<Node> openList;
    vector<Node> closedList;
    vector<Node> resultList;
    Node begNode=Affect(0, 0, 0, 0, 0, 0, 0, 0), endNode=Affect(matrix.GetX()-1, matrix.GetY()-1, 0, 0, 0, 0, 0, 0), currentNode;
    bool exist;

    openList.push_back(begNode);
    while(!openList.empty() && !(currentNode.x==endNode.x && currentNode.y==endNode.y))
    {
        if((double)closedList.size()/250 == floor((double)closedList.size()/250))
            cout << "Processing nodes...       " << closedList.size() <<endl;
        currentNode = SetCurrent(openList, closedList);
        for(int i=1; i<9; i++)
            if(Valid(i, currentNode, closedList, matrix))
                New(i, currentNode, openList, closedList, endNode, matrix);
    }

    if(!openList.empty())
    {
        cout << "Processing nodes...       " << closedList.size() << endl;
        int i;
        while(!(currentNode.x == begNode.x && currentNode.y == begNode.y))
        {
            i=0;
            resultList.push_back(currentNode);
            currentNode.x = currentNode.Px;
            currentNode.y = currentNode.Py;
            while(!(closedList[i].x == currentNode.Px && closedList[i].y == currentNode.Py))
                i++;
            currentNode.Px = closedList[i].Px;
            currentNode.Py = closedList[i].Py;
        }
        resultList.push_back(currentNode);
        cout << endl << "Writing result..." << endl;
        ofstream data("RESULT.txt");
        for(i=0; i<matrix.GetX(); i++)
        {
            for(int j=0; j<matrix.GetY(); j++)
            {
                exist=false;
                for(int k=0; k<(int)resultList.size(); k++)
                    if(resultList[k].x == i && resultList[k].y == j)
                        exist=true;
                if(exist)
                    data << "O";
                else
                {
                    for(int k=1; k<9; k++)
                        if(matrix.GetValue(j, i, k, 3)==1)
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

Node SetCurrent(vector<Node>& openList, vector<Node>& closedList)
{
    double minNode = 0;
    if(!openList.empty())
        for(int i=0; i<(int)openList.size(); i++)
            if(openList[i].F<openList[minNode].F)
                minNode = i;
    Node resultNode = openList[minNode];
    closedList.push_back(openList[minNode]);
    if(openList.size()>1)
        openList[minNode] = openList[openList.size()-1];
    openList.pop_back();
    return resultNode;
}

Node Affect(int x, int y, int Px, int Py, int Pg, double G, double H, double F)
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

void New(int i, Node currentNode, vector<Node>& openList, vector<Node> closedList, Node endNode, Matrix matrix)
{
    int H=0;
    int isOpen=-1;
    int eventualX, eventualY, eventualCost;
    eventualX=currentNode.x+matrix.GetValue(currentNode.y, currentNode.x, i, DEC_X);
    eventualY=currentNode.y+matrix.GetValue(currentNode.y, currentNode.x, i, DEC_Y);
    eventualCost=matrix.GetValue(currentNode.y, currentNode.x, i, COST);
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

bool Valid(int i, Node currentNode, vector<Node>& closedList, Matrix matrix)
{
    int eventualX, eventualY, eventualCost;
    eventualX=currentNode.x+matrix.GetValue(currentNode.y, currentNode.x, i, DEC_X);
    eventualY=currentNode.y+matrix.GetValue(currentNode.y, currentNode.x, i, DEC_Y);
    if(matrix.GetValue(currentNode.y, currentNode.x, i, 3)==1)
    {
        return false;
    }
    eventualCost=matrix.GetValue(currentNode.y, currentNode.x, i, COST);
    if(!closedList.empty())
        for(int i=0; i<(int)closedList.size(); i++)
            if(closedList[i].x==eventualX && closedList[i].y==eventualY)
                return false;
    return true;
}
