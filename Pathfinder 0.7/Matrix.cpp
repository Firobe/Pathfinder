#include "Matrix.h"
#include <fstream>
//#include <windows.h>

using namespace std;

Matrix::Matrix()
{

}

Matrix::~Matrix()
{
    //dtor
}

const int Matrix::GetValue(const int x, const int y, const int d)
{
    return _array[x][y][d];
}

const int Matrix::GetX()
{
    return _x;
}
const int Matrix::GetY()
{
    return _y;
}

void Matrix::DataFile()
{
    int carac(0);
    ifstream file; // Ouverture fichier
    file.open("Plans/plan.bmp", ios::binary);
    ofstream data("Ressources/data.dt"); //Ouvertur du fichier de donnĂŠes ordonnĂŠes

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

    FinalData();
}

void Matrix::FinalData()
{

    unsigned long int line_wanted(18), width(0), height(0), line_found(0), pixel(0);
    unsigned long int carac, carac1, carac2, carac3, carac4;
    ifstream data("Ressources/data.dt");
    if(data)
    {
        do //Recherche des dimensions du fichier
        {
            carac=data.get();
            if(carac=='\n')
                line_found++;
        }
        while(line_found<line_wanted);

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
            }
        }
        _x = width;
        _y = height;

        _array=new int**[width];
        for(unsigned int i=0; i<width+1; i++)
        {
            _array[i]=new int*[height];
            for(unsigned int j=0; j<height; j++)
                _array[i][j]=new int[11];
        }
        cout<<"Extracting grey levels..."<<endl;
        data.seekg(0, ios::beg);
        line_wanted=54;
        carac=0;
        line_found=0;
        do //Aller jusqu'au dĂŠbut du codage pixel
        {
            carac=data.get();
            if(carac=='\n')
                line_found++;
        }
        while(line_found<line_wanted);
        for(unsigned int j=0 ; j < height ; j++)
        {
            for(unsigned int i=0; i<width; i++)
            {
                pixel=0;
                for(unsigned int k=0; k<3; k++) //Pour chaque octet de chaque pixel recommencer (3 fois car 24 bits)
                {
                    data>>carac;
                    pixel+=carac;
                }
                pixel=pixel/3;
                _array[i][j][0]=pixel;
            }
            unsigned long int width_pixel(0), four_multip(0);
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
        }
        data.close();

        cout << "Converting grey levels into exploitable array..."<<endl;
        calcWalls();
    }
    else
    {
        cout<<"!! ERROR !! Data missing";
    }
}
void Matrix::calcWalls()
{
    for(int i=0; i<_x; i++)
        for(int j=0; j<_y; j++)
            for(int k=1; k<9; k++)
            {
                //Si la case du tableau correspondant au pixel testĂŠ existe
                if(i+decalXY[k][0]>=0 && i+decalXY[k][0]<=_x-1 && j+decalXY[k][1]>=0 && j+decalXY[k][1]<=_y-1)
                {
                    if(abs((double)(_array[i][j][0]-_array[i+decalXY[k][0]][j+decalXY[k][1]][0]))>MAX_DIFF)
                        _array[i][j][k]=1;
                    else
                        _array[i][j][k]=0;
                }
                else
                    _array[i][j][k]=1;
            }
}
