#include <eigen3/Eigen/Dense>
#include "reseau.hpp"
#include "couche.hpp"

#include "gestionnaireMemoire.hpp"
#include "MainWindow.hpp"

#include <iostream>

#include <map>
#include <vector>
#include <cmath>
using namespace Eigen;
using namespace std;

Reseau::Reseau(Parametres p)
{
	typeSim = p.typeSim;
	nbCouches = p.nbCouches;
	tauxApprentissage = p.tauxApprentissage;
	vCouches.reserve(nbCouches);

	//Couche d'entrée
	vCouches.emplace_back(Couche(p.nbNeuronesEntree, aleaPoids(p.nbNeuronesCache, p.nbNeuronesEntree), aleaBiais(p.nbNeuronesEntree)));

	//Couches cachées-1
	for (int i = 1; i < (nbCouches - 2); i++)
		vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesCache, p.nbNeuronesCache), aleaBiais(p.nbNeuronesCache)));

	//Couche cachée; avant-dernière
	int i = 1; //ajout
	vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesSortie, p.nbNeuronesCache), aleaBiais(p.nbNeuronesCache)));

	//Couche de sortie
	vCouches.emplace_back(Couche(p.nbNeuronesSortie, aleaBiais(p.nbNeuronesSortie)));
}

Reseau::Reseau(Parametres p, vector<MatrixXd> mPoids, vector<VectorXd> vBiais)
{
	typeSim = p.typeSim;
	nbCouches = p.nbCouches;
	tauxApprentissage = p.tauxApprentissage;
	vCouches.reserve(nbCouches);

	//Couche d'entrée

	vCouches.emplace_back(Couche(p.nbNeuronesEntree, mPoids[0], vBiais[0]));

	//Couches cachées-1
	for (int i = 1; i < (nbCouches - 2); i++)
	{
		vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[i], vBiais[i]));
	}

	//Couche cachée; avant-dernière
	vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[nbCouches - 2], vBiais[nbCouches - 2]));

	//Couche de sortie
	vCouches.emplace_back(Couche(p.nbNeuronesSortie, vBiais[nbCouches - 1]));
}

//Destructeur
Reseau::~Reseau() {}

//Getteurs
unsigned int Reseau::getNbCouches() { return nbCouches; };

vector<MatrixXd> Reseau::getPoids()
{
	vector<MatrixXd> poids;
	poids.resize(nbCouches); //mettre taille  au vec

	for (int i = 0; i < nbCouches; i++)
	{
		poids.push_back(vCouches[i].mPoids);
	}
	return poids;
};

//Méthodes du RNU
int Reseau::max(VectorXd sorties) //permet d'avoir l'indice de la valeur max
{
	int max = 0;
	int indice = -1;
	for (int i = 0; i < (sorties.size()); i++)
	{
		if (max < sorties[i])
		{
			max = sorties[i];
			indice = i;
		}
	}
	return indice;
}

void Reseau::printReseau()
{
	for(int i = 0; i < nbCouches; i++)
	{
		cout << "Matrice " << i << " :\n" << vCouches[i].mPoids << endl << endl;
	}

	/*for(int i = 0; i < nbCouches; i++)
	{
		cout << "Vecteur " << i << " :\n" << vCouches[i].vBiais << endl << :endl;
	}*/
}

int Reseau::simulation(VectorXd entrees) /*applique la propa + max */
{
	propagation(entrees);
	return max(vCouches[nbCouches-1].vActivation);
}

VectorXd sigmoide(VectorXd entree) //apparently slow
{
	VectorXd sortie(entree.size());
	for(int i = 0; i < entree.size(); i++)
	{
		double dividend = exp(-entree[i]);
		dividend += 1;
		sortie[i] = 1 / dividend;
	}
	return sortie;
}

double fast_sigmoide(double entree)
{
	double sortie;
	double diviseur = abs(entree);
	diviseur += 1;
	sortie = 1 / diviseur;

	return sortie;
}

double Reseau::deriveeSigmoide(double sigmo)
{
	double sortie;
	sortie = 1 - sigmo;
	sortie *= sigmo;
	return sigmo;
}

void Reseau::propagation(VectorXd entrees)
{
	MatrixXd mult;

	vCouches[0].vActivation = sigmoide(entrees + vCouches[0].vBiais); //modify the activation vector

	for (int i = 1; i < nbCouches; i++) //boucle pour TOUTES les couches cachees
	{
		mult = vCouches[i - 1].mPoids * vCouches[i - 1].vActivation;
		//cout << "MULT : \n" << mult << endl;

		vCouches[i].vActivation = sigmoide(mult + vCouches[i].vBiais); //modify the activation vector
	}
}


// calcul delta1
VectorXd Reseau::calculDelta1(VectorXd resultatAttendu)
{
	VectorXd error = resultatAttendu - vCouches[nbCouches - 1].vActivation;


	return error;
}

void Reseau::calculDelta2(VectorXd delta)
{
	for(int i = 0; i < vCouches[nbCouches - 1].nbNeurones; i++)
	{
		vCouches[nbCouches-1].error[i] = delta[i] * deriveeSigmoide(vCouches[nbCouches-1].vActivation[i]);
	}

	for(int m = nbCouches - 2; m >= 0; m--)
	{
		for(int i = 0; i < vCouches[m].nbNeurones; i++) //chaque neurone de la couche actuelle
		{
			double sommeErreurs = 0;

			for(int j = 0; j < vCouches[m+1].nbNeurones; j++) //chaque neurone de la couche suivante
			{
				sommeErreurs += vCouches[m+1].error[j] * vCouches[m].mPoids(j, i); //i, j ???
			}
			vCouches[m].error[i] = deriveeSigmoide(vCouches[m].vActivation[i]) * sommeErreurs;
		}
		cout << "Erreur : \n" << vCouches[m].error << endl;
	}
}

void Reseau::miseAJour() //, VectorXd activation) 
{
	double gradient;
	for (int m = 0; m < nbCouches - 1; m++)
	{
		for (int j = 0; j < vCouches[m+1].nbNeurones; j++)
		{
			for(int i = 0; i < vCouches[m].nbNeurones; i++)
			{
				cout << "		Neurone " << j << " Couche " << m+1 << endl;
				cout << "			Poids original : " << vCouches[m].mPoids(j, i) << " Biais : " << vCouches[m].vBiais[i] << endl;

				gradient = tauxApprentissage * vCouches[m].vActivation[i] * vCouches[m].error[i];

				vCouches[m].mPoids(j, i) = vCouches[m].mPoids(j, i) + gradient;
				vCouches[m].vBiais[i] = vCouches[m].vBiais(i) + gradient;

				cout << "			Poids final : " << vCouches[m].mPoids(j, i) << " Biais final  : " << vCouches[m].vBiais[i] << endl;
			}
		}
	}
	/*void Couche::updateWeights()
		{
			for(int i = 0; i < vCouches[m].nbNeurones; i++) //tout les neurones de la couche actuelle
			{
				for(int j = 0; j < vCouches[m-1].nbNeurones; j++) // nbNeurones couche precedente 
				{
					neurons[i].getWeights()[j] += ALPHA * neurons[i].getPrevWeightDelta()[j];
					neurons[i].getPrevWeightDelta()[j] = tauxApprentissage * vCouches[m].error[i] * vCouches[m].vActivation[i]; //[i].getInput()[j];
					vCouches[m].mPoids(i, j) += neurons[i].getPrevWeightDelta()[j]; // error between Couche[m].neurone[i] and Couche[m+1].neurone[j]
				}

				neurons[i].getWeights()[neurons[i].getNumInputs()] += ALPHA * neurons[i].getPrevWeightDelta()[neurons[i].getNumInputs()];
				neurons[i].getPrevWeightDelta()[neurons[i].getNumInputs()] = tauxApprentissage * neurons[i].error;
				neurons[i].getWeights()[neurons[i].getNumInputs()] += neurons[i].getPrevWeightDelta()[neurons[i].getNumInputs()];
			}
		}*/
}

bool Reseau::retropropagation(VectorXd entree, VectorXd resultatAttendu)
{
	// propagation
	propagation(entree);

	// récupération du resultatattendu
	if (max(vCouches[nbCouches -1].vActivation) != max(resultatAttendu))
	{
		VectorXd delta = calculDelta1(resultatAttendu);
		// calculdelta2
		calculDelta2(delta);
		miseAJour();

		return false;
	}
	else
	{
		cout << "On est pas passe par Mise a Jour\n";
		return true;
	}
}

VectorXd Reseau::retourLabel(int label)
{
	// récupération du resultat attendu
	VectorXd repAttendues;
	if (typeSim == 1) // bmp chien chat
		repAttendues.Zero(2);
	else if(typeSim == 2) //chiffres
		repAttendues.Zero(10);
	else if(typeSim == 3) //lettres
		repAttendues.Zero(26);
	
	repAttendues[label] = 1;
	return repAttendues;
}

vector<VectorXd> Reseau::allLabels(vector<int> labels)
{
  vector<VectorXd> vReponses;
  for(int i = 0; i < labels.size(); i++)
  {
	vReponses[i] = retourLabel(labels[i]);
  }

  return vReponses;
}

void Reseau::entrainement(vector<VectorXd> setFichiers, vector<int> labels)
{
	bool verifRetro = true;

	while(setFichiers.size()) //TOUS les fichiers
	{
		VectorXd image = setFichiers.back();
		VectorXd label = retourLabel(labels.back());
		setFichiers.pop_back(); labels.pop_back();

		unsigned int tauxEchec = 0;
		while(verifRetro)
		{
			verifRetro = retropropagation(image, label);
			tauxEchec++;
		}
		stats.push_back(tauxEchec);
	}
}