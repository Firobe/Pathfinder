#include "Matrix.h"
#include <fstream>

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

void Matrix::RandMap()
{
    _x = rand()%300+201;
    _y = rand()%300+201;

    _array = new int**[_x];
    for(unsigned int i=0; i<_x+1; i++)
    {
        _array[i] = new int*[_y];
        for(unsigned int j=0; j<_y; j++)
            _array[i][j] = new int[11];
    }

    int X(30);
    for(unsigned int j=0 ; j < _y ; j++)
    {
        for(unsigned int i=0; i<_x; i++)
        {
            if(j==0)
            {
                if(i==0)
                    _array[i][j][0] = rand()%255+1;
                else
                    _array[i][j][0] = (_array[i-1][j][0] + rand()%X+1)-15;
            }
            else if(i==0)
                _array[i][j][0] = (_array[i][j-1][0] + rand()%X+1)-10;
            else
                _array[i][j][0] = ((_array[i][j-1][0]+_array[i-1][j][0]+_array[i-1][j-1][0])/3 + rand()%X+1)-15 ;
            if(_array[i][j][0]<0)
                _array[i][j][0] = 0;
            if(_array[i][j][0]>255)
                _array[i][j][0] = 255;
        }
    }
}


string Matrix::LocalMap()
{
    string fichier;
    string chaine;

    cout << endl << "Liste des Maps :" << endl << endl;
    if(SYS)
        system("dir Plans");
    else
        system("ls -C Plans");
    cout << endl << endl << "Choisir le fichier map voulu" << endl << " !! ATTENTION !!  N'ecrire que le nom (sans l'extension), en respectant la CASSE" << endl;
    cin >> chaine;
    fichier =  "Plans/" + chaine + ".bmp";
    return fichier;
}


string Matrix::Internet()
{
    char _index[50];
    string fichier;
    string chaine;

    cout<<endl<<"Donwnload index file..."<<endl<<endl;
    system("wget -N http://dreamearth.no-ip.org/MineSweeper/index");

    FILE*index=fopen("index", "r");

    if(index)
    {
        cout<<endl<<"Liste des Maps disponibles sur le serveur distant :"<<endl<<endl;
        int index_(0), i(0);
        while(index_!=-1)
        {
            index_ = fgetc(index);
            if(index_==10)
                i++;
        }
        fseek(index, 0, SEEK_SET);
        for(int j=0; j<i; j++)
        {
            fgets(_index, 50, index);
            cout<<"  " << j+1 << ". " << _index;
        }
    }
    else
    {
        system("cls");
        system("color 0C");
        cout<<"!!ERREUR!! Fichier d'index absent ou illisible"<< endl << endl;
        system("PAUSE");
        exit(EXIT_FAILURE);
    }

    cout << endl << endl << "Choisir le fichier map voulu" << endl << " !! ATTENTION !!  N'ecrire que le nom (sans l'extension), en respectant la CASSE" << endl;
    cin >> chaine;
    fichier = "Plans/" + chaine + ".bmp";

    cout << "Download map_file..." << endl << endl;
    chaine = "wget -N -PPlans http://dreamearth.no-ip.org/MineSweeper/" + chaine + ".bmp";
    system(chaine.c_str());

    return fichier;
}


void Matrix::DataFile()
{
    system("color 00");
    string fichier;
    int _c_(0);
    srand(time(0));

    cout << "Utilisation de map :" << endl << "    Generation d'une map aleatoire --> 0\n    Map existante : Local          --> 1\n    Map existante : Internet       --> 2\n    Sortir du Programme            --> 3" << endl << endl;
    do
    {
        cin >> _c_;
    }
    while(-1<_c_&&_c_>4);

    switch(_c_)
    {
    case 0:
        Matrix::RandMap();
        break;
    case 1:
        fichier = Matrix::LocalMap();
        break;
    case 2:
        fichier = Matrix::Internet();
        break;
    case 3:
        exit(EXIT_FAILURE);
        break;
    }

    if(_c_==1||_c_==2)
    {
        unsigned long int carac_wanted(0), carac_found(0), pixel(0);
        unsigned long int carac(0), carac1, carac2, carac3, carac4;

        ifstream file(fichier.c_str(), ios::binary);

        if(file) // Si fichier alors lecture
        {
            cout << "Extracting raw datadata..." << endl;

            file.seekg(0, ios::beg);
            carac_wanted = 18;
            do //Recherche des dimensions du fichier
            {
                carac=file.get();
                carac_found++;
            }
            while(carac_found<carac_wanted);

            ///LECTURE DE L'IMAGE
            for(int i=0; i<2; i++)
            {
                carac1 = file.get();
                carac2 = file.get();
                carac3 = file.get();
                carac4 = file.get();
                switch(i)
                {
                case 0:
                    if(carac3>0)
                        _x = carac3*256*256+carac2*256+carac1;
                    else if(carac2>0)
                        _x = carac2*256+carac1;
                    else
                        _x = carac1;
                    break;
                case 1:
                    if(carac3>0)
                        _y = carac3*256*256+carac2*256+carac1;
                    else if(carac2>0)
                        _y = carac2*256+carac1;
                    else
                        _y = carac1;
                    break;
                }
            }
            _array=new int**[_x];
            for(unsigned int i=0; i<_x+1; i++)
            {
                _array[i]=new int*[_y];
                for(unsigned int j=0; j<_y; j++)
                    _array[i][j]=new int[11];
            }
            cout<<"Extracting grey levels..."<<endl;
            file.seekg(0, ios::beg);
            carac_wanted=54;
            carac=0;
            carac_found=0;
            do //Recherche des dimensions du fichier
            {
                carac=file.get();
                carac_found++;
            }
            while(carac_found<carac_wanted);

            unsigned long int width_pixel(0), four_multip(0);
            width_pixel=_x*3;
            while(four_multip<width_pixel)
                four_multip+=4;
            carac_wanted=four_multip-width_pixel;

            for(unsigned int j=0 ; j < _y ; j++)
            {
                for(unsigned int i=0; i<_x; i++)
                {
                    pixel=0;
                    for(unsigned int k=0; k<3; k++) //Pour chaque octet de chaque pixel recommencer (3 fois car 24 bits)
                    {
                        carac=file.get();
                        pixel+=carac;
                    }
                    pixel=pixel/3;
                    _array[i][j][0]=pixel;
                }

                carac=0;
                carac_found=0;
                do
                {
                    carac=file.get();
                    carac_found++;
                }
                while(carac_found<carac_wanted);

            }
            file.close();
        }

        else
        {
            system("cls");
            system("color 0C");
            cout << "!! ERREUR !! Plan illisible, manquant ou nom de fichier mal saisit" << endl << endl;
            system("PAUSE");
            system("cls");
            system("Pathfinder.exe");
            exit(EXIT_FAILURE);
        }
    }

    cout << "Converting grey levels into exploitable array..." << endl;
}

void Matrix::calcWalls(int maxDiff)
{
    for(unsigned int i=0; i<_x; i++)
        for(unsigned int j=0; j<_y; j++)
            for(unsigned int k=1; k<9; k++)
            {
                //Si la case du tableau correspondant au pixel testĂŠ existe
                if(i+decalXY[k][0]>=0 && i+decalXY[k][0]<=_x-1 && j+decalXY[k][1]>=0 && j+decalXY[k][1]<=_y-1)
                {
                    if(abs(_array[i][j][0]-_array[i+decalXY[k][0]][j+decalXY[k][1]][0])>maxDiff)
                        _array[i][j][k]=1;
                    else
                        _array[i][j][k]=0;
                }
                else
                    _array[i][j][k]=1;
            }
}

