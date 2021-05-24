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
	param.tauxApprentissage = 30;

	Reseau res(param);
	//cout << "NB NEURONES FINAL" << res.vCouches[res.nbCouches -1].nbNeurones << endl;
	//sauvegardeRN(res, "papa.txt");
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
	
	/*MNIST img;
	img = recupDonneesFileMNIST("MNIST/emnist-letters-train-images-idx3-ubyte", "MNIST/emnist-letters-train-labels-idx1-ubyte");
	//img = recupDonneesFileMNISTSimulation("emnist-letters-test-images-idx3-ubyte");
	VectorXd MNISTAttendu = etiquetteMNIST(img, param.typeSim);
	cout << "Vecteur Attendu : " << endl << MNISTAttendu << endl;

	VectorXd entree = allPixelMNIST(img);*/
	bool verif = false;
/*	while(!verif)
	{
		cout << "On passe part la retro" << endl;
		verif = res.retropropagation(entree, MNISTAttendu);
		cout << "Vecteur de sortie :\n" << res.vCouches[res.nbCouches-1].vActivation << endl;
	}*/

	//TESTS POUR LES FONCTIONS BITMAPS & LEAKS
	/*BitMapFileHeader header;
	BitMapImageHeader image;
	cout << "Let's get this bitmap file" << endl;
	recupAnalyseDonneesBmp("ACNH.bmp", &header, &image);
	cout << "Let's convert it to an image struct" << endl;
	Image img = convertBitmapToImage(image); 
	cout << "Let's make it into the vector we want" << endl;
	VectorXd entree = allPixelBitMap(img, 28*28);
	cout << "Let's simulate as your girls does" << endl;

	int Reponse = res.simulation(entree);
	std::cout << "Valeur de sortie : \n" <<  res.vCouches[res.nbCouches-1].vActivation << std::endl;
	std::cout << "Le neurone de reponse est le : " << Reponse << std::endl;*/


	//char c = Reponse + 97;
	//std::cout << "La lettre de reponse est le : " << c << std::endl;

	//cout << "LA REPONSE EST : " << resToString(19, 1) << endl;
	//string a = resToString(19, 1);
	//cout << " J'ai dit " << a << endl;
	/*vector<int> labels;
	vector<VectorXd> vec = allImage(&labels, "BMP", 28*28);
	while(vec.size())
	{
		vec.pop_back();
		labels.pop_back();
	}*/
	vector<int> labels;
	vector<VectorXd> all = recupDonneesFileMNIST("MNIST/emnist-letters-train-images-idx3-ubyte", "MNIST/emnist-letters-train-labels-idx1-ubyte", &labels);
	cout << "LA FIN EST PROOOOCHE ATTENNTION A LA COLERE DIVIIIINE" << endl;
	cout << "Testemoi ca : " << all.back() << endl;
	//On supprime les couches
	while(res.vCouches.size())
	{
		delete[](res.vCouches.back().error);
		res.vCouches.pop_back();
	}
	all.clear();
	//On supprime les stats
	res.stats.clear();

	//DELETES MIS DANS allImage !
	/*
	//Libere BitMapFileHeader
	delete[](header.type);
	delete[](header.reserved);
	//Libere BitMapImageHeader
	delete[](image.R);
	delete[](image.G);
	delete[](image.B);
	//Libere struct Image
	delete[](img.pixel);*/

	return 0;



	//AFFICHAGE TEST

	/*QApplication app (argc, argv);
	MainWindow w;
	w.show();


	return app.exec();*/
}