#if !defined(MAINWINDOW_HPP)
#define MAINWINDOW_HPP

#include "couche.hpp"
#include "reseau.hpp"
#include "gestionnaireMemoire.hpp"


#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>
#include <string>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QBrush>
#include <string>
#include <QChartView>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <eigen3/Eigen/Dense>

using namespace QtCharts;
using namespace std;
using namespace Eigen;

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
    Parametres p;

    
public:
    MainWindow();
    ~MainWindow(); 
	QChartView* affGraphe(QString f);
    QString resultat(string resultatMap);
	QString getChemin();
	//void afficherStructure(Parametres p, QPaintEvent *)override;
	void afficherStructure();

protected:
    void paintEvent(QPaintEvent *event) override;
    
    

public slots:
    
    Parametres sendToStructParam();
    void saisieParam();
	QString openDirectory();
    //fenetre de saisi des parametre RN par user
    
 
};


#endif // MAINWINDOW_HPP
