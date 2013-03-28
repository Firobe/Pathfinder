#include "Pathfinder.h"

Pathfinder::Pathfinder()
{
    cout<<"\nOUVERTURE DE L'IMAGE (Valere)\n\n";
    matrix.DataFile();
    exCount=count=actTime=exTime=0;
    cells=new int**[matrix.GetX()];
    for(int i=0; i<matrix.GetX(); i++)
    {
        cells[i]=new int*[matrix.GetY()];
        for(int j=0; j<matrix.GetY(); j++)
            cells[i][j]=new int[3];
    }
}
const Node Pathfinder::SetCurrent()
{
    double minNode = 0;
    if(!openList.empty())
        for(int i=0; i<(int)openList.size(); i++)
            if(openList[i].F<openList[minNode].F)
                minNode = i;
    Node resultNode = openList[minNode];
    cells[openList[minNode].x][openList[minNode].y][0]=-1;
    cells[openList[minNode].x][openList[minNode].y][1]=openList[minNode].Px;
    cells[openList[minNode].x][openList[minNode].y][2]=openList[minNode].Py;
    if(openList.size()>1)
        openList[minNode] = openList[openList.size()-1];
    openList.pop_back();
    return resultNode;
}

void Pathfinder::New(int i, Node endNode)
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
    if(matrix.GetValue(currentNode.x, currentNode.y, i)==1 || cells[currentNode.x+decalXY[i][0]][currentNode.y+decalXY[i][1]][0]==-1)
        return false;
    return true;
}
Matrix Pathfinder::getMatrix()
{
    return matrix;
}

vector<Node> Pathfinder::find(vector<int*> bombList)
{
    int** possib;
    int sum=0;
    vector<int> bombNumber;
    for(unsigned int i=1; i<bombList.size(); i++)
        bombNumber.push_back(i);
    possib=new int*[factorial(bombList.size()-1)];
    for(unsigned int i=0; i<factorial(bombList.size()-1); i++)
        possib[i]=new int[bombList.size()+1];
    exTime = time(0);
    cout<<"\nRECHERCHE DE CHEMIN (Virgile)\n\n";

    //Evaluation des distances
    cout<<"Calcul des distances inter-bombes..."<<endl;
    for(unsigned int bombe=0; bombe<bombList.size(); bombe++)
        for(unsigned int cible=0; cible<bombList.size(); cible++)
            if(bombe!=cible && bombList[bombe][cible]==0)
            {
                bombList[bombe][cible]=path(Affect(bombList[bombe][bombList.size()], bombList[bombe][bombList.size()+1], -1, 0, 0, 0, 0, 0),
                                            Affect(bombList[cible][bombList.size()], bombList[cible][bombList.size()+1], 0, 0, 0, 0, 0, 0)).back().x;
                bombList[cible][bombe]=bombList[bombe][cible];
            }

    //Listage des combinaisons
    cout<<"Listage des parcours possibles..."<<endl;
    do
    {
        possib[sum][0]=0;
        for (unsigned int x=1; x<bombList.size(); x++)
            possib[sum][x]=bombNumber[x-1];
        sum++;
    }
    while(next_permutation(bombNumber.begin(),bombNumber.end()));

    //Evaluation des combinaisons
    cout<<"Calcul des couts totaux des parcours possibles.."<<endl;
    for(unsigned int i=0; i<factorial(bombList.size()-1); i++)
    {
        sum=0;
        for(unsigned int j=0; j<bombList.size()-1; j++)
        {
            if(bombList[possib[i][j]][possib[i][j+1]]==0)
                sum=-1;
            if(sum!=-1)
                sum+=bombList[possib[i][j]][possib[i][j+1]];
        }
        possib[i][bombList.size()]=sum;
    }

    //Recherche de la combinaison la moins coûteuse
    cout<<"Selection du parcours le moins couteux possible..."<<endl;
    sum=0;
    for(unsigned int i=1; i<factorial(bombList.size()-1); i++)
        if(possib[i][bombList.size()]<=possib[sum][bombList.size()] && possib[i][bombList.size()]!=-1)
            sum=i;

    //Reconstitution finale
    cout<<"Reconstitution du parcours sous sa forme finale..."<<endl;
    finalList.clear();
    for(unsigned int i=0; i<bombList.size()-1; i++)
        addResult(path(Affect(bombList[possib[sum][i]][bombList.size()], bombList[possib[sum][i]][bombList.size()+1], -1, 0, 0, 0, 0, 0)
                       , Affect(bombList[possib[sum][i+1]][bombList.size()], bombList[possib[sum][i+1]][bombList.size()+1], 0, 0, 0, 0, 0, 0)));
    return finalList;
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
{
    delete cells;
}

void Pathfinder::addResult(vector<Node> toAdd)
{
    for(unsigned int i=0; i<toAdd.size()-1; i++)
        finalList.push_back(toAdd[i]);
}

vector<Node> Pathfinder::path(Node begb,Node endb)
{
    int totCost=0;
    vector<Node> resultList;
    for(int i=0; i<matrix.GetX(); i++)
        for(int j=0; j<matrix.GetY(); j++)
            for(int k=0; k<3; k++)
                cells[i][j][k]=0;
    openList.push_back(begb);
    exTime = time(0);
    while(!openList.empty() && !(currentNode.x==endb.x && currentNode.y==endb.y))
    {
        actTime = time(0);
        count++;
        if(actTime-exTime>=1)
        {
            cout << "     Vitesse de traitement : " << count-exCount << " nd/s" <<endl;
            exCount= count;
            exTime = actTime;
        }
        currentNode = SetCurrent();
        for(int i=1; i<9; i++)
            if(Valid(i))
                New(i, endb);
    }
    if(!openList.empty())
    {
        while(!(currentNode.x == begb.x && currentNode.y == begb.y))
        {
            resultList.push_back(currentNode);
            if(currentNode.x!=currentNode.Px && currentNode.y!=currentNode.Py)
                totCost+=COST_DIAG;
            else if(currentNode.Px!=-1)
                totCost+=COST_FOR;
            currentNode.x = currentNode.Px;
            currentNode.y = currentNode.Py;
            currentNode.Px = cells[currentNode.x][currentNode.y][1];
            currentNode.Py = cells[currentNode.x][currentNode.y][2];
        }
        resultList.push_back(currentNode);
    }
    openList.clear();
    cout<<"     Noeuds traites : "<<count<<" - Cout : "<<totCost<<endl;
    reverse(resultList.begin(),resultList.end());
    resultList.push_back(Affect(totCost, 0, 0, 0, 0, 0, 0, 0));
    return resultList;
}

unsigned int factorial(unsigned int n)
{
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}
