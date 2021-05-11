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
	param.nbCouches = 6;
	param.nbNeuronesEntree = 70;
	param.nbNeuronesCache = 100;
	param.nbNeuronesSortie = 10;
	param.tauxApprentissage = 0.50;

	Reseau res(param);

	vector<MatrixXd> vPoids;
	vector<VectorXd> vBiais;
	//Couche d'entrée
	vPoids.emplace_back(aleaPoids(param.nbNeuronesEntree, param.nbNeuronesCache));
	vBiais.emplace_back(aleaBiais(param.nbNeuronesEntree));

	//Couches cachées-1
	for(int i = 1; i < (param.nbCouches-2); i++)
	{
		vPoids.emplace_back(aleaPoids(param.nbNeuronesCache, param.nbNeuronesCache));
		vBiais.emplace_back(aleaBiais(param.nbNeuronesCache));
	}
	//Couche cachée; avant-dernière
	int i = 1;//ajout
	vPoids.emplace_back(aleaPoids(param.nbNeuronesCache, param.nbNeuronesSortie));
	vBiais.emplace_back(aleaBiais(param.nbNeuronesCache));

	//Couche de sortie
	vBiais.emplace_back(aleaBiais(param.nbNeuronesSortie));
	Reseau res2(param, vPoids, vBiais);

	//MatrixXd wagon = aleaPoids(param.nbNeuronesCache, param.nbNeuronesSortie);
	//std::cout << wagon << std::endl;

	res2.printReseau();
	res.printReseau();

	return 0;
	//AUCUN AFFICHAGE TEST C'EST QUOI CA MEME

	//Test MainWindow
	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}