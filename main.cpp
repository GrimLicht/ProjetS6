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
	param.nbCouches = 3;
	param.nbNeuronesEntree = 1;
	param.nbNeuronesCache = 2;
	param.nbNeuronesSortie = 2;
	param.tauxApprentissage = 0.50;

	Reseau res(param);
	Reseau res2(param);


	std::cout << "wagon" << std::endl;

	res.printReseau();
	res2.printReseau();
	VectorXd entree; entree = aleaBiais(1);
	cout << "Vecteur test : " << entree << endl;

	VectorXd Attendu(2); Attendu << 1,0;

	bool verif = false;
	//while(!verif)
	//{
		verif = res.retropropagation(entree, Attendu);
		/*cout << "Vecteur test : \n" << entree << endl;
		cout << "Vecteur de sortie : \n" << res.vCouches[res.nbCouches-1].vActivation << endl;
		cout << "La plus grande valeur dans sortie est : " << res.max(res.vCouches[res.nbCouches-1].vActivation) << endl;
		cout << "La plus grande valeur dans attendue est : " << res.max(Attendu) << endl;
		cout << "La propa est " << ((verif == true) ? "vrai" : "faux") << endl;*/
		res.printReseau();
	//}

	int Reponse = res.simulation(entree);

	std::cout << "Le neurone de reponse est le : " << Reponse << std::endl;

	return 0;



	//AFFICHAGE TEST

	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}