#include "ext.hpp"
#include "couche.hpp"
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <map>
#include <vector>

using namespace Eigen;
using namespace std;

//Constructeurs/Destructeur
Couche::Couche (int nbNeurones, MatrixXd mPoids, VectorXd vBiais)
{
	this->nbNeurones = nbNeurones ;
	this->mPoids = mPoids;
	this->vBiais = vBiais;
	this->error = new double[nbNeurones];
}

Couche::Couche(int nbNeurones, VectorXd vBiais)
{
	this->nbNeurones = nbNeurones;
	this->vBiais = vBiais;
	this->mPoids.Constant(nbNeurones, 1, -1);
	this->error = new double[nbNeurones];
}

Couche::~Couche() {/*delete[](error);*/}

//Setters/Getters
unsigned int Couche::getNbNeurones() { return nbNeurones; }
double Couche::getPoids(int i, int j) { return mPoids(i, j); }
double Couche::getBiais(int i) { return vBiais[i]; }