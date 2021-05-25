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
	if (nbCouches == 2)
		vCouches.emplace_back(Couche(p.nbNeuronesEntree, aleaPoids(p.nbNeuronesSortie, p.nbNeuronesEntree), aleaBiais(p.nbNeuronesEntree)));
	else
		vCouches.emplace_back(Couche(p.nbNeuronesEntree, aleaPoids(p.nbNeuronesCache, p.nbNeuronesEntree), aleaBiais(p.nbNeuronesEntree))); //Couches cachées-1
	
	for (int i = 1; i < (nbCouches - 2); i++)
		vCouches.emplace_back(Couche(p.nbNeuronesCache, aleaPoids(p.nbNeuronesCache, p.nbNeuronesCache), aleaBiais(p.nbNeuronesCache)));

	//Couche cachée; avant-dernière
	int i = 1; //ajout
	if (nbCouches > 2)
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

	//Couches cachées
	for (int i = 1; i <= (nbCouches - 2); i++)
		vCouches.emplace_back(Couche(p.nbNeuronesCache, mPoids[i], vBiais[i]));

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
	stats.clear();
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
}

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

MatrixXd Reseau::multiply(MatrixXd m, VectorXd v)
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
		mult = multiply(vCouches[i - 1].mPoids, vCouches[i - 1].vActivation); //mult = vCouches[i - 1].mPoids * vCouches[i - 1].vActivation;

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
				errorHidden += vCouches[nbCouches-1].error[o] * vCouches[couche].mPoids(o, h); //errorHidden += tabError[o] * self.output_layer.neurons[o].weights[h]

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

	//cout << "Simu : " << typeSim << " | repAttendue : " << label << endl;
	// récupération du resultat attendu
	if (typeSim == 0) // bmp chien chat
	{
		VectorXd repAttendues(2);
		repAttendues[label] = 1;
		//cout << "On a remplis repAttendue" << endl;
		return repAttendues;
	}
	else if(typeSim == 1) //lettres
	{
		VectorXd repAttendues(26);
		repAttendues[label] = 1;
		//cout << "On a remplis repAttendue" << endl;
		return repAttendues;
	}
	else if(typeSim == 2) //chiffres
	{
		VectorXd repAttendues(10);
		repAttendues[label] = 1;
		//cout << "On a remplis repAttendue" << endl;
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
		cout << "Sizes : " << setFichiers.size() << " " << labels.size() << endl;
		VectorXd image = setFichiers.back();

		cout << "On a le neurone rep : " << labels.back() <<  endl;
		label = retourLabel(labels.back());
		cout << "On a recup le res Attendu" << endl;
		setFichiers.pop_back(); labels.pop_back();
		verifRetro = false;
		unsigned int tauxEchec = -1;
		while(!verifRetro)
		{
			cout << "Image et label sizes : " << image.size() << "/" << vCouches[0].nbNeurones << " " << label.size() << "/" << vCouches[nbCouches-1].nbNeurones << endl;
			cout << "Loop : " << i << "/" << ma << endl;
			verifRetro = this->retropropagation(image, label);
			cout << "Activation : " << endl << vCouches[nbCouches-1].vActivation << endl;
			tauxEchec++;
		}
		i++;
		cout << "on a fini un fichier" << endl;
		stats.push_back(tauxEchec);
	}
	cout << "On a fini l'entrainement" << endl;
}