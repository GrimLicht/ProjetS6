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
	param.nbCouches = 2;
	param.nbNeuronesEntree = 28*28;
	param.nbNeuronesCache = 0;
	param.nbNeuronesSortie = 26;
	param.tauxApprentissage = 0.75;

	Reseau res(param);
	//cout << "NB NEURONES FINAL" << res.vCouches[res.nbCouches -1].nbNeurones << endl;
	//sauvegardeRN(res, "papa.txt");

	//img = recupDonneesFileMNISTSimulation("emnist-letters-test-images-idx3-ubyte");
	std::cout << "Starting point" << endl;
	std::vector<int> labels;

	std::vector<VectorXd> entree = recupDonneesFileMNIST("MNIST/emnist-letters-train-images-idx3-ubyte", "MNIST/emnist-letters-train-labels-idx1-ubyte", &labels);
	std::cout << "On a recup les MNISTS" << endl;
	int nbneufin = res.nbCouches-1;
	std::cout << "Ona la taille : " << res.vCouches[nbneufin].nbNeurones << endl;
	VectorXd MNISTAttendu(res.vCouches[nbneufin].nbNeurones);
	std::cout << "On a ree le vecteur" << endl;
	cout << "Label : " << labels.back() << endl;
	MNISTAttendu = res.retourLabel(labels.back());
	std::cout << "on a le vector final attendu" << endl;
	bool verif = false;
	res.simulation(entree.back());
	cout << res.vCouches[res.nbCouches-1].vActivation << endl;
	while(!verif)
	{
		cout << "On passe part la retro" << endl;
		verif = res.retropropagation(entree.back(), MNISTAttendu);
		cout << "Vecteur de sortie :\n" << res.vCouches[res.nbCouches-1].vActivation << endl;
	}

	//On supprime les couches
	while(res.vCouches.size())
	{
		delete[](res.vCouches.back().error);
		res.vCouches.pop_back();
	}
	//all.clear();
	//On supprime les stats
	res.stats.clear();

	return 0;

	//AFFICHAGE TEST

	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}