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
	param.nbCouches = 30;
	param.nbNeuronesEntree = 20;
	param.nbNeuronesCache = 50;
	param.nbNeuronesSortie = 10;
	param.tauxApprentissage = 1;

	Reseau res(param);

	VectorXd entree; entree = aleaBiais(20);
	cout << "Vecteur test : " << entree << endl;

	VectorXd Attendu(10); Attendu << 0, 1, 0, 0, 0, 0, 0, 0, 0, 0;

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

	int Reponse = res.simulation(entree);

	std::cout << "Le neurone de reponse est le : " << Reponse << std::endl;

	return 0;



	//AFFICHAGE TEST

	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}