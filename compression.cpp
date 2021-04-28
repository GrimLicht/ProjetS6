#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <memory>

void compression(Eigen::MatrixXd *aCompresser, int nbNeurones)
{
	int row = aCompresser->rows();
	int col = aCompresser->cols(); 
	
	while(row*col > nbNeurones)
	{
		row--; col--;
		Eigen::MatrixXd compressee = Eigen::MatrixXd::Zero(row, col);
		for(int i = 0; i < row; i++)
		{
			for(int j = 0; j < col; j++) 
				compressee(i,j) = ((*aCompresser)(i,j) + (*aCompresser)(i+1,j) + (*aCompresser)(i,j+1) + (*aCompresser)(i+1,j+1))/4; 
		}
		*aCompresser = compressee;//la matrice deviens donc une matrice avec une taille (lignes -1, colonnes -1)
	}
}

void remplissage(Eigen::MatrixXd *aRemplir)
{
	int row = aRemplir->rows() + 1;
	int col = aRemplir->cols() + 1;
	Eigen::MatrixXd remplie = Eigen::MatrixXd::Zero(row, col);
	remplie.block(0,0,row-1, col-1) = aRemplir->block(0,0,row-1, col-1);
	*aRemplir = remplie;
}

int main()
{
	Eigen::MatrixXd m = Eigen::MatrixXd::Random(20, 20);
	std::cout << "Here is m at the start :\n" << m << std::endl;
	//std::cout << m.block(1,1,1,1) << std::endl << "End of m" << std::endl;
	//std::cout << m(1,1) << std::endl;

	compression(&m, 100);
	std::cout << "Here is m after compression :\n" << m << std::endl;
	
	//remplissage(&m);
	//std::cout << "Matrice m remplie\n" << m << std::endl;
	//mat = Eigen::MatrixXd::Random(12,10);
	//std::cout << mat << std::endl;
}