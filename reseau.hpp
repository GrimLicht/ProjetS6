#ifndef RESEAU_HPP
#define RESEAU_HPP

#include "couche.hpp"
#include "ext.hpp"

#include "gestionnaireMemoire.hpp"
#include "MainWindow.hpp"

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <map>
#include <vector>
#include <string>

using namespace Eigen;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;


class Reseau
{
	private :
	unsigned int nbCouches;
	vector<Couche> vCouches;
	vector<unsigned int> stats;
	unsigned int typeSim;
	double tauxApprentissage;


	public:
	//Constructeurs/Destructeur
	Reseau(Parametres p);
	Reseau(Parametres p, std::vector<MatrixXd> mpoids, std::vector<VectorXd> vbiais);
	~Reseau();

	//Setters/Getters
	unsigned int getNbCouches();
	vector<MatrixXd> getPoids();
	vector<unsigned int> getStats();

	//Methode de tests
	void printReseau();
	
	//Méthodes de RNU
	//double sigmoide(double entree);
	int max(VectorXd sorties);
	VectorXd propagation(VectorXd entrees);
	int simulation(VectorXd entrees);
	string toString(int resultatSimulation);
	//Parametres chargerRN(string fichierTXT , vector<MatriXd> ∗mPoids, vector<VectorXd> ∗vBiais ); 

	//Méthode d'apprentissage
	double deriveeSigmoide(double sig);
	VectorXd calculDelta1(VectorXd resultatAttendu);
	bool verificationDelta(VectorXd delta);
	MatrixXd calculDelta2(VectorXd delta, MatrixXd poids);
	void miseAJour(MatrixXd delta, VectorXd activation);
	bool retropropagation();
	void entrainement(vector<double*> setFichiers);

};

#endif /* Reseau_hpp */