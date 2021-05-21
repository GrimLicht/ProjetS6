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
	param.nbNeuronesEntree = 28*28;
	param.nbNeuronesCache = 10;
	param.nbNeuronesSortie = 26;
	param.tauxApprentissage = 0.5;

	Reseau res(param);

	/*VectorXd entree; entree = aleaBiais(3);
	cout << "Vecteur test : " << entree << endl;

	VectorXd Attendu(3); Attendu << 0, 1, 0;
	//res.printReseau();
	res.stats.push_back(12);
	res.stats.push_back(7);
	res.stats.push_back(14);
	res.stats.push_back(8);
	res.stats.push_back(6);
	res.stats.push_back(3);

	bool verif = false;
	while(!verif)
	{
		verif = res.retropropagation(entree, Attendu);
	}*/
	/*cout << "Retropropa finie\n";
	sauvegardeRN(res, "reseau.txt");
	res.printReseau();
	sauvegardeStat(res, "stat.txt");*/
	
	MNIST img;
	img = recupDonneesFileMNIST("emnist-letters-train-images-idx3-ubyte", "emnist-letters-train-labels-idx1-ubyte");
	VectorXd MNISTAttendu = etiquetteMNIST(img, param.typeSim);
	cout << "Vecteur Attendu : " << MNISTAttendu << endl;

	VectorXd entree = allPixelMNIST(img);
	bool verif = false;
	//while(!verif)
	//{
		verif = res.retropropagation(entree, MNISTAttendu);
		//cout << "Matrice de poids vers la derniere couche :\n" << res.vCouches[res.nbCouches-2].mPoids << endl << "Vecteur de sortie :\n" << res.vCouches[res.nbCouches-1].vActivation << endl;
	//}

	int Reponse = res.simulation(entree);
	std::cout << "Valeur de sortie : \n" <<  res.vCouches[res.nbCouches-1].vActivation << std::endl;
	std::cout << "Le neurone de reponse est le : " << Reponse << std::endl;
	char c = Reponse + 97;
	std::cout << "La lettre de reponse est le : " << c << std::endl;
	return 0;



	//AFFICHAGE TEST

	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}