#include "couche.hpp"
#include "reseau.hpp"
#include "ext.cpp"
#include "gestionnaireMemoire.hpp"
#include "MainWindow.hpp"

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <map>
#include <vector>
using namespace Eigen;
using namespace std;


Reseau::Reseau(Parametres p)
{
	this->typeSim = p.typeSim;
	this->nbCouches = p.nbCouches;
	this->tauxApprentissage = p.tauxApprentissage;
	this->vCouches.resize(this->nbCouches);
	
	//Couche d'entrée
	this->vCouches.emplace_back(Couche(p.nbNeuronesEntree, aleaPoids(p.nbNeuronesEntree, p.nbNeuronesCache), aleaBiais(p.nbNeuronesEntree)));
	
	//Couches cachées-1
	for(int i = 1; i < (this->nbCouches-2); i++)
		this->vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesCache, p.nbNeuronesCache), aleaBiais(p.nbNeuronesCache)));
	
	//Couche cachée; avant-dernière
	int i = 1;//ajout
	this->vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesCache, p.nbNeuronesSortie), aleaBiais(p.nbNeuronesCache)));
	
	//Couche de sortie
	this->vCouches.emplace_back(Couche(p.nbNeuronesSortie, aleaBiais(p.nbNeuronesSortie)));
	
}

Reseau::Reseau(Parametres p, vector<MatrixXd> mPoids, vector<VectorXd> vBiais)
{
	this->typeSim = p.typeSim;
	this->nbCouches = p.nbCouches;
	this->tauxApprentissage = p.tauxApprentissage;
	this->vCouches.resize(this->nbCouches);
	
	//Couche d'entrée
	
	this->vCouches.emplace_back(Couche(p.nbNeuronesEntree, mPoids[0], vBiais[0]));
	
	//Couches cachées-1
	for(int i = 1; i < (this->nbCouches-2); i++)
	{
		this->vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[i], vBiais[i]));
	}
	
	//Couche cachée; avant-dernière
	this->vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[this->nbCouches-2], vBiais[this->nbCouches-2]));
	
	//Couche de sortie
	this->vCouches.emplace_back(Couche(p.nbNeuronesSortie, vBiais[this->nbCouches-1]));
}

//Destructeur
Reseau::~Reseau(){}

//Getteurs
unsigned int Reseau::getNbCouches(){return this->nbCouches;};

vector<MatrixXd> Reseau::getPoids()
{
	vector<MatrixXd> poids;
	poids.resize(this->nbCouches);//mettre taille  au vec
	
	for(int i = 0; i < this->nbCouches; i++){
		poids.push_back(this->vCouches[i].mPoids);
	}
	return poids;
};

//Méthodes du RNU
int Reseau::max(VectorXd sorties) //permet d'avoir l'indice de la valeur max
{ 
	int max = 0;
	int indice = -1;
	for (int i = 0; i < (sorties.size()); i++)
	{ 
		if(max < sorties[i])
		{
			max = sorties[i];
			indice = i;
		}
	}
	return indice;
}

int Reseau::simulation(VectorXd entrees) /*applique la propa + max */
{ 
	VectorXd v;
	v = this->propagation(entrees);
	return this->max(v); 
}
