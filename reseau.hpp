#ifndef Reseau_hpp
#define Reseau_hpp

#include "couche.hpp"
#include "ext.cpp"

#include "gestionnaireMemoire.hpp"
//#include "MainWindow.hpp"

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
	Reseau(Parametres p, vector<MatrixXd> mpoids, vector<VectorXd> vbiais);
	~Reseau();

	//Setters/Getters
	unsigned int getNbCouches();
	vector<MatrixXd> getPoids();


	//Méthodes de RNU
	double sigmoide(double entree);
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
	void entrainement(vector<double *pixel> setFichiers);

};

#endif /* Reseau_hpp */