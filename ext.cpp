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
	random_device rd;
	default_random_engine re(rd());
	normal_distribution<double> nd(0.0, 1);

	VectorXd vB(nbNeurones);

	for(int i = 0; i < nbNeurones; i++)
	{
		vB(i) = nd(re);
		vB(i) += 4;
		vB(i) /= 8;
	}
	return vB;
}

MatrixXd aleaPoids(int nbNeuronesSuivants, int nbNeurones)
{
	MatrixXd mP(nbNeuronesSuivants, nbNeurones);
	random_device rd;
	default_random_engine re(rd());
	normal_distribution<double> nd(0.0, 1);
	
	for(int i = 0; i < nbNeuronesSuivants; i++)
	{
		for(int j = 0; j < nbNeurones; j++)
		{
			
			mP(i,j) = nd(re);
			mP(i) += 4;
			mP(i) /= 8;
		}
	}
	return mP;
}


string toString(int resultatSimulation, int typeSim)
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