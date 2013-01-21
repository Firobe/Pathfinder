#include "Matrix.h"
#include <fstream>

Matrix::Matrix()
{

}

Matrix::~Matrix()
{
    //dtor
}

void Matrix::Load(const char* path)
{
    int carac;
    ifstream file(path);
    if (file.is_open())
    {
        file >> _y;
        file >> _x;
        _array = new bool*[_x];
        for(int i=0; i<_x; i++)
            _array[i] = new bool[_y];
        file.get();
        for(int i=0; i<_x; i++)
        {
            for(int j=0; j<_y; j++)
            {
                carac = file.get();
                switch(carac)
                {
                case ' ':
                    _array[_x-i-1][j]=false;
                    break;
                default:
                    _array[_x-i-1][j]=true;
                }
            }
            file.get();
        }
        file.close();
    }
    else
        cout << "Erreur : fichier introuvable" << endl;
}

bool Matrix::GetValue(int x, int y)
{
    return _array[x][y];
}

int Matrix::GetX()
{
    return _x;
}
int Matrix::GetY()
{
    return _y;
}
