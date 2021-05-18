#ifndef RESEAU_HPP
#define RESEAU_HPP

#include "couche.hpp"
#include "ext.hpp"

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
	public :
	unsigned int nbCouches;
	vector<Couche> vCouches;
	vector<unsigned int> stats;
	unsigned int typeSim;
	double tauxApprentissage;


	public:
	//Constructeurs/Destructeur
	Reseau(Parametres p);
	Reseau(Parametres p, vector<MatrixXd> mpoids, vector<VectorXd> vbiais);
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
	void propagation(VectorXd entrees);
	int simulation(VectorXd entrees);
	string toString(int resultatSimulation);
	//Parametres chargerRN(string fichierTXT , vector<MatriXd> ∗mPoids, vector<VectorXd> ∗vBiais ); 

	//Méthode d'apprentissage
	double deriveeSigmoide(double sig);
	void calculDelta(VectorXd resultatAttendu);
	bool retropropagation(VectorXd entree, VectorXd resultatattendu);
	void entrainement(vector<VectorXd> setFichiers, vector<int> reponsesAttendues);
	VectorXd retourLabel(int label);
	vector<VectorXd> allLabels(vector<int> labels);

};

#endif /* Reseau_hpp */