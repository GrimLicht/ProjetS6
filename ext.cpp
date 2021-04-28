#include <eigen3/Eigen/Dense>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;
using namespace Eigen;

//Map (etiquette)
map<int,string> mapAnimaux;
map<int,char> mapChar;
map<int,char> mapInt;

//Structure Parametres
typedef struct Parametres
{
	unsigned int typeSim;
	unsigned int nbCouches;
	unsigned int nbNeuronesEntree;
	unsigned int nbNeuronesCache;
	unsigned int nbNeuronesSortie;
	double tauxApprentissage;
}Parametres;




VectorXd aleaBiais(int nbNeurones){
    VectorXd vB(nbNeurones);
    srand(time(NULL));
    for(int i = 0; i < nbNeurones; i++){
        vB(i) = ( rand() /rand() )% 1;
    }
    return vB;
}

MatrixXd aleaPoids(int nbNeurones, int nbNeuronesSuivants){
    MatrixXd mP(nbNeurones, nbNeuronesSuivants);
    srand(time(NULL));
    for(int i = 0; i < nbNeurones; i++){
        for(int j = 0; j < nbNeuronesSuivants; j++){
            mP(i,j) = (rand() /rand()) % 1;
        }
    }
    return mP;
}
 
