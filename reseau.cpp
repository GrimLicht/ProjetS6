#include "couche.hpp"
#include "reseau.hpp"
#include "ext.hpp"
#include "gestionnaireMemoire.hpp"
#include "MainWindow.hpp"

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <map>
#include <vector>
#include <cmath>
using namespace Eigen;
using namespace std;


Reseau::Reseau(Parametres p)
{
	this->typeSim = p.typeSim;
	this->nbCouches = p.nbCouches;
	this->tauxApprentissage = p.tauxApprentissage;
	this->vCouches.reserve(this->nbCouches);
	
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
	this->vCouches.reserve(this->nbCouches);
	
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
vector<unsigned int> Reseau::getStats(){return this->stats;};

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

double sigmoide(double entree) //apparently slow
{
	double sortie;
	double dividend = exp(-entree);
	dividend += 1;
	sortie = 1 / dividend;

	return sortie;
}

double fast_sigmoide(double entree)
{
	double sortie;
	double diviseur = abs(entree);
	diviseur += 1;
	sortie = 1/diviseur;

	return sortie;
}

VectorXd Reseau::propagation(VectorXd entrees) 
{

	MatrixXd mult;
	VectorXd transfer; //le vector qui passe d'une couche a une autre
	//premiere couche shmilblick
	for(int i = 0; i < nbCouches; i++) //boucle pour TOUTES les couches cachees
	{
		/*
		Pour i de 1 à n neurones de la couche prec
			pour j de 1 à m neurones suiv ->actuelle
				mult[i][j] <— activation[i] * poids[j][i]
		*/
		mult = MatrixXd::Zero(vCouches[i-1].getNbNeurones(), vCouches[i].getNbNeurones()); //le getters est inutile, vu qu'on est en friends tho
		mult = vCouches[i-1].mPoids * vCouches[i-1].vActivation; 

		/*
		Pour i de 1 à n neurones prec
			Pour j de 1 à m neurones suiv
				somme[i] <— somme[i] + mult[i][j]
		*/
		for(int n = 0; n < vCouches[i].getNbNeurones(); n++)
		{
			for(int m = 0; m < vCouches[i-1].getNbNeurones(); m++)
			{
				transfer[n] += mult(m,n);
			}
		}

		/*
		Pour i de 1 à n neurone prec 
			pactivation[i]<— somme[i] + biais[i]
			activation[i] <— sigmoide(pactivation[i])
		*/
		for(int n = 0; n < vCouches[n].nbNeurones; n++) 
		{
			vCouches[i].vActivation[n] = sigmoide(transfer[n] + vCouches[i].vBiais[n]); //modify the activation vector
		}

	}

	return vCouches[nbCouches-1].vActivation; //retourne le calcul sigmoide de la derniere couche
}


void Reseau::printReseau()
{
	for(int i = 0; i < nbCouches; i++)
	{
		std::cout << "Matrice " << i << " :\n" << vCouches[i].mPoids << std::endl << std::endl;
	}

	for(int i = 0; i < nbCouches; i++)
	{
		std::cout << "Vecteur " << i << " :\n" << vCouches[i].vBiais << std::endl << std::endl;
	}
}