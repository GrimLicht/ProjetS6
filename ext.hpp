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
#include <ctime>

//Structure Parametres
struct Parametres
{
	unsigned int typeSim;
	unsigned int nbCouches;
	unsigned int nbNeuronesEntree;
	unsigned int nbNeuronesCache;
	unsigned int nbNeuronesSortie;
	double tauxApprentissage;
};

struct Image;


Image compression(Image i, int nbNeurones);

std::string resToString(int resultatSimulation, int typeSim);
Eigen::VectorXd aleaBiais(int nbNeurones);
Eigen::MatrixXd aleaPoids(int nbNeurones, int nbNeuronesSuivants);

#endif