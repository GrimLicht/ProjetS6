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
	/*Parametres param;
	param.typeSim = 1;
	param.nbCouches = 3;
	param.nbNeuronesEntree = 28*28;
	param.nbNeuronesCache = 20;
	param.nbNeuronesSortie = 26;
	param.tauxApprentissage = 0.75;

	Reseau res(param);

	//img = recupDonneesFileMNISTSimulation("emnist-letters-test-images-idx3-ubyte");
	//std::cout << "Starting point" << endl;
	std::vector<int> labels;

	std::vector<VectorXd> entree = recupDonneesFileMNIST("MNIST/emnist-letters-train-images-idx3-ubyte", "MNIST/emnist-letters-train-labels-idx1-ubyte", &labels);
	int nbneufin = res.nbCouches-1;
	VectorXd MNISTAttendu(res.vCouches[nbneufin].getNbNeurones());
	MNISTAttendu = res.retourLabel(labels.back());
	int a = res.max(MNISTAttendu);
	bool verif = false;
	res.simulation(entree.back());

	//while(!verif)
	//{
		//cout << "On passe part la retro : " << a << endl;
		verif = res.retropropagation(entree.back(), MNISTAttendu);
		//cout << res.vCouches[res.nbCouches-1].vActivation << endl;
	//}

	sauvegardeRN(res, "../Backups/res2.txt");

	//On supprime les couches
	while(res.vCouches.size())
	{
		delete[](res.vCouches.back().error);
		res.vCouches.pop_back();
	}

	//On supprime les stats
	res.stats.clear();

	return 0;*/

	//AFFICHAGE TEST

	QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();
}