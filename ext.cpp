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
			mP(i,j) += 4;
			mP(i,j) /= 8;
		}
	}
	return mP;
}

VectorXd compressionVec(double* pixels, int height, int width, int nbNeurones)
{
	int hei, len;

	double * temp = new double[height * width];
	for(hei = 0; hei < height; hei++)
	{
		for(len = 0; len < width; len++)
		{
			temp[hei*width + len] = pixels[hei*width + len];
		}
	}

	while(hei*len > nbNeurones)
	{
		hei--; len--;
		for(int m = 0; m < hei; m++)
		{
			for(int n = 0; n < len; n++) 
				temp[m*len + n] = (temp[m*len + n] + temp[(m+1)*len] + temp[m*len + n+1] + temp[(m+1)*len + n+1])/4; 
		}
	}
	VectorXd v(nbNeurones);
	for(int m = 0; m < hei; m++)
	{
		for(int n = 0; n < len; n++)
		{
			v(m*len + n) = temp[m*len + n];
		}
	}
	int cp = hei*len;
	while(cp++ < nbNeurones)
	{
		v(cp) = 0;
	}
	delete[](temp);
	return v;
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