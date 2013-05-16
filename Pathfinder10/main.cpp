#include "main.h"

using namespace std;

int main (int argc, char** argv)
{
    srand(time(NULL));
    cout << "Pathfinder V2 : THE DEMINER"<< endl << "---------------------------" <<endl;
    Pathfinder pathfinder;
    outPut moteur(pathfinder.getMatrix());
    moteur.init_outPut();
    vector<int*> bombs = moteur.choosePoints();

    while(moteur.getStatus().running)
    {
        cout<<moteur.getStatus().running;

        vector<Node> resultList;
        vector<Node>* resultPointer;
        if(moteur.getStatus().running)
            resultList=pathfinder.find(bombs, moteur);

        if(resultList.empty())
        {
            cout << "ERROR" << endl << "The problem has no solution." << endl << "Operation aborted" << endl;
            resultPointer = NULL;
        }
        else
        {
            moteur.drawResult(&resultList);
            cout << endl << "Writing result..." << endl;
            cout << "Taille de la liste : " << resultList.size() << endl;
            cout << "Pathfinding ended successfully" << endl;
            resultPointer = &resultList;
        }
        while(moteur.getStatus().running)
        {
            moteur.setScene();
            moteur.drawScene();
            moteur.drawResult(resultPointer);
            moteur.display();
        }
    }
    return 0;
}
