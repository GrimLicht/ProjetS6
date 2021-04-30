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
		for(int i=0; i<numberOfImages; ++i){

			for(int r=0; r<numberOfRows; ++r){
				for(int c=0; c<numberOfColumns; ++c){

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
							cout << valPixel << "   ";
						}

						else if(valPixel >=10 && valPixel <=99){
							cout << valPixel << "  ";
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
		cout << "///////////////////HEADER///////////////////" << endl;
		
		// lecture du type => B M dans notre cas 
		char* type = new char[2];
		is.read(type, 2);
		header->type=type;
		cout << "Signature: "<< header->type << endl;

		// lecture de la taille du file header  
		int size;
		is.read(reinterpret_cast<char*>(&size), 4);
		header->size=size;
		cout << "File size: " << header->size << endl;

		// lecture de la partie reserved 
		char* tmp = new char[4];
		is.read(tmp, 4);
		header->reserved=tmp;

		// lecture de l'adresse de début des pixels ( offset) 
		int offset;
		is.read(reinterpret_cast<char*>(&offset), 4);
		header->offset=offset;
		cout << "DataOffset: " << header->offset << endl;
		
		//INFOHEADER
		cout << "///////////////////INFOHEADER///////////////////" << endl;

		// lire la taille du infoheader 
		int tailleHeaderImage; 
		is.read(reinterpret_cast<char*>(&tailleHeaderImage),4) ; 
		image->Size=tailleHeaderImage;
		cout << "Size: " << image->Size << endl ; 

		// lire la largeur 
		int width; 
		is.read(reinterpret_cast<char*>(&width),4); 
		image->Width=width;
		cout << "Width: "  << image->Width << endl ; 

		// lecture de la hauteur 
		int height; 
		is.read(reinterpret_cast<char*>(&height),4) ; 
		image->Height=height;
		cout << "Height: " << image->Height << endl ; 

		// lecture des color planes du infoHeader
		int numberOfColorPlanes ; 
		is.read(reinterpret_cast<char*>(&numberOfColorPlanes),2) ; 
		image->Planes=numberOfColorPlanes;
		cout << "Number of Color Planes: " << image->Planes << endl ; 

		// lecture des bits par pixel du infoHeader
		char numberOfBitPerPixel ; 
		is.read(reinterpret_cast<char*>(&numberOfBitPerPixel),2 ) ; 
		image->bitCount=numberOfBitPerPixel;
		cout << "Number of bit per pixel: " << (int)image->bitCount << endl ; 

		// lecture de la  compression du infoHeader
		int compression ; 
		is.read(reinterpret_cast<char*>(&compression),4) ; 
		image->Compression=compression;
		cout << "Compression: " << image->Compression << endl ; 

		// lecture de la taille de l'image 
		int tailleImage ; 
		is.read(reinterpret_cast<char*>(&tailleImage),4) ; 
		image->imageSize=tailleImage;
		cout << "Image size: " << image->imageSize << endl ; 

		// lecture de la résolution horizontale 
		int horizontalResolution;
		is.read(reinterpret_cast<char*>(&horizontalResolution),4);
		image->heightPixelsPerMeter=horizontalResolution;
		cout << "Resolution horizontal: " << image->heightPixelsPerMeter << endl;

		// lecture de la résolution verticale 
		int verticalResolution;
		is.read(reinterpret_cast<char*>(&verticalResolution),4);
		image->widthPixelsPerMeter=verticalResolution;
		cout << "Resolution vertical: " << image->widthPixelsPerMeter << endl;

		//lecture du nombre de couleurs utilisées dans l'image 
		int colorUsed;
		is.read(reinterpret_cast<char*>(&colorUsed),4);
		image->pixelsUsed=colorUsed;
		cout << "Colors used: " << image->pixelsUsed << endl;

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
		for (int j=0; j<height; j++){
			for(int i=0; i<width; i++){
					// lecture et stockage dans rgb[]
					is.read(reinterpret_cast<char*>(&rgb),3);
					image->R[i*width + j]=rgb[2];
					image->G[i*width + j]=rgb[1];
					image->B[i*width + j]=rgb[0];
					//cout << "Pixel[" << j <<"][" << i << "] : R[" << (int)image->R[i*width+j] << "], G[" << (int)image->G[i*width+j] << "], B[" << (int)image->B[i*width+j] << "]" << endl;
			}
			// lecture du pitch à ignorer 
			is.read(reinterpret_cast<char*>(&rgb),pitch);
		}
	}
}



/* Permet de transférer les informations utiles de la structure BMPImageHeader à la structure Image */
Image convertBitmapToImage(BitMapImageHeader b){
	
	Image image;
	//initialisation du height et width de la structure Image
	image.Height=b.Height;
	image.Width=b.Width;
	//allocation de mémoire
	image.pixel=new double[image.Height*image.Width];
	for(int i=0; i<(image.Height*image.Width); i++){
			int lightness = b.R[i] + b.G[i] + b.B[i];
			int exp1 = lightness / 8;
			int man1 = lightness % 8;
			int mantissa_bits = (1 << 23) + (man1 << 20) + (b.R[i] << 8) + b.G[i];
			double multiplier = pow(2, exp1 - 119);
			double rgb_encoding = mantissa_bits * multiplier; 
			image.pixel[i]=rgb_encoding;
			//cout << "RGB PIXEL: " << image.pixel[i] << endl;
	}
	return image;
}

/*Permet de sauvegarder les statistiques de reussite du RN*/
void sauvegardeStat(Reseau r)
{
	string chemin = MainWindow::getChemin().toStdString();

	ofstream txt(chemin.c_str()/*, ios::app*/); //passer le commentaire dans le constructeur en tant qu'argument si on veut ajouter du texte a la fin du fichier plutot que de l'ecraser pour sauvegarder

	if (txt)
	{
		vector<unsigned int> vecStats = r.getStats();
		ostringstream streamStats;

		if (!vecStats.empty())
		{
			copy(vecStats.begin(), vecStats.end()-1), //on recupere tous les elements sauf le dernier, pour eviter les char accidentels a la fin
				ostream_iterator<unsigned int>(streamStats, ","));

				streamStats << vecStats.back(); //on ajoute le dernier element, sans char a la fin
		}
		txt << streamStats << endl;
	}
	else
		cout << "Erreur lors de l'ouverture du fichier\n";

	txt.close();
}