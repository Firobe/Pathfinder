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

void Matrix::DataFile()
{
    unsigned long int carac_wanted(0), width(0), height(0), carac_found(0), pixel(0);
    unsigned long int carac(0), carac1, carac2, carac3, carac4;
    char _index[50];
    bool choice;
    string chaine;
    string fichier;

    cout<<"Utilisation interface reseau:" << endl <<"   Internet --> 0\n   Local    --> 1"<<endl<<endl;

    int _c_(0);
    do
    {
        cin>>_c_;
    }
    while(_c_!=0&&_c_!=1);
    if(_c_==0)
        choice=0;
    else
        choice=1;

    if(choice)
    {
        cout << endl << "Liste des Maps :" << endl << endl;
        if(SYS)
            system("dir Plans");
        else
            system("ls -C Plans");
        cout << endl << endl << "Choisir le fichier map voulu (ATTENTION: Respecter la CASSE)"<<endl;
        cin >> chaine;
        fichier =  "Plans/" + chaine + ".bmp";
    }
    else
    {
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
            cout<<"!!ERREUR!! Fichier d'index absent ou illisible"<<endl;

    cout<<endl<<endl<<"Choisir le fichier map voulu" << endl <<"!! ATTENTION !! :  N'ecrire que le nom, en respectant la CASSE" << endl;
    cin>>chaine;
    fichier="Plans/" + chaine + ".bmp";

    cout<< "Download map_file..."<<endl<<endl;
    chaine = "wget -N -PPlans http://dreamearth.no-ip.org/MineSweeper/" + chaine + ".bmp";
    system(chaine.c_str());
    }

    ifstream file(fichier.c_str(), ios::binary);

    if(file) // Si fichier lecture
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
            carac1=file.get();
            carac2=file.get();
            carac3=file.get();
            carac4=file.get();
            switch(i)
            {
            case 0:
                if(carac3>0)
                    width=carac3*256*256+carac2*256+carac1;
                else if(carac2>0)
                    width=carac2*256+carac1;
                else
                    width=carac1;
                break;
            case 1:
                if(carac3>0)
                    height=carac3*256*256+carac2*256+carac1;
                else if(carac2>0)
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
        width_pixel=width*3;
        while(four_multip<width_pixel)
                four_multip+=4;
        carac_wanted=four_multip-width_pixel;

        for(unsigned int j=0 ; j < height ; j++)
        {
            for(unsigned int i=0; i<width; i++)
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
            cout<<"!! ERROR !! Plan unreadable or missing"<<endl;
        }

            cout << "Converting grey levels into exploitable array..."<<endl;
            for(unsigned int i=0; i<width; i++)
                for(unsigned int j=0; j<height; j++)
                    for(unsigned int k=1; k<9; k++)
                    {
                        //Si la case du tableau correspondant au pixel testĂŠ existe
                        if(i+decalXY[k][0]>=0 && i+decalXY[k][0]<=width-1 && j+decalXY[k][1]>=0 && j+decalXY[k][1]<=height-1)
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

void Matrix::CloseCell(const int x, const int y, const int Px, const int Py)
{
    _array[x][y][0]=-1;
    _array[x][y][9]=Px;
    _array[x][y][10]=Py;
}
