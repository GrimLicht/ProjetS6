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
	this->typeSim = p.typeSim;
	this->nbCouches = p.nbCouches;
	this->tauxApprentissage = p.tauxApprentissage;
	this->vCouches.reserve(this->nbCouches);

	//Couche d'entrée
	this->vCouches.emplace_back(Couche(p.nbNeuronesEntree, aleaPoids(p.nbNeuronesEntree, p.nbNeuronesCache), aleaBiais(p.nbNeuronesEntree)));

	//Couches cachées-1
	for (int i = 1; i < (this->nbCouches - 2); i++)
		this->vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesCache, p.nbNeuronesCache), aleaBiais(p.nbNeuronesCache)));

	//Couche cachée; avant-dernière
	int i = 1; //ajout
	this->vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesCache, p.nbNeuronesSortie), aleaBiais(p.nbNeuronesCache)));

	//Couche de sortie
	this->vCouches.emplace_back(Couche(p.nbNeuronesSortie, aleaBiais(p.nbNeuronesSortie)));
}

Reseau::Reseau(Parametres p, vector<MatrixXd> mPoids, vector<VectorXd> vBiais)
{
	this->typeSim = p.typeSim;
	this->nbCouches = p.nbCouches;
	this->tauxApprentissage = p.tauxApprentissage;
	this->vCouches.reserve(this->nbCouches);

	//Couche d'entrée

	this->vCouches.emplace_back(Couche(p.nbNeuronesEntree, mPoids[0], vBiais[0]));

	//Couches cachées-1
	for (int i = 1; i < (this->nbCouches - 2); i++)
	{
		this->vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[i], vBiais[i]));
	}

	//Couche cachée; avant-dernière
	this->vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[this->nbCouches - 2], vBiais[this->nbCouches - 2]));

	//Couche de sortie
	this->vCouches.emplace_back(Couche(p.nbNeuronesSortie, vBiais[this->nbCouches - 1]));
}

//Destructeur
Reseau::~Reseau() {}

//Getteurs
unsigned int Reseau::getNbCouches() { return this->nbCouches; };

vector<MatrixXd> Reseau::getPoids()
{
	vector<MatrixXd> poids;
	poids.resize(this->nbCouches); //mettre taille  au vec

	for (int i = 0; i < this->nbCouches; i++)
	{
		poids.push_back(this->vCouches[i].mPoids);
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
        std::cout << "Matrice " << i << " :\n" << vCouches[i].mPoids << std::endl << std::endl;
    }

    for(int i = 0; i < nbCouches; i++)
    {
        std::cout << "Vecteur " << i << " :\n" << vCouches[i].vBiais << std::endl << std::endl;
    }
}

int Reseau::simulation(VectorXd entrees) /*applique la propa + max */
{
	VectorXd v;
	v = this->propagation(entrees);
	return this->max(v);
}

double sigmoide(double entree) //apparently slow
{
	double sortie;
	double dividend = exp(-entree);
	dividend += 1;
	sortie = 1 / dividend;

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

VectorXd Reseau::propagation(VectorXd entrees)
{

	MatrixXd mult;
	VectorXd transfer; //le vector qui passe d'une couche a une autre
	//premiere couche shmilblick
	for (int i = 0; i < nbCouches; i++) //boucle pour TOUTES les couches cachees
	{
		/*
        Pour i de 1 à n neurones de la couche prec
            pour j de 1 à m neurones suiv ->actuelle
                mult[i][j] <— activation[i] * poids[j][i]
        */
		mult = MatrixXd::Zero(vCouches[i - 1].getNbNeurones(), vCouches[i].getNbNeurones()); //le getters est inutile, vu qu'on est en friends tho
		mult = vCouches[i - 1].mPoids * vCouches[i - 1].vActivation;

		/*
        Pour i de 1 à n neurones prec
            Pour j de 1 à m neurones suiv
                somme[i] <— somme[i] + mult[i][j]
        */
		for (int n = 0; n < vCouches[i].getNbNeurones(); n++)
		{
			for (int m = 0; m < vCouches[i - 1].getNbNeurones(); m++)
			{
				transfer[n] += mult(m, n);
			}
		}

		/*
        Pour i de 1 à n neurone prec 
            pactivation[i]<— somme[i] + biais[i]
            activation[i] <— sigmoide(pactivation[i])
        */
		for (int n = 0; n < vCouches[n].nbNeurones; n++)
		{
			vCouches[i].vActivation[n] = sigmoide(transfer[n] + vCouches[i].vBiais[n]); //modify the activation vector
		}
	}

	return vCouches[nbCouches - 1].vActivation; //retourne le calcul sigmoide de la derniere couche
}


// calcul delta1
VectorXd Reseau::calculDelta1(VectorXd resultatAttendu)
{
	VectorXd error;
	
	if (typeSim == 1)
	{
		// dans simulation chien chat
		// vecteur resultat [0,1] = {????}

		error = VectorXd::Zero(2); // init le vecteur v à zero
		// vCouches[nbCouches - 1].vActivation ;  //= vecteur de la derniere couche
		// selon le type de simulation i guess
		for (int i = 0; i < sizeof(vCouches[nbCouches - 1].vActivation); i++)
		{
			error[i] += (resultatAttendu[i] - (vCouches[nbCouches - 1].vActivation[i]));
		}
	}
	else if (typeSim == 2)
	{
		// simulation chiffre
		error = VectorXd::Zero(10); // init le vecteur v à zero
		// vCouches[nbCouches - 1].vActivation ;  //= vecteur de la derniere couche
		for (int i = 0; i < sizeof(vCouches[nbCouches - 1].vActivation); i++)
		{
			error[i] += (resultatAttendu[i] - (vCouches[nbCouches - 1].vActivation[i]));
		}
	}
	else if (typeSim == 3)
	{
		// récupération de l'activation de la derniere couche
		// simulation chiffre
		error = VectorXd::Zero(26); // init le vecteur v à zero
		// vCouches[nbCouches - 1].vActivation ;  //= vecteur de la derniere couche
		// selon le type de simulation i guess
		for (int i = 0; i < sizeof(vCouches[nbCouches - 1].vActivation); i++)
		{
			error[i] += (resultatAttendu[i] - (vCouches[nbCouches - 1].vActivation[i]));
		}
	}
	return error;
};

bool Reseau::verificationDelta(VectorXd delta)
{
	// calculate mean of vector error
	// vérifier que ce calcul est au dessus ou en dessous du taux d'apprentissage
	for (int i = 0; i < sizeof(delta); i++)
	{
		if (delta[i] == 0)
		{
			return true;
		}
	}
	return false;
}

MatrixXd Reseau::calculDelta2(VectorXd delta, MatrixXd poids)
{

	MatrixXd deltaMatrice; // matrice d'erreur
	int nbNeuronesDerniereCouche = vCouches[nbCouches - 1].nbNeurones;
	int indexDerniereCouche;
	int nbNeuronesCouche;

	MatrixXd mult;
	VectorXd transfer;
	// calcul somme
	for (int i = 0; i < nbCouches; i++) //boucle pour TOUTES les couches cachees
	{
		mult = MatrixXd::Zero(vCouches[i - 1].getNbNeurones(), vCouches[i].getNbNeurones()); //le getters est inutile, vu qu'on est en friends tho
		mult = vCouches[i - 1].mPoids * vCouches[i - 1].vActivation;

		/*
        Pour i de 1 à n neurones prec
            Pour j de 1 à m neurones suiv
                somme[i] <— somme[i] + mult[i][j]
        */
		for (int n = 0; n < vCouches[i].getNbNeurones(); n++)
		{
			for (int m = 0; m < vCouches[i - 1].getNbNeurones(); m++)
			{
				transfer[n] += mult(m, n);
			}
		}
	}

	// copier le vecteur delta du calcul delta1
	for (int i = 0; i < vCouches[nbCouches - 1].nbNeurones; i++)
	{
		deltaMatrice(nbCouches - 1, (i)) = delta(i);
	}
	//
	for (int j = nbCouches - 2; j >= 0; j--)
	{
		for (int i = 0; i < vCouches[i].nbNeurones; i++)
		{
			deltaMatrice(j, i) = ((deriveeSigmoide(vCouches[j].vActivation[i])) * deltaMatrice(j, i - 1) * transfer[i]);
		}
	}

	return deltaMatrice;
}

void Reseau::miseAJour(MatrixXd delta, VectorXd activation)
{

	int indexDerniereCouche;
	int nbNeuronesCouche;

	double gradient;

	for (int j = 0; j < nbCouches; j++)
	{
		for (int i = 0; i < vCouches[i].nbNeurones; i++)
		{
			gradient = tauxApprentissage * vCouches[j].vActivation[i - 1] * delta(i, j);
			vCouches[j].mPoids(i, j) = vCouches[j].mPoids(i, j) + gradient;
			vCouches[j].vBiais(i) = vCouches[j].vBiais(i) + gradient;
		}
	}
}

bool Reseau::retropropagation(VectorXd entree, VectorXd resultatattendu)
{
    // propagation
    VectorXd propa;
    propa = this->propagation(entree);
    // récupération du resultatattendu
    if (verificationDelta(calculDelta1(this->propagation(entree))))
    {
        // calculdelta1
        VectorXd d1;
        d1 = this->calculDelta1(resultatattendu);
        // calculdelta2
        MatrixXd d2;
        for (int i = 0; i < nbCouches - 1; i++)
        {
            d2 = this->calculDelta2(d1, vCouches[i].mPoids);
            // miseàjour
            this->miseAJour(d2, vCouches[i].vActivation);
        }

        return false;
    }
    else return true;
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