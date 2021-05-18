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
	param.nbNeuronesEntree = 3;
	param.nbNeuronesCache = 10;
	param.nbNeuronesSortie = 3;
	param.tauxApprentissage = 0.5;

	Reseau res(param);

	VectorXd entree; entree = aleaBiais(3);
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
		/*cout << "Vecteur test : \n" << entree << endl;
		cout << "Vecteur de sortie : \n" << res.vCouches[res.nbCouches-1].vActivation << endl;
		cout << "La plus grande valeur dans sortie est : " << res.max(res.vCouches[res.nbCouches-1].vActivation) << endl;
		cout << "La plus grande valeur dans attendue est : " << res.max(Attendu) << endl;
		cout << "La propa est " << ((verif == true) ? "vrai" : "faux") << endl;*/
		//res.printReseau();
	}
	cout << "Retropropa finie\n";
	sauvegardeRN(res, "reseau.txt");
	res.printReseau();
	sauvegardeStat(res, "stat.txt");
	//sauvegardeStat(res, "stat.txt");

	int Reponse = res.simulation(entree);
	std::cout << "Valeur de sortie : \n" <<  res.vCouches[res.nbCouches-1].vActivation << std::endl;
	std::cout << "Le neurone de reponse est le : " << Reponse << std::endl;

	return 0;



	//AFFICHAGE TEST

	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}