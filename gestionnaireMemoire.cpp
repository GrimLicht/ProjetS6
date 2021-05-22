#include "gestionnaireMemoire.hpp"

#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "fstream"
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

/* Permet d'inverser les bits dans le sens inverse (ex: b3b2b1b0 --> b0b1b2b3) */
int reverseInt(int i)
{
	unsigned char c1, c2, c3, c4;

	c1 = i & 255;
	c2 = (i >> 8) & 255;
	c3 = (i >> 16) & 255;
	c4 = (i >> 24) & 255;

	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}


/* Permet de récupérer les données du fichier MNIST et les stocker dans la struct MNIST */
MNIST recupDonneesFileMNIST(string fImage, string fLabel)
{
	MNIST m;
	int nbAlea;
	int cpt=0;

	/* Ouverture et lecture du fichier Image */

	ifstream monFichier(fImage);

	if(monFichier.is_open()){
		int magicNumber=0;
		int numberOfImages=0;
		int numberOfRows=0;
		int numberOfColumns=0;

		//Lecture du magic number dans le fichier
		monFichier.read((char*)&magicNumber, sizeof(magicNumber));
		magicNumber=reverseInt(magicNumber);
		cout << "Magic Number: " << magicNumber << endl;

		//Lecture du nombre d'images dans le fichier
		monFichier.read((char*)&numberOfImages, sizeof(numberOfImages));
		numberOfImages=reverseInt(numberOfImages);
		cout << "Number of images: " << numberOfImages << endl;

		//Lecture du nombre de rang dans le fichier
		monFichier.read((char*)&numberOfRows, sizeof(numberOfRows));
		numberOfRows=reverseInt(numberOfRows);
		cout << "Number of rows: " << numberOfRows << endl;
		m.Width=numberOfRows;

		//Lecture du nombre de colonnes dans le fichier
		monFichier.read((char*)&numberOfColumns, sizeof(numberOfColumns));
		numberOfColumns=reverseInt(numberOfColumns);
		cout << "Number of columns: " << numberOfColumns << endl;
		m.Height=numberOfColumns;

		//Donne un nombre aléatoire parmi le nombre total d'images dans le fichier afin de piocher aléatoirement une image dans le fichier
		srand(time(NULL));
		nbAlea=rand()%(numberOfImages+1);
		cout << "Nombre aléatoire: " << nbAlea << endl;

		//Initialise le tableau de pixels à -1 
		for(int r=0; r<numberOfRows; ++r){
			for(int c=0; c<numberOfColumns; ++c){
				m.pixels[r][c]=-1;
				//cout << "PIXEL[" << r << "][" << c <<"] : " << m.pixels[r][c] << endl;
			}
		}

		//Lecture de la valeur du pixel dans le fichier
		for(int i=0; i<numberOfImages; ++i)
		{
			for(int r=0; r<numberOfRows; ++r)
			{
				for(int c=0; c<numberOfColumns; ++c)
				{
					//Lecture du pixel
					unsigned char temp = 0;
					monFichier.read((char*)&temp, sizeof(temp));
					int valPixel=(int)temp;
					
					//Si le nombre aléatoire et le numéro d'image coincide, alors on stocke la valeur du pixel entre 0 et 255 dans le tableau de pixel de la structure MNIST
					if(i == nbAlea){

						m.pixels[r][c]=valPixel;

						//Permet d'afficher correctement la matrice des pixels de l'image 
						if(cpt%28==0){
							cout << "\n";
						}

						if(valPixel>=0 && valPixel<=9){
							cout << valPixel << "	 ";
						}

						else if(valPixel >=10 && valPixel <=99){
							cout << valPixel << "	";
						}

						else if(valPixel >=100 && valPixel <= 999){
							cout << valPixel << " ";
						}

						cpt++;

					}
				}
			}
			
		}

	}

	else{
		cout << "Impossible d'ouvrir le fichier d'image" << endl;
	}


	/* Ouverture et lecture du fichier Label */

	ifstream monFichier2(fLabel);

	if(monFichier2.is_open()){

		int magicNumber=0;
		int numberOfItems=0;

		//Lecture du nombre magic dans le fichier
		monFichier2.read((char*)&magicNumber, sizeof(magicNumber));
		magicNumber=reverseInt(magicNumber);
		cout << "\nMagic Number: " << magicNumber << endl;

		//Lecture du nombre d'items dans le fichier
		monFichier2.read((char*)&numberOfItems, sizeof(numberOfItems));
		numberOfItems=reverseInt(numberOfItems);
		cout << "Number of items: " << numberOfItems << endl;

		//Lecture du label dans le fichier
		for(int i=0; i<numberOfItems; ++i){

			char temp2 = 0;
			monFichier2.read(&temp2, sizeof(temp2));
			
			//si le nombre alea et le numero de label i coincide alors on stocke dans l'étiquette de la structure MNIST
			if(i==nbAlea){
				cout << "Valeur label: " << (int)temp2 << endl;	
				 m.etiquette=(int)temp2;
			}	
			
		}
	}

	else{
		cout << "Impossible d'ouvrir le fichier de label" << endl;
	}

	return m;
}

/* Permet de récupérer les données */
MNIST recupDonneesFileMNISTSimulation(string fImage)
{
	MNIST m;
	int nbAlea;
	int cpt=0;

	/* Ouverture et lecture du fichier Image */

	ifstream monFichier(fImage);

	if(monFichier.is_open())
	{
		int magicNumber=0;
		int numberOfImages=0;
		int numberOfRows=0;
		int numberOfColumns=0;

		//Lecture du magic number dans le fichier
		monFichier.read((char*)&magicNumber, sizeof(magicNumber));
		magicNumber=reverseInt(magicNumber);
		cout << "Magic Number: " << magicNumber << endl;

		//Lecture du nombre d'images dans le fichier
		monFichier.read((char*)&numberOfImages, sizeof(numberOfImages));
		numberOfImages=reverseInt(numberOfImages);
		cout << "Number of images: " << numberOfImages << endl;

		//Lecture du nombre de rang dans le fichier
		monFichier.read((char*)&numberOfRows, sizeof(numberOfRows));
		numberOfRows=reverseInt(numberOfRows);
		cout << "Number of rows: " << numberOfRows << endl;
		m.Width=numberOfRows;

		//Lecture du nombre de colonnes dans le fichier
		monFichier.read((char*)&numberOfColumns, sizeof(numberOfColumns));
		numberOfColumns=reverseInt(numberOfColumns);
		cout << "Number of columns: " << numberOfColumns << endl;
		m.Height=numberOfColumns;

		//Donne un nombre aléatoire parmi le nombre total d'images dans le fichier afin de piocher aléatoirement une image dans le fichier
		srand(time(NULL));
		nbAlea=rand()%(numberOfImages+1);
		cout << "Nombre aléatoire: " << nbAlea << endl;

		//Initialise le tableau de pixels à -1
		for(int r=0; r<numberOfRows; ++r){
			for(int c=0; c<numberOfColumns; ++c){
				m.pixels[r][c]=-1;
				//cout << "PIXEL[" << r << "][" << c <<"] : " << m.pixels[r][c] << endl;
			}
		}

		//Lecture de la valeur du pixel dans le fichier
		for(int i=0; i<numberOfImages; ++i)
		{
			for(int r=0; r<numberOfRows; ++r)
			{
				for(int c=0; c<numberOfColumns; ++c)
				{
					//Lecture du pixel
					unsigned char temp = 0;
					monFichier.read((char*)&temp, sizeof(temp));
					int valPixel=(int)temp;
					
					//Si le nombre aléatoire et le numéro d'image coincide, alors on stocke la valeur du pixel entre 0 et 255 dans le tableau de pixel de la structure MNIST
					if(i == nbAlea){

						m.pixels[r][c]=valPixel;

						//Permet d'afficher correctement la matrice des pixels de l'image
						if(cpt%28==0){
							cout << "\n";
						}

						if(valPixel>=0 && valPixel<=9){
							cout << valPixel << "	 ";
						}

						else if(valPixel >=10 && valPixel <=99){
							cout << valPixel << "	";
						}

						else if(valPixel >=100 && valPixel <= 999){
							cout << valPixel << " ";
						}

						cpt++;

					}
				}
			}
			
		}

	}

	else{
		cout << "Impossible d'ouvrir le fichier d'image" << endl;
	}

	return m;
}

VectorXd etiquetteMNIST(Mnist m, int typeSimu)
{
	int nbReponses;
	if(typeSimu == 1) nbReponses = 26;
	else if(typeSimu == 2) nbReponses =10;
	else 
	{ 
		cout << "Echec woula t'a demander une simu qui existe pas";
		exit(1);
	}

	VectorXd resAttendu(nbReponses);
	resAttendu.Zero(nbReponses);
	resAttendu[m.etiquette] = 1;

	return resAttendu;
}

/* Permet de récupérer les données du fichier BMP et les stocker dans les struct BMPFileHeader et BMPImageHeader */
void recupAnalyseDonneesBmp (string f, BitMapFileHeader *header , BitMapImageHeader *image)
{	
	char corrpitch[4] = {0,3,2,1};
	// ouverture du fichier 
	fstream is;
	is.open(f, ios::in|ios::binary);

	if(is.is_open())
	{
		//image->etiquette=f;
		//HEADER
		//cout << "///////////////////HEADER///////////////////" << endl;
		
		// lecture du type => B M dans notre cas 
		char* type = new char[2];
		is.read(type, 2);
		header->type=type;
		//cout << "Signature: "<< header->type << endl;

		// lecture de la taille du file header	
		int size;
		is.read(reinterpret_cast<char*>(&size), 4);
		header->size=size;
		//cout << "File size: " << header->size << endl;

		// lecture de la partie reserved 
		char* tmp = new char[4];
		is.read(tmp, 4);
		header->reserved=tmp;

		// lecture de l'adresse de début des pixels ( offset) 
		int offset;
		is.read(reinterpret_cast<char*>(&offset), 4);
		header->offset=offset;
		//cout << "DataOffset: " << header->offset << endl;
		
		//INFOHEADER
		//cout << "///////////////////INFOHEADER///////////////////" << endl;

		// lire la taille du infoheader 
		int tailleHeaderImage; 
		is.read(reinterpret_cast<char*>(&tailleHeaderImage),4) ; 
		image->Size=tailleHeaderImage;
		//cout << "Size: " << image->Size << endl ; 

		// lire la largeur 
		int width; 
		is.read(reinterpret_cast<char*>(&width),4); 
		image->Width=width;
		//cout << "Width: "	<< image->Width << endl ; 

		// lecture de la hauteur 
		int height; 
		is.read(reinterpret_cast<char*>(&height),4) ; 
		image->Height=height;
		//cout << "Height: " << image->Height << endl ; 

		// lecture des color planes du infoHeader
		int numberOfColorPlanes ; 
		is.read(reinterpret_cast<char*>(&numberOfColorPlanes),2) ; 
		image->Planes=numberOfColorPlanes;
		//cout << "Number of Color Planes: " << image->Planes << endl ; 

		// lecture des bits par pixel du infoHeader
		char numberOfBitPerPixel ; 
		is.read(reinterpret_cast<char*>(&numberOfBitPerPixel),2 ) ; 
		image->bitCount=numberOfBitPerPixel;
		//cout << "Number of bit per pixel: " << (int)image->bitCount << endl ; 

		// lecture de la	compression du infoHeader
		int compression ; 
		is.read(reinterpret_cast<char*>(&compression),4) ; 
		image->Compression=compression;
		//cout << "Compression: " << image->Compression << endl ; 

		// lecture de la taille de l'image 
		int tailleImage ; 
		is.read(reinterpret_cast<char*>(&tailleImage),4) ; 
		image->imageSize=tailleImage;
		//cout << "Image size: " << image->imageSize << endl ; 

		// lecture de la résolution horizontale 
		int horizontalResolution;
		is.read(reinterpret_cast<char*>(&horizontalResolution),4);
		image->heightPixelsPerMeter=horizontalResolution;
		//cout << "Resolution horizontal: " << image->heightPixelsPerMeter << endl;

		// lecture de la résolution verticale 
		int verticalResolution;
		is.read(reinterpret_cast<char*>(&verticalResolution),4);
		image->widthPixelsPerMeter=verticalResolution;
		//cout << "Resolution vertical: " << image->widthPixelsPerMeter << endl;

		//lecture du nombre de couleurs utilisées dans l'image 
		int colorUsed;
		is.read(reinterpret_cast<char*>(&colorUsed),4);
		image->pixelsUsed=colorUsed;
		//cout << "Colors used: " << image->pixelsUsed << endl;

		//lecture des couleurs importantes 
		int importantColors;
		is.read(reinterpret_cast<char*>(&importantColors),4);
		image->importantpixels=importantColors;
		cout << "Important colors: " << image->importantpixels << endl;
		
		//No colortable because bits per pixel > 8 (24)

		//Pixel

		
		cout << "///////////////////PIXEL///////////////////" << endl;
		unsigned char rgb[3]; // un tableau contenant les trois composants R, G et B 
		// allocation de mémoire : 
		image->R=new unsigned int[width*height];
		image->G=new unsigned int[width*height];
		image->B=new unsigned int[width*height];
		// le pitch est le nombre d'octet que prend une ligne => dans notre cas il faut que ça soit un multiple de 4 octets
		int pitch=corrpitch[(3*width)%4];
		cout << "Height :" << height << endl;
		cout << "Width :" << width << endl;
		for (int j=0; j<height; j++)
		{
			cout << "J : " << j << endl;
			for(int i=0; i<width; i++)
			{
				//cout << "	I : " << i << endl;
				// lecture et stockage dans rgb[]
				is.read(reinterpret_cast<char*>(&rgb),3);
				image->R[j*width + i] = rgb[2];
				image->G[j*width + i] = rgb[1];
				image->B[j*width + i] = rgb[0];
				//cout << "Pixel[" << j <<"][" << i << "] : R[" << (int)image->R[i*width+j] << "], G[" << (int)image->G[i*width+j] << "], B[" << (int)image->B[i*width+j] << "]" << endl;
			}
			// lecture du pitch à ignorer 
			is.read(reinterpret_cast<char*>(&rgb),pitch);
		}
	}
	cout << "ON A FINI CETTE PUTE" << endl;
}

/* Permet de transférer les informations utiles de la structure BMPImageHeader à la structure Image */
Image convertBitmapToImage(BitMapImageHeader b)
{	
	Image image;
	cout << "ON ENTRE DANS LA PLACEUUUH" << endl;
	//initialisation du height et width de la structure Image
	image.Height=b.Height;
	image.Width=b.Width;
	//allocation de mémoire
	cout << "ON A GERE LES TAILLES HEHE" << endl;
	cout << "La taille voulue = " << image.Height*image.Width << endl << "Height : " << image.Height << " Width : " << image.Width << endl;
	image.pixel = new double[image.Height*image.Width];

	cout << "PROUT PROUT CAMEMBER" << endl;
	for(int i=0; i<(image.Height*image.Width); i++)
	{
			int lightness = b.R[i] + b.G[i] + b.B[i];
			int exp1 = lightness / 8;
			int man1 = lightness % 8;
			int mantissa_bits = (1 << 23) + (man1 << 20) + (b.R[i] << 8) + b.G[i];
			double multiplier = pow(2, exp1 - 119);
			double rgb_encoding = mantissa_bits * multiplier; 
			image.pixel[i]=rgb_encoding;
			//cout << "RGB PIXEL: " << image.pixel[i] << endl;
	}
	cout << "to continue" << endl;
	return image;
}

VectorXd allPixelMNIST(Mnist m) 
{
	VectorXd pixels(784);

	for (int i = 0; i < 28; i++) 
	{
		for(int j = 0; j < 28; j++)
		{
			pixels[i] = m.pixels[i][j] / 255.0;
			if (isnan(pixels[i]))
			{
				//cout << "IT IS A NAN WATCH OUUUUT WATCH PITG" << endl;
				pixels[i] = 0;
			}
		}
	}

	return pixels;
}

vector<VectorXd> allMNIST(string fImage, string fLabel, vector<int> *labels)
{
	vector<VectorXd> set(100);
	for(int i = 0; i < 100; i++)
	{
		MNIST m = recupDonneesFileMNIST(fImage, fLabel);
		VectorXd v(784);
		v = allPixelMNIST(m);
		set.push_back(v);
		labels->push_back(m.etiquette);
	}
	return set;
}

VectorXd allPixelBitMap(Image i, int nbNeurones)
{
    //Compression de l'image jusqu'à ce sa taille <= nbNeurones
	cout << "Compression pas encore commence" << endl;
    i = compression(i, nbNeurones);

	cout << "COMPRESSION FINIE" << endl;
    //Passage d'un double** à un vecteur
    VectorXd v(nbNeurones);
    int cp = 0;
    for(int a = 0; a < (int)i.Height; a++)
	{
        for(int b = 0; b < (int)i.Width; b++)
		{
            v[cp] = i.pixel[a * i.Width + b];
            cp++;
        }
    }

	cout << "Je sais meme pas ou je suis cense etre" << endl;
    
    //Remplissage si besoin
    while(cp < nbNeurones)
	{
        v[cp] = 0;
        cp++;
    }
    
	cout << "Ici c'est la fin au moins" << endl;

    return v;
}

vector<VectorXd> allImage(vector<int> *labels, string f, int nbneurones)
{
	BitMapFileHeader fileheader;
	BitMapImageHeader imageheader;
	Image img;

	//uniform_real_distribution<int> unif{0, 12499};
	//default_random_engine re;
	srand(time(0));
	cout << "Bad alloc ?" << endl;
	// pour bmpp
	// rand pour aller dans les deux dossiers aléatoirement
	string dirname;
	vector<VectorXd> bmp;

	for (int i = 0; i < 1000; i++)
	{
		int choix;
		//choix = unif(re) % 2; // choix 0 ou 1 pour chien ou chat
		cout << "Name of folders" << endl;
		choix = rand()%2;
		if (choix == 0)		// le choix ça sera le label du coup
		{
			dirname = "/Cats";
		}
		else
		{
			dirname = "/Dogs";
		}
		// dirname = to_string(choix);
		// ouvrir le directory ou concatener avec string f => doss_entr/chien_ou_chat
		string open;
		open = f + dirname;
		cout << "Name of the folder : " << open << endl;
		// un rand pour piocher aléatoirement dans le dossier et garder le nom du dossier à chaque
		int alea = rand()%12499;
		open = open + "/" + to_string(alea) + ".bmp";
		cout << "Name of the file : " << open << endl;

		// lecture du fichier
		recupAnalyseDonneesBmp(open, &fileheader, &imageheader);
		cout << "On a recup les bmp" << endl;
		// conversion en struct image
		img = convertBitmapToImage(imageheader);
		cout << "On a recup l'image" << endl;
		// conversion en vectorXd


		cout << "Out with the old, " << endl;
		VectorXd vec1;
		vec1 = allPixelBitMap(img, nbneurones);
		cout << "In with the new" << endl;

		// remplir vector<vectorXd>
		bmp.push_back(vec1);
		// remplir vector<int> label
		labels->push_back(choix);
	}
	cout << "No bad alloc ?" << endl;
	return bmp;
}

/*Permet de sauvegarder les statistiques de reussite du RN*/
void sauvegardeStat(Reseau r, string chemin)
{
	ofstream txt(chemin, ios::out | ios::ate); //passer le commentaire dans le constructeur en tant qu'argument si on veut ajouter du texte a la fin du fichier plutot que de l'ecraser pour sauvegarder

	if (txt)
	{
		vector<unsigned int> vecStats = r.getStats();
		for (int i = 0; i < vecStats.size(); i++)
		{
			txt << vecStats[i] << endl;
		}
		txt.close();
	}
	else cout << "Erreur lors de l'ouverture du fichier\n";
}

void sauvegardeRN(Reseau r, string chemin)
{ //Adapter avec des getteurs

	ofstream fichier(chemin, ios::out | ios::trunc); //permet de supp le contenu du fichier avant l'écriture
	if (fichier) // Si le fichier est ouvert
	{
		//Structure paramètres sur la 1ère ligne
		fichier << r.typeSim << " " << r.getNbCouches() << " " << r.vCouches[0].getNbNeurones() << " ";

		if (r.getNbCouches() > 2) //Si on a + de 2 couches, on met le nombre de neurones pour la couche cachée
			fichier << r.vCouches[1].getNbNeurones() << " ";
		else //Sinon, on met 0
			fichier << "0 ";

		fichier << r.vCouches[r.getNbCouches() - 1].getNbNeurones() << " " << r.tauxApprentissage << "\n";

		//Mettre tous les mPoids

		for(int i = 0; i < r.getNbCouches()-1; i++)
		{ //Pour toutes les couches sauf la dernière
			for(int n = 0; n < r.vCouches[i].getNbNeurones(); n++)
			{ //n est le neurone de la couche actuelle
				for(int m = 0; m < r.vCouches[i+1].getNbNeurones(); m++)
				{ //m est le neurone de la couche suivante
					fichier << r.vCouches[i].mPoids(m,n) << " "; //on récupère le poids entre n et m
				}
				//Potentiellement mettre un char spécial pour signifier qu'on passe à une nouvelle ligne ?
			}
			//On passe à la matrice de la couche d'après
			fichier << "\n";
		}

		//Mettre tous les vBiais

		for (int i = 0; i < r.getNbCouches(); i++)
		{ //pour chaque couche i qui sont sur chaque ligne i+1
			for (int j = 0; j < r.vCouches[i].getNbNeurones(); j++)
			{ //pour chaque neurone j, copie des biais
				fichier << r.vCouches[i].vBiais(j) << " ";
			}
		}

		//On a fini d'écrire dans le fichier
		fichier.close();
	}
	else // Si le fichier ne s'est pas ouvert
	{
		cout << "Erreur lors de l'enregistrement" << endl;
	}
}