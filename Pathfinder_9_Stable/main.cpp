#include "main.h"

using namespace std;

int main (int argc, char** argv)
{
    cout<<"Hey!";
    srand(time(NULL));
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    Pathfinder pathfinder;
    outPut moteur(pathfinder.getMatrix());
    moteur.init_outPut();
    vector<int*> bombs = moteur.choosePoints();
    vector<Node> resultList;
    if(moteur.getStatus())
     resultList=pathfinder.find(bombs, moteur);

    if(resultList.empty())
    {
        moteur.recalc();
        cout << "!! ERREUR !!" << endl << "Aucun parcours valide pour une difference max de "<<MAX_DIFF<<endl<<"La situation n'a aucune solution" << endl;
        while(moteur.getStatus())
        {
            moteur.setScene();
            moteur.drawScene();
            moteur.display();
        }
    }
    else
    {
        moteur.drawResult(&resultList);
        moteur.recalc();
        cout<<"Succes du calcul !"<<endl;
        while(moteur.getStatus())
        {
            moteur.setScene();
            moteur.drawScene();
            moteur.drawResult(&resultList);
            moteur.display();
        }

        cout << "Pathfinding ended successfully" << endl;
    }
    return 0;
}
