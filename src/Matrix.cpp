#include "Matrix.h"
#include <fstream>
#include <windows.h>

using namespace std;

Matrix::Matrix()
{

}

Matrix::~Matrix()
{
    //dtor
}

bool Matrix::GetValue(int x, int y, int d, int i)
{
    return _array[x][y][d][i];
}

int Matrix::GetX()
{
    return _x;
}
int Matrix::GetY()
{
    return _y;
}

void Matrix::DataFile()
{
    int carac(0);
    ifstream file("Plans/plan.bmp"); // Ouvertur fichier
    ofstream data("Ressources/data.dt"); //Ouvertur du fichier de données ordonnées

    if(file&&data) // Si fichier lecture
    {
        cout << "Extracting raw data..." << endl;
        while(carac!=EOF)
        {
            carac=file.get();
            if(carac!=-1)
                data<<carac<<endl; //Affichage des octets ligne par ligne
        }
        file.close();
        data.close();
    }
    else
    {
        cout<<"!! ERROR !! Plan unreadable or missing"<<endl;
    }
}
void Matrix::LoadData()
{
    DataFile();
    cout << "Reading raw data..." << endl;
    unsigned long int line_wanted(18), width(0), height(0), line_found(0);
    int  carac, carac1, carac2, carac3, carac4;
    ifstream data("Ressources/data.dt");
    ofstream grey_data("Ressources/grey_lvl.pdt");
    if(data&&grey_data)
    {
        do //Recherche des dimensions du fichier
        {
            carac=data.get();
            if(carac=='\n')
                line_found++;
        }
        while(line_found<line_wanted);
        //La longeur
        ///LECTURE DE L'IMAGE
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
                ifstream data("Ressources/data.dt");
                ofstream grey_data("Ressources/gris_lvl.pdt");
            }
        }
        grey_data<<width<<endl<<height<<endl;
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
        cout << "Converting raw data into raw grey levels..." << endl;
        unsigned long int end_line(0), end_column(0), pixel(0);
        _x = width;
        _y = height;
        ///DETECTION DES NIVEAUX DE GRIS
        end_column=0;
        while(end_column<height) //Tant que Toutes les lignes ne sont pas lues
        {
            end_line=0;
            while(end_line<width) //Tant que chaque pixel de la ligne n'est pas lu
            {
                pixel=0;
                for(int i=0; i<3; i++) //Pour chaque octet de chaque pixel recommencer (3 fois car 24 bits)
                {
                    data>>carac;
                    pixel+=carac;
                }
                pixel=pixel/3;
                grey_data<<pixel;
                end_line++;
                if(end_line!=width)
                    grey_data<<"*";

            }
            grey_data<<endl;
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
            }
            while(line_found<line_wanted);
            end_column++;
        }
        grey_data.close();
    }
    else
    {
        cout<<"!! ERROR !! Data missing";
    }
    ///DETECTION DES PENTES
    FinalData();
    ///ECRITURE DES INFOS POUR EXPLOITATION
    cout<<"Converting ended successfully." <<endl;
}

void Matrix::FinalData()
{
    ifstream grey_data("Ressources/grey_lvl.pdt");
    if(grey_data)
    {
        unsigned long int carac(0), line_wanted(2), line_found(0);
        int width(0), height(0), end_height(0), end_width(0), x(0), y(0),discard[9];
        grey_data>>width;
        grey_data>>height;
        _array=new int***[width];
        for(int i=0; i<width+1; i++)
        {
            _array[i]=new int**[height];
            for(int j=0; j<height; j++)
            {
                _array[i][j]=new int*[9];
                for(int k=0; k<9; k++)
                {
                    _array[i][j][k]=new int[4];
                    _array[i][j][k][3]=0;
                    switch(k)
                    {
                    case 1:
                        _array[i][j][k][0] = 0;
                        _array[i][j][k][1] = 1;
                        _array[i][j][k][2] = COST_FOR;
                        break;
                    case 2:
                        _array[i][j][k][0] = 1;
                        _array[i][j][k][1] = 1;
                        _array[i][j][k][2] = COST_DIAG;
                        break;
                    case 3:
                        _array[i][j][k][0] = 1;
                        _array[i][j][k][1] = 0;
                        _array[i][j][k][2] = COST_FOR;
                        break;
                    case 4:
                        _array[i][j][k][0] = 1;
                        _array[i][j][k][1] = -1;
                        _array[i][j][k][2] = COST_DIAG;
                        break;
                    case 5:
                        _array[i][j][k][0] = 0;
                        _array[i][j][k][1] = -1;
                        _array[i][j][k][2] = COST_FOR;
                        break;
                    case 6:
                        _array[i][j][k][0] = -1;
                        _array[i][j][k][1] = -1;
                        _array[i][j][k][2] = COST_DIAG;
                        break;
                    case 7:
                        _array[i][j][k][0] = -1;
                        _array[i][j][k][1] = 0;
                        _array[i][j][k][2] = COST_FOR;
                        break;
                    case 8:
                        _array[i][j][k][0] = -1;
                        _array[i][j][k][1] = 1;
                        _array[i][j][k][2] = COST_DIAG;
                        break;

                    }
                }
            }
        }
        cout<<"Extracting grey levels..."<<endl;
        end_height=height-1;
        while(end_height>-1)
        {
            for(int i=0; i<width; i++)
            {
                grey_data>>carac;
                _array[i][end_height][0][0]=carac;
                carac=grey_data.get();
                if(carac=='\n')
                {
                    line_wanted++;
                    carac=0;
                    line_found=0;
                    grey_data.seekg(0, ios::beg);
                    do //Aller jusqu'au début du codage pixels
                    {
                        carac=grey_data.get();
                        if(carac=='\n')
                            line_found++;
                    }
                    while(line_found<line_wanted);
                }
            }
            end_height--;
        }
        grey_data.close();
        cout << "Converting grey levels into exploitable array..."<<endl;
        for(int i=0; i<width; i++)
            for(int j=0; j<height; j++)
                for(int k=1; k<9; k++)
                {
                    //Si la case du tableau correspondant au pixel testé existe
                    if(i+_array[i][j][k][0]>=0 && i+_array[i][j][k][0]<=width-1 && j+_array[i][j][k][1]>=0 && j+_array[i][j][k][1]<=height-1)
                    {
                        if(abs((double)(_array[i][j][0][0]-_array[i+_array[i][j][k][0]][j+_array[i][j][k][1]][0][0]))>0)
                            _array[i][j][k][3]=1;
                        else
                            _array[i][j][k][3]=0;
                    }
                    else
                        _array[i][j][k][3]=1;
                }

    }
    else
    {
        cout<<"!! ERROR !! Data missing";
    }
system("pause");
}
