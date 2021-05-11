#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "couche.hpp"
#include "reseau.hpp"
#include "gestionnaireMemoire.hpp"

#include <string>
#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QBrush>
#include <QChartView>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>

using namespace QtCharts;
using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	QPushButton* creer;
	QPushButton* charger;
	QVBoxLayout* boxButton1;
	QPushButton* simulation;
	QPushButton* apprentissage;
	QPushButton* sauvegarde;
	QVBoxLayout* boxButton2;
	QVBoxLayout* boxAllButton;

	QString cheminDacces;
	QGroupBox* groupBox;

	QBoxLayout* affRN;
	QBoxLayout* graphe;
	QVBoxLayout* grapheMatrice;
	QPushButton* quitt;

	//fenetrepara
	QSpinBox* champNE; 
	QSpinBox* champCC;
	QSpinBox* champNC;
	QComboBox* typeSimulation;
	QDoubleSpinBox* tauxApp;

	
public:
	MainWindow();
	~MainWindow(); 
	QChartView* affGraphe(QString f);
	QString resultat(string resultatMap);
	//void afficherRN(Parametres p, vector<MatrixXd> poids);
	QString getChemin();
	
   
	

public slots:
	
	Parametres sendToStructParam();
	void saisieParam();
	//Parametres sendToStructParam(QString typeSim, int nbCouches, int nbNeuronesEntree,int nbNeuronesCache, double tauxApprentissage);
	QString openDirectory();
	//bool pop();
	//fenetre de saisi des parametre RN par user
	//void validerParametre();// va permettre d'envoyer les donner parametrer a la structure parametre
	//void afficherStructure(Parametres p);
	
 
};


#endif // MAINWINDOW_HPP
