#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
    srand(time(NULL));
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    bool restart(false);

    Pathfinder pathfinder;
    cout << "Pathfinder cree";
    outPut moteur(pathfinder.getMatrix());
    moteur.init_outPut();
    vector<int*> bombs = moteur.choosePoints();

        vector<Node> resultList;
        vector<Node>* resultPointer;
        if(moteur.getStatus().running)
        resultList=pathfinder.find(bombs, moteur);

        if(resultList.empty() && moteur.getStatus().running)
        {
            cout << "Erreur" << endl << "Le probleme n'a pas de solution" << endl << "Annulation de l'operation" << endl;
            resultPointer = NULL;
        }
        else
        {
            moteur.drawResult(&resultList);
            cout << "La recherche de chemin est une reussite" << endl;
            resultPointer = &resultList;
        }
        while(moteur.getStatus().running)
        {
            moteur.setScene();
            moteur.drawScene();
            moteur.drawResult(resultPointer);
            moteur.display();
        }
    return 0;
}
