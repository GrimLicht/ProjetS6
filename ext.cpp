#include "ext.hpp"

using namespace std;
using namespace Eigen;

//Map (etiquette)
map<int,string> mapAnimaux;
map<int,char> mapChar;
map<int,char> mapInt;

//Fonctions aléatoires
VectorXd aleaBiais(int nbNeurones)
{	
	std::uniform_real_distribution<double> unif(0.0, 1);
	std::random_device rd;
	std::default_random_engine re(rd());

	VectorXd vB(nbNeurones);
	//std::uniform_real_distribution<double> unif(0, 1);
	//std::default_random_engine re;
	
	for(int i = 0; i < nbNeurones; i++){
		vB(i) = unif(re);
	}
	return vB;
}

MatrixXd aleaPoids(int nbNeuronesSuivants, int nbNeurones)
{
	MatrixXd mP(nbNeuronesSuivants, nbNeurones);
	std::uniform_real_distribution<double> unif(0.0, 1);
	std::random_device rd;
	std::default_random_engine re(rd());
	//return distribution(generator);
	
	for(int i = 0; i < nbNeuronesSuivants; i++){
		for(int j = 0; j < nbNeurones; j++){
			
			mP(i,j) = unif(re);
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

/*void remplissage(MatrixXd *aRemplir)
{
	int row = aRemplir->rows() + 1;
	int col = aRemplir->cols() + 1;
	MatrixXd remplie = MatrixXd::Zero(row, col);
	remplie.block(0,0,row-1, col-1) = aRemplir->block(0,0,row-1, col-1);
	*aRemplir = remplie;
}*/
