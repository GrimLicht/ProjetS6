#include <iostream>
#include <QApplication>
#include <QDebug>

#include "MainWindow.hpp"
#include "couche.hpp"
#include "reseau.hpp"
#include "gestionnaireMemoire.hpp"
#include "ext.hpp"

int main(int argc, char **argv)
{
	Parametres param;
	param.typeSim = 1;
	param.nbCouches = 4;
	param.nbNeuronesEntree = 4;
	param.nbNeuronesCache = 10;
	param.nbNeuronesSortie = 2;
	param.tauxApprentissage = 0.50;

	Reseau res(param);



	//MatrixXd wagon = aleaPoids(param.nbNeuronesCache, param.nbNeuronesSortie);
	//std::cout << wagon << std::endl;

	//res.printReseau();

	VectorXd propa; propa = aleaBiais(4);

	std::cout << "L'ERREUR N'EST PAS ICI\n\n";

	VectorXd resultat = res.propagation(propa);

	std::cout << "\nActivation de la couche de sortie :\n" << resultat << std::endl;
	VectorXd Attendu(2); Attendu << 1, 0;

	bool verif = false;
	while(!verif)
	{
		verif = res.retropropagation(propa, Attendu);
		cout << "La propa est " << verif << endl;
		//res.printReseau();
	}

	std::cout << "La retro a ete effectuee" << endl;

	int Reponse = res.simulation(propa);

	std::cout << "Le neurone de reponse est le : " << Reponse << std::endl;



	return 0;
















	//AFFICHAGE TEST

	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}