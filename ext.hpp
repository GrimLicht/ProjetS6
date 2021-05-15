#ifndef EXT_HPP
#define EXT_HPP

#include <eigen3/Eigen/Dense>
#include <vector>
#include <string>
#include <random>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

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

void compression(Eigen::MatrixXd* aCompresser, int nbNeurones);

Eigen::VectorXd aleaBiais(int nbNeurones);

Eigen::MatrixXd aleaPoids(int nbNeurones, int nbNeuronesSuivants);

#endif