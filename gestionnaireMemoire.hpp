#ifndef GESTIONNAIRE_HPP
#define GESTIONNAIRE_HPP

#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "fstream"
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

//Structure BitmapFileHeader
struct BitMapFileHeader
{
	char* type;
	int size;
	char* reserved;
	int offset;
};
typedef struct BitMapFileHeader BitmapFileHeader;

//Structure BitmapImageHeader
struct BitMapImageHeader
{
	int Size ;
	int Width ;
	int Height ;
	int Planes ;
	char bitCount ;
	int Compression ;
	int imageSize ;
	int widthPixelsPerMeter ;
	int heightPixelsPerMeter ;
	int pixelsUsed ;
	int importantpixels ;
	unsigned int* R;
	unsigned int* G;
	unsigned int* B;
	int etiquette;
};
typedef struct BitMapImageHeader BitmapImageHeader;



//Structure Image
struct Image
{
	unsigned  long Width ;
	unsigned  long Height ;
	double* pixel;
	int etiquette;
	
};
typedef struct Image Image;



//Structure Mnist
struct Mnist {
	unsigned long Width ;
	unsigned long Height ; 
	int pixels[28][28];
	int etiquette ;
};
typedef struct Mnist MNIST;



//Méthodes du gestionnaire de mémoire
int reverseInt(int i);
MNIST recupDonneesFileMNIST(string fImage, string fLabel);
void recupAnalyseDonneesBmp (string f, BitMapFileHeader *header , BitMapImageHeader *image);
Image convertBitmapToImage(BitMapImageHeader b);
Image compression(unsigned int nbNeurones);
Image remplissage (unsigned int nbNeurones);
//VectorXd allPixelsBitMap(Image i);
//VectorXd allPixelMNIST(Mnist m); 
//string recupCheminAcces(QString chemin);
//void sauvegardeRN(Reseau r);
//void sauvegardeStat(Reseau r);


#endif /* GestionnaireMemoire_hpp */