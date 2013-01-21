#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Matrix.h"
#define NN 0
#define NE 1
#define EE 2
#define SE 3
#define SS 4
#define SO 5
#define OO 6
#define NO 7
#define COST_FOR 5
#define COST_DIAG 7

struct Node
{
    int Px;
    int Py;
    int Pg;
    double G; //Depuis départ
    double H; //Vol d'oiseau jusqu'à arrivée
    double F; //Coût total = G+H
    int x;
    int y;
};
typedef struct Node Node;

void LoadData();
bool Valid(int, Node, vector<Node>&, Matrix);
void New(int, Node, vector<Node>&, vector<Node>, Node);
Node Affect(int, int, int, int, int, double, double, double);
Node SetCurrent(vector<Node>&, vector<Node>&);
using namespace std;

int main()
{
    /*  Beg Node : 0;0
        End Node : _x;_y    */
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    LoadData();
    Matrix matrix;
    matrix.Load("Ressources/size_pixel.pdt");
    vector<Node> openList;
    vector<Node> closedList;
    vector<Node> resultList;
    Node begNode, endNode, currentNode;
    begNode = Affect(0, 0, 0, 0, 0, 0, 0, 0);
    endNode = Affect(matrix.GetX()-1, matrix.GetY()-1, 0, 0, 0, 0, 0, 0);
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    cout << "Extracting raw data..." << endl;
    cout << "Analyzing file dimension..." << endl;
    cout << "Converting raw pixel data into exploitable data..." << endl;
    cout << "Processing nodes...       ";
    openList.push_back(begNode);
    bool exist;
    while(!openList.empty() && !(currentNode.x==endNode.x && currentNode.y==endNode.y))
    {
        if((double)closedList.size()/500 == floor((double)closedList.size()/500))
        {
            cout << "Processing nodes...       " << closedList.size() <<endl;
        }
        currentNode = SetCurrent(openList, closedList);
        for(int i=0; i<8; i++)
            if(Valid(i, currentNode, closedList, matrix))
                New(i, currentNode, openList, closedList, endNode);
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
        cout << endl << "Writing...          " << endl << endl;
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
                else if(!exist && matrix.GetValue(i, j))
                    data << "#";
                else if(!exist && !matrix.GetValue(i, j))
                    data << " ";
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

void New(int i, Node currentNode, vector<Node>& openList, vector<Node> closedList, Node endNode)
{
    int H=0;
    int isOpen=-1;
    int eventualX, eventualY, eventualCost;
    eventualX=currentNode.x;
    eventualY=currentNode.y;
    switch(i)
    {
    case NN:
        eventualY++;
        eventualCost=COST_FOR;
        break;
    case NE:
        eventualX++;
        eventualY++;
        eventualCost=COST_DIAG;
        break;
    case EE:
        eventualX++;
        eventualCost=COST_FOR;
        break;
    case SE:
        eventualX++;
        eventualY--;
        eventualCost=COST_DIAG;
        break;
    case SS:
        eventualY--;
        eventualCost=COST_FOR;
        break;
    case SO:
        eventualX--;
        eventualY--;
        eventualCost=COST_DIAG;
        break;
    case OO:
        eventualX--;
        eventualCost=COST_FOR;
        break;
    case NO:
        eventualX--;
        eventualY++;
        eventualCost=COST_DIAG;
        break;
    }
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
    if(currentNode.x==0 && (i==SO ||i==OO || i==NO))
        return false;
    if(currentNode.y==0 && (i==SO ||i==SS || i==SE))
        return false;
    if(currentNode.x==matrix.GetX()-1 && (i==EE ||i==NE || i==SE))
        return false;
    if(currentNode.x==matrix.GetY()-1 && (i==NE ||i==NN || i==NO))
        return false;
    int eventualX, eventualY, eventualCost;
    eventualX=currentNode.x;
    eventualY=currentNode.y;
    switch(i)
    {
    case NN:
        eventualY++;
        eventualCost=COST_FOR;
        break;
    case NE:
        eventualX++;
        eventualY++;
        eventualCost=COST_DIAG;
        break;
    case EE:
        eventualX++;
        eventualCost=COST_FOR;
        break;
    case SE:
        eventualX++;
        eventualY--;
        eventualCost=COST_DIAG;
        break;
    case SS:
        eventualY--;
        eventualCost=COST_FOR;
        break;
    case SO:
        eventualX--;
        eventualY--;
        eventualCost=COST_DIAG;
        break;
    case OO:
        eventualX--;
        eventualCost=COST_FOR;
        break;
    case NO:
        eventualX--;
        eventualY++;
        eventualCost=COST_DIAG;
        break;

    }
    if(matrix.GetValue(eventualX, eventualY))
        return false;
    if(!closedList.empty())
        for(int i=0; i<(int)closedList.size(); i++)
            if(closedList[i].x==eventualX && closedList[i].y==eventualY)
                return false;
    return true;
}

void DataFile()
{
    int carac(0);
    ifstream file("Plans/plan.bmp"); // Ouverture fichier
    ofstream data("Ressources/data.dt"); //Ouvertur du fichier de données ordonnées

    if(file&&data) // Si fichier lecture
    {
        while(carac!=EOF)
        {
            carac=file.get();
            if(carac==-1)
            {
            }
            else
            {
                data<<carac<<endl; //Affichage des octets ligne par ligne
            }
        }
        file.close();
        data.close();
    }
    else
        cout<<"!! ERROR !! Plan unreadable or missing"<<endl;
}

void LoadData()
{
    cout << "Extracting raw data..." << endl;
    DataFile();
    cout << "Analyzing file dimension..." << endl;
    long int line_wanted(18), width(0), height(0), line_found(0);
    int  carac, carac1, carac2, carac3, carac4;
    ifstream data("Ressources/data.dt");
    ofstream picture_data("Ressources/size_pixel.pdt");
    if(data&&picture_data)
    {
        do //Recherche des dimensions du fichier
        {
            carac=data.get();
            if(carac=='\n')
                line_found++;
        }while(line_found<line_wanted);
        cout << "Validating file format..." << endl;
        //La longeur
        for(int i=0; i<2; i++)
        {
            data>>carac1;
            data>>carac2;
            data>>carac3;
            data>>carac4;
            switch(i)
            {
            case 0:
                if(carac2>0)
                    width=carac2*256+carac1;
                else
                    width=carac1;
                break;
            case 1:
                if(carac2>0)
                    height=carac2*256+carac1;
                else
                    height=carac1;
                break;
            }
        }
        picture_data<<width<<endl<<height<<endl;
        data.seekg(0, ios::beg);
        line_wanted=54;
        carac=0;
        line_found=0;
        do //Aller jusqu'au début du codage pixel
        {
            carac=data.get();
            if(carac=='\n')
                line_found++;
        }
        while(line_found<line_wanted);
        cout << "Converting raw pixel data into exploitable data..." << endl;
        int end_line(0), end_column(0), pixel(0);
        while(end_column<height) //Tant que Toutes les lignes ne sont pas lues
        {
            end_line=0;
            while(end_line<width) //Tant que chaque pixel de la ligne n'est pas lu
            {
                cout << "       " << end_line << "/" << width << endl;
                pixel=0;
                for(int i=0; i<3; i++) //Pour chaque octet de chaque pixel recommencer (3 fois car 24 bits)
                {
                    data>>carac;
                    pixel+=carac;
                }
                if(pixel==765) //SI BLANC 'espace'
                    picture_data<<" ";
                if(pixel==0) //SI NOIR '#'
                    picture_data<<"#";
                end_line++;
            }
            picture_data<<endl;
            long int width_pixel(0), four_multip(0);
            width_pixel=width*3;
            while(four_multip<width_pixel)
                four_multip+=4;
            data.seekg(0, ios::beg);
            line_wanted=line_wanted+four_multip;
            carac=0;
            line_found=0;
            do
            {
                carac=data.get();
                if(carac=='\n')
                    line_found++;
                cout << "   " << line_found << "/" << line_wanted << endl;
            }
            while(line_found<line_wanted);
            end_column++;
        }
    }
    else
        cout<<"!! ERROR !! Data missing";
}
