#include "ext.hpp"
#include "gestionnaireMemoire.hpp"
using namespace std;
using namespace Eigen;

//Map (etiquette)
map<int,string> mapAnimaux;
map<int,char> mapChar;
map<int,char> mapInt;

//Fonctions aléatoires
/*double gaussianRandom() 
{
	std::uniform_real_distribution<double> unif(0.0, 1);
	std::random_device rd;
	std::default_random_engine re(rd());
	
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(70, 10);

    double r1, r2, w, X1, X2;
 
    while (w >= 1)
	{
        r1 = 2 * unif(re) - 1;
        r2 = 2 * unif(re) - 1;
        w = r1 * r1 + r2 * r2;
    }
 
    w = sqrt( ( -2 * log( w ) ) / w );
 
    X1 = r1 * w;
    X2 = r2 * w;

    std::map<int, int> hist;
    for (int n = 0; n < 100000; ++n) 
	{
        ++hist[std::round(dist(e2))];
    }

    for (auto p : hist) 
	{
        std::cout << std::fixed << std::setprecision(1) << std::setw(2) << p.first << ' ' << std::string(p.second/200, '*') << '\n';
	}


    return X1;
}*/

VectorXd aleaBiais(int nbNeurones)
{
	std::uniform_real_distribution<double> unif(0.0, 1);
	std::random_device rd;
	std::default_random_engine re(rd());

	VectorXd vB(nbNeurones);

	for(int i = 0; i < nbNeurones; i++)
	{
		vB(i) = unif(re);
	}
	return vB;
}

MatrixXd aleaPoids(int nbNeuronesSuivants, int nbNeurones)
{
	MatrixXd mP(nbNeuronesSuivants, nbNeurones);
	std::uniform_real_distribution<double> unif(0.0, 1);
	std::random_device rd;
	std::default_random_engine re(rd());
	
	for(int i = 0; i < nbNeuronesSuivants; i++)
	{
		for(int j = 0; j < nbNeurones; j++)
		{
			
			mP(i,j) = unif(re);
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