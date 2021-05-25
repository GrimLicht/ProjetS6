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
	if (nbCouches == 2) //Si il n'y a pas de couche cachée
		vCouches.emplace_back(Couche(p.nbNeuronesEntree, aleaPoids(p.nbNeuronesSortie, p.nbNeuronesEntree), aleaBiais(p.nbNeuronesEntree)));
	else
		vCouches.emplace_back(Couche(p.nbNeuronesEntree, aleaPoids(p.nbNeuronesCache, p.nbNeuronesEntree), aleaBiais(p.nbNeuronesEntree))); //Couches cachées-1
	
	for (int i = 1; i < (nbCouches - 2); i++) //Couches cachées - 1
		vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesCache, p.nbNeuronesCache), aleaBiais(p.nbNeuronesCache)));

	if (nbCouches > 2) //Couche cachée; avant-dernière
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

	for (int i = 1; i <= (nbCouches - 2); i++) //Couches cachées
		vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[i], vBiais[i]));

	//Couche de sortie
	vCouches.emplace_back(Couche(p.nbNeuronesSortie, vBiais[nbCouches - 1]));
}

Reseau::~Reseau() //Destructeur
{
	while(vCouches.size())
		vCouches.pop_back();

	stats.clear();
}

//Getteurs
unsigned int Reseau::getNbCouches() { return nbCouches; };

vector<unsigned int> Reseau::getStats() { return stats;}

/*vector<MatrixXd> Reseau::getPoids()
{
	vector<MatrixXd> poids;
	poids.resize(nbCouches); //Ajuste taille du vecteur

	for (int i = 0; i < nbCouches; i++)
		poids.push_back(vCouches[i].mPoids);

	return poids;
}*/

//Méthodes du RNU
int Reseau::max(VectorXd sorties) //permet d'avoir l'indice de la valeur max
{
	double max = 0;	int indice = -1;

	for (int i = 0; i < sorties.size(); i++)
	{
		if (max < sorties[i])
		{
			max = sorties[i];
			indice = i;
		}
	}
	return indice;
}

int Reseau::simulation(VectorXd entrees)
{
	propagation(entrees); //Propage les informations vers la couche de sortie du réseau
	return max(vCouches[nbCouches-1].vActivation); //Renvoie le neurone de sortie ayant la plus grande valeur, soit "la réponse du réseau"
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

VectorXd fast_sigmoide(VectorXd entree)
{
	VectorXd sortie(entree.size());
	for(int i = 0; i < entree.size(); i++)
	{
		double diviseur = abs(entree[i]);
		diviseur += 1;
		sortie[i] = 1 / diviseur;
		bool nan = isnan(sortie[i]);
		if (nan || (sortie[i]<0))
			sortie[i] = 0;
	}

	return sortie;
}

double Reseau::deriveeSigmoide(double sigmo)
{
	double sortie;
	sortie = 1 - sigmo;
	sortie *= sigmo;
	return sortie;
}

VectorXd multiply(MatrixXd m, VectorXd v)
{
	VectorXd solution(m.col(0).size());
	solution.Zero(m.col(0).size());
	for(int i = 0; i < m.col(0).size(); i++)
	{
		for(int j = 0; j < m.row(0).size(); j++)
			solution[i] += m(i,j) * v[j];
	}

	return solution;
}

void Reseau::propagation(VectorXd entrees)
{
	VectorXd mult;

	vCouches[0].vActivation = fast_sigmoide(entrees + vCouches[0].vBiais); //modify the activation vector
	for (int i = 1; i < nbCouches; i++)
	{
		mult.Zero(vCouches[i].nbNeurones);
		mult = multiply(vCouches[i - 1].mPoids, vCouches[i - 1].vActivation);

		vCouches[i].vActivation = fast_sigmoide(mult + vCouches[i].vBiais); //modify the activation vector
	}
}

void Reseau::calculDelta(VectorXd resultatAttendu)
{
	// 1. Output neuron deltas		//∂E/∂zⱼ
	for(int o = 0; o < vCouches[nbCouches-1].nbNeurones; o++)
		vCouches[nbCouches-1].error[o] = (vCouches[nbCouches-1].vActivation[o] - resultatAttendu[o]) * deriveeSigmoide(vCouches[nbCouches-1].vActivation[o]);

	//2. Hidden neuron deltas
	for(int couche = nbCouches-2; couche >= 0; couche--)
	{
		for(int h = 0; h < vCouches[couche].nbNeurones; h++)
		{
			// We need to calculate the derivative of the error with respect to the output of each hidden layer neuron
			// dE/dyⱼ = Σ ∂E/∂zⱼ * ∂z/∂yⱼ = Σ ∂E/∂zⱼ * wᵢⱼ
			double errorHidden = 0; //errorHidden = 0
			for(int o = 0; o < vCouches[couche+1].nbNeurones; o++) 
				errorHidden += vCouches[nbCouches-1].error[o] * vCouches[couche].mPoids(o, h); 

			// ∂E/∂zⱼ = dE/dyⱼ * ∂zⱼ/∂
			vCouches[couche].error[h] = errorHidden * deriveeSigmoide(vCouches[couche].vActivation[h]);
		}
	}
}

void Reseau::miseAJour()
{
		// 3. Update output neuron weights
	double errorPoids;
	for(int o = 0; o < vCouches[nbCouches-1].nbNeurones; o++) 
	{
		for(int w = 0; w < vCouches[nbCouches-2].nbNeurones; w++)
		{
			// ∂Eⱼ/∂wᵢⱼ = ∂E/∂zⱼ * ∂zⱼ/∂wᵢⱼ
			errorPoids = vCouches[nbCouches-1].error[o] * vCouches[nbCouches-2].vActivation[w];

			// Δw = α * ∂Eⱼ/∂wᵢ
			if((vCouches[nbCouches-2].mPoids(o, w) - tauxApprentissage * errorPoids) <= 1)
				vCouches[nbCouches-2].mPoids(o, w) -= tauxApprentissage * errorPoids;
		}
	}

	// 4. Update hidden neuron weights
	for(int couche = 0; couche < nbCouches-1; couche++)
	{
		for(int h = 0; h < vCouches[couche+1].nbNeurones; h++)
		{
			for(int w = 0; w < vCouches[couche].nbNeurones; w++)
			{
				// ∂Eⱼ/∂wᵢ = ∂E/∂zⱼ * ∂zⱼ/∂wᵢ
				errorPoids = vCouches[couche+1].error[h] * vCouches[couche].vActivation[w];

				// Δw = α * ∂Eⱼ/∂wᵢ
				if((vCouches[couche].mPoids(h, w) - tauxApprentissage * errorPoids) <= 1)
					vCouches[couche].mPoids(h, w) -= tauxApprentissage * errorPoids;
			}
		}
	}
}

bool Reseau::retropropagation(VectorXd entree, VectorXd resultatAttendu)
{
	// propagation
	propagation(entree);
	// récupération du resultatattendu
	if (max(vCouches[nbCouches -1].vActivation) != max(resultatAttendu))
	{
		calculDelta(resultatAttendu);
		miseAJour();
		return false;
	}
	else return true;
}

VectorXd Reseau::retourLabel(int label)
{
	// récupération du resultat attendu
	if (typeSim == 0) // bmp chien chat
	{
		VectorXd repAttendues(2);
		repAttendues[label] = 1;
		return repAttendues;
	}
	else if(typeSim == 1) //lettres
	{
		VectorXd repAttendues(26);
		repAttendues[label] = 1;
		return repAttendues;
	}
	else if(typeSim == 2) //chiffres
	{
		VectorXd repAttendues(10);
		repAttendues[label] = 1;
		return repAttendues;
	}
	else exit(45) ;
}

void Reseau::entrainement(vector<VectorXd> setFichiers, vector<int> labels)
{
	bool verifRetro; int i = 0;
	int ma = setFichiers.size();
	VectorXd label;
	while(setFichiers.size() && labels.size()) //TOUS les fichiers
	{
		VectorXd image = setFichiers.back();

		cout << "Neurone de sortie attendu : " << labels.back() <<  endl;
		cout << "--------------------------------" << endl;
		label = retourLabel(labels.back());
		setFichiers.pop_back(); labels.pop_back();
		verifRetro = false;
		unsigned int tauxEchec = -1;
		//if((i != 33 && typeSim == 1) || ((i != 17) && (i!=18) && (i != 43) && typeSim == 2));
		//{
		while(!verifRetro)
		{
			cout << "Image " << i << "/" << ma << endl;
			verifRetro = retropropagation(image, label);
			cout << "Activation : " << endl << vCouches[nbCouches-1].vActivation << endl;
			tauxEchec++;
		}
		//}
		i++;
		cout << "Image apprise" << endl;
		stats.push_back(tauxEchec); //calcul pour avoir le taux de reussite
	}
	cout << "On a fini l'entrainement" << endl;
}