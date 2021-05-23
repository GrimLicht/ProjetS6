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
Reseau::~Reseau() 
{
	while(vCouches.size())
	{
		vCouches.pop_back();
	}
	while(stats.size())
	{
		stats.pop_back();
	}
}

//Getteurs
unsigned int Reseau::getNbCouches() { return nbCouches; };

vector<unsigned int> Reseau::getStats(){ return stats;}

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
	double max = 0;
	int indice = -1;
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

void Reseau::printReseau()
{
	for(int i = 0; i < nbCouches; i++)
	{
		cout << "Matrice " << i << " :\n" << vCouches[i].mPoids << endl << endl;
	}

	for(int i = 0; i < nbCouches; i++)
	{
		cout << "Vecteur " << i << " :\n" << vCouches[i].vBiais << endl << endl;
	}
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
	//cout << "Vecteur de sigmoide : \n" << sortie << endl;
	return sortie;
}

VectorXd fast_sigmoide(VectorXd entree)
{
	//cout << "Prout" << endl;
	VectorXd sortie(entree.size());
	for(int i = 0; i < entree.size(); i++)
	{
		//cout << entree[i] << " -> ";
		double diviseur = abs(entree[i]);
		//cout << abs(entree[i]) << " -> ";
		diviseur += 1;
		//cout << diviseur << " = ";
		sortie[i] = 1 / diviseur;
		bool nan = isnan(sortie[i]);
		if (nan)
		{
			//cout << "IT IS A NAN WATCH OUUUUT WATCH PITG" << endl;
			sortie[i] = 0;
		}
		//cout << sortie[i] << endl;
	}
	//cout << "Vecteur de sigmoide : \n" << sortie << endl;
	return sortie;
}

double Reseau::deriveeSigmoide(double sigmo)
{
	double sortie;
	sortie = 1 - sigmo;
	sortie *= sigmo;
	return sortie;
}

MatrixXd Reseau::multiply(MatrixXd m, VectorXd v)
{
	VectorXd solution(m.col(0).size());
	//cout << "SIZE OF THE VECTOR : " << v.size() << endl;
	//int solution[3];
	//initilization.
	solution.Zero(m.col(0).size());
	//cout << "Vector : " << endl << v << endl;
	//multiplication.
	//cout << "Column size or number of lines (i): " << m.col(0).size() << endl;
	//cout << "Row size or number of cases per line (j): " << m.row(0).size() << endl;
	//cout << "LES COLONNES SONT EGALES A : " << m.col(0).size() << endl;
	//cout << "LES LIGNE SONT EGALES A : " << m.row(0).size() << endl;
	for(int i = 0; i < m.col(0).size(); i++)
	{
	//	cout << "SELON LES COLONNES : " << i << endl;
		for(int j = 0; j < m.row(0).size(); j++)
		{
	//		cout << "	V de " << j << " : " << v[j] << endl;
			solution[i] += m(i,j) * v[j];
		}
	}

	return solution;
}

void Reseau::propagation(VectorXd entrees)
{
	VectorXd mult;

	vCouches[0].vActivation = fast_sigmoide(entrees + vCouches[0].vBiais); //modify the activation vector
	for (int i = 1; i < nbCouches; i++)
	{
		//cout << "Couche actuelle " << i << endl << "Nb Couches : " << nbCouches << endl;
		mult.Zero(vCouches[i].nbNeurones);
		mult = multiply(vCouches[i - 1].mPoids, vCouches[i - 1].vActivation); //mult = vCouches[i - 1].mPoids * vCouches[i - 1].vActivation;
		//cout << "Mult : \n" << mult << endl;

		/*double temp;
		for(int j = 0; j < mult.size(); j++)
		{
			cout << "Value before division : " << mult[j] << endl;
			temp = mult[j]/vCouches[i-1].nbNeurones;
			mult[j] = temp;
			cout << "Value after division : " << mult[j] << endl;
		}*/
		//cout << "vecteur d'input net : " << endl << mult << endl;
		vCouches[i].vActivation = fast_sigmoide(mult + vCouches[i].vBiais); //modify the activation vector
	}
}

void Reseau::calculDelta(VectorXd resultatAttendu)
{
// 1. Output neuron deltas
	//cout << "on commence les calculs" << endl;
	//double *tabError = new double[vCouches[nbCouches-1].nbNeurones]; //tabError = [0] * len(self.output_layer.neurons)
	for(int o = 0; o < vCouches[nbCouches-1].nbNeurones; o++)//for o in range(len(self.output_layer.neurons)):
	{
		//∂E/∂zⱼ
		vCouches[nbCouches-1].error[o] = (vCouches[nbCouches-1].vActivation[o] - resultatAttendu[o]) * deriveeSigmoide(vCouches[nbCouches-1].vActivation[o]);
		 //tabError[o]
		 //= self.output_layer.neurons[o].calculate_pd_error_wrt_total_net_input(training_outputs[o]) = self.calculate_pd_error_wrt_output(target_output) * self.calculate_pd_total_net_input_wrt_input();
		 //= -(target_output - self.output) * self.output * (1 - self.output)
	}
	//tabError = vCouches[nbCouches-1].error;
	//cout << "on a calcule pour la couche de sortie" << endl;

	//2. Hidden neuron deltas
	double errorPoids;
	for(int couche = nbCouches-2; couche >= 0; couche--)
	{
		for(int h = 0; h < vCouches[couche].nbNeurones; h++) //for h in range(len(self.hidden_layer.neurons)):
		{
			// We need to calculate the derivative of the error with respect to the output of each hidden layer neuron
			// dE/dyⱼ = Σ ∂E/∂zⱼ * ∂z/∂yⱼ = Σ ∂E/∂zⱼ * wᵢⱼ
			double errorHidden = 0; //errorHidden = 0
			for(int o = 0; o < vCouches[couche+1].nbNeurones; o++) //for o in range(len(self.output_layer.neurons)):
			{
				errorHidden += vCouches[nbCouches-1].error[o] * vCouches[couche].mPoids(o, h); //errorHidden += tabError[o] * self.output_layer.neurons[o].weights[h]
			}

			// ∂E/∂zⱼ = dE/dyⱼ * ∂zⱼ/∂
			vCouches[couche].error[h] = errorHidden * deriveeSigmoide(vCouches[couche].vActivation[h]);//tabErrorHidden[h] = errorHidden * self.hidden_layer.neurons[h].calculate_pd_total_net_input_wrt_input()
		}
	}

	//cout << "On a fini de calculer les deltas" << endl; /////////// correct

	// 3. Update output neuron weights
	for(int o = 0; o < vCouches[nbCouches-1].nbNeurones; o++) //for o in range(len(self.output_layer.neurons)):
	{
		//cout << "Neurone " << o << " de la couche de sortie" << endl;
		for(int w = 0; w < vCouches[nbCouches-2].nbNeurones; w++)//for w_ho in range(len(self.output_layer.neurons[o].weights)):
		{
			//cout << "	Neurone : " << w << " de la couche precedente " << endl;
			// ∂Eⱼ/∂wᵢⱼ = ∂E/∂zⱼ * ∂zⱼ/∂wᵢⱼ
			errorPoids = vCouches[nbCouches-1].error[o] * vCouches[nbCouches-2].vActivation[w];//pd_error_wrt_weight = tabError[o] * self.output_layer.neurons[o].calculate_pd_total_net_input_wrt_weight(w_ho)
//			cout << "Apres le calcul de l'erreur par poids ca bloque pas ! ";
			// Δw = α * ∂Eⱼ/∂wᵢ
			//cout << "Ajout : " << tauxApprentissage*errorPoids << endl;
			if((vCouches[nbCouches-2].mPoids(o, w) - tauxApprentissage * errorPoids) <= 1)
				vCouches[nbCouches-2].mPoids(o, w) -= tauxApprentissage * errorPoids; //self.output_layer.neurons[o].weights[w_ho] -= self.LEARNING_RATE * pd_error_wrt_weight
		}
	}

	//cout << "On a fini de modif les poids de la couche de sortie" << endl;

	// 4. Update hidden neuron weights
	for(int couche = 0; couche < nbCouches-1; couche++)
	{
		for(int h = 0; h < vCouches[couche+1].nbNeurones; h++)//for h in range(len(self.hidden_layer.neurons)):
		{
			for(int w = 0; w < vCouches[couche].nbNeurones; w++) //for w_ih in range(len(self.hidden_layer.neurons[h].weights)):
			{
				// ∂Eⱼ/∂wᵢ = ∂E/∂zⱼ * ∂zⱼ/∂wᵢ
				errorPoids = vCouches[couche+1].error[h] * vCouches[couche].vActivation[w]; //pd_error_wrt_weight = tabErrorHidden[h] * self.hidden_layer.neurons[h].calculate_pd_total_net_input_wrt_weight(w_ih)

				// Δw = α * ∂Eⱼ/∂wᵢ
				if((vCouches[couche].mPoids(h, w) - tauxApprentissage * errorPoids) <= 1)
					vCouches[couche].mPoids(h, w) -= tauxApprentissage * errorPoids; //self.hidden_layer.neurons[h].weights[w_ih] -= self.LEARNING_RATE * pd_error_wrt_weight
			}
		}
	}
	//cout << "On a fini de modif tout les poids" << endl;
}

bool Reseau::retropropagation(VectorXd entree, VectorXd resultatAttendu)
{
	// propagation
	propagation(entree);
	//cout << "Matrice attendue : \n" << resultatAttendu << endl;
	//cout << "Matrice de sortie : \n" << vCouches[nbCouches-1].vActivation << endl;
	// récupération du resultatattendu
	if (max(vCouches[nbCouches -1].vActivation) != max(resultatAttendu))
	{
		// calculdelta2
		calculDelta(resultatAttendu);
		//cout << vCouches[nbCouches-2].mPoids << endl;
		return false;
	}
	else
	{
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