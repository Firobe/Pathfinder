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
        cout << "ERROR" << endl << "The problem has no solution." << endl << "Operation aborted" << endl;
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
        cout << endl << "Writing result..." << endl;
        while(moteur.getStatus())
        {
            moteur.setScene();
            moteur.drawScene();
            moteur.drawResult(&resultList);
            moteur.display();
        }
        cout << "Taille de la liste : " << resultList.size() << endl;

        cout << "Pathfinding ended successfully" << endl;
    }
    return 0;
}
