#include "main.h"

using namespace std;


int main (int argc, char** argv)
{
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    Pathfinder pathfinder;
    outPut moteur(pathfinder.getMatrix());
    moteur.init_outPut();
    moteur.genList();
    vector<Node> resultList=pathfinder.find(moteur.choosePoints());
    if(resultList.empty())
    {
        cout << "ERREUR" << endl << "Le probleme n'a pas de solution" << endl << "Operation annulee" << endl;
        cout<<"\nAFFICHAGE (Vianney)\n\n";
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
        while(moteur.getStatus())
        {
            moteur.setScene();
            moteur.drawScene();
            moteur.drawResult(&resultList);
            moteur.display();
        }
        cout << "Fin de la recherche atteinte avec succes." << endl;
        cout << "Cout total : " << resultList.size() << endl;
        cout<<"\nAFFICHAGE (Vianney)\n\n";
    }
    return 0;
}