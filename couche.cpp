#include "ext.cpp"
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
	this-> nbNeurones = nbNeurones ;
	this-> mPoids = mPoids;
	this-> vBiais = vBiais;
}

Couche::Couche(int nbNeurones, VectorXd vBiais)
{
	this->nbNeurones = nbNeurones;
	this->vBiais = vBiais;
	this->mPoids.Constant(nbNeurones, -1);

}

Couche::~Couche(){} //A enlever

//Setters/Getters
unsigned int Couche::getNbNeurones()
{
	return nbNeurones;
}







