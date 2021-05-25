#ifndef COUCHE_HPP
#define COUCHE_HPP

#include "ext.hpp"

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <map>
#include <vector>
#include <string>

using namespace std;
using namespace Eigen;
using Eigen::MatrixXd;
using Eigen::VectorXd;

class Couche
{
	public:
	friend class MainWindow;
	friend class Reseau;
	//friend class ext;
	//friend void sauvegardeRN(Reseau r, string chemin);

	private :
	unsigned int nbNeurones;
	double* error;
	VectorXd vActivation;
	VectorXd vBiais;
	MatrixXd mPoids;

	public:
	//Constructeurs/Destructeur
	Couche (int nbNeurones, MatrixXd mPoids, VectorXd vBiais);
	Couche(int nbNeurones, VectorXd vBiais);
	~Couche();

	//Setters/Getters
	unsigned int getNbNeurones();
	double getPoids(int i, int j);
	double getBiais(int i);

	//Méthodes de RNU
	//MatrixXd aleaPoids();
	//MatrixXd aleaBiais();
};

#endif  //Couche_hpp 