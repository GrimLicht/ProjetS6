#include <eigen3/Eigen/Dense>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

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
 
void compression(MatrixXd *aCompresser, int nbNeurones)
{
	int row = aCompresser->rows();
	int col = aCompresser->cols(); 
	
	while(row*col > nbNeurones)
	{
		row--; col--;
		MatrixXd compressee = MatrixXd::Zero(row, col);
		for(int i = 0; i < row; i++)
		{
			for(int j = 0; j < col; j++) 
				compressee(i,j) = ((*aCompresser)(i,j) + (*aCompresser)(i+1,j) + (*aCompresser)(i,j+1) + (*aCompresser)(i+1,j+1))/4; 
		}
		*aCompresser = compressee;//la matrice deviens donc une matrice avec une taille (lignes -1, colonnes -1)
	}
}

std::stringstream hexStr(char* data, int len)
{
	std::stringstream ss;
	ss << std::hex;
	for(int i = 0; i < len; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];
	return ss;
}

/*void remplissage(MatrixXd *aRemplir)
{
	int row = aRemplir->rows() + 1;
	int col = aRemplir->cols() + 1;
	MatrixXd remplie = MatrixXd::Zero(row, col);
	remplie.block(0,0,row-1, col-1) = aRemplir->block(0,0,row-1, col-1);
	*aRemplir = remplie;
}*/