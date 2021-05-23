#include "ext.hpp"
#include "gestionnaireMemoire.hpp"
using namespace std;
using namespace Eigen;

//Map (etiquette)
map<int,string> mapAnimaux;
map<int,char> mapChar;
map<int,char> mapInt;

//Fonctions aléatoires
VectorXd aleaBiais(int nbNeurones)
{	
	std::uniform_real_distribution<double> unif(0.0, 1);
	std::random_device rd;
	std::default_random_engine re(rd());

	VectorXd vB(nbNeurones);
	//std::uniform_real_distribution<double> unif(0, 1);
	//std::default_random_engine re;
	
	for(int i = 0; i < nbNeurones; i++)
	{
		vB(i) = unif(re);
	}
	return vB;
}

string resToString(int resultatSimulation, int typeSim)
{
	if(typeSim==0) //Chien & Chats
	{
		if(resultatSimulation == 0)
		  return "chat";
		else return "chien";
	}
	
	else if(typeSim == 1) //Characteres
	{
		char a = ((char)resultatSimulation + 97);
		string val = "";
		val += a;
		return val;
	}
	
	else if(typeSim == 2) //Nombres
	{
		char a = ((char)resultatSimulation + 48);
		string val = "";
		val += a;
		return val;
	}
	
	return "invalide";
}

MatrixXd aleaPoids(int nbNeuronesSuivants, int nbNeurones)
{
	MatrixXd mP(nbNeuronesSuivants, nbNeurones);
	std::uniform_real_distribution<double> unif(0.0, 1);
	std::random_device rd;
	std::default_random_engine re(rd());
	//return distribution(generator);
	
	for(int i = 0; i < nbNeuronesSuivants; i++)
	{
		for(int j = 0; j < nbNeurones; j++)
		{
			
			mP(i,j) = unif(re);
		}
	}
	return mP;
}