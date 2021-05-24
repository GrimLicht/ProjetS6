#if !defined(MAINWINDOW_HPP)
#define MAINWINDOW_HPP


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
#include <Eigen/Dense>
#include <vector>
#include <memory>
#include <QLabel>


using namespace QtCharts;
using namespace Eigen;


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPushButton* creer;//slot fait
    QPushButton* charger;//slot fait
    QVBoxLayout* boxButton1;
    QPushButton* simulationP;//slot fait
    QPushButton* apprentissage;//slot pas fait
    QPushButton* sauvegardeR;//slot fait
    QPushButton* sauvegardeS;//slot fait
    QVBoxLayout* boxButton2;
    QVBoxLayout* boxAllButton;
    QBoxLayout* graphe;
    QString cheminDacces;    
    QLabel res;




//Attribut ajouter pra rapport au cahier des spec
    bool paint;// pour maj le paintevent
    vector<VectorXd> uBiais;
    vector<MatrixXd> uPoids;
    shared_ptr<Reseau> rUtilisation;//AAAAAAAAAAHHH
    Parametres p;

    //fenetrepara
    QLineEdit* champNE; 
    QLineEdit* champCC;
    QLineEdit* champNC;
    QComboBox* typeSimulation;
    QDoubleSpinBox* tauxApp;

    
public:
    MainWindow();
    ~MainWindow(); 
	QChartView* affGraphe(QString f);
    void resultat(string resultatMap);
	void afficherStructure();
    Parametres chargerRN(QString fichierTXT, vector<MatrixXd>* mPoids, vector<VectorXd>* vBiais );
    void afficheRN(Parametres param, vector<MatrixXd>* mPoids);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
    
//les slot qui n'ont pas ete ajouter au cahier des spec
public slots:
    

    //fenetre de saisi des parametre RN par user
    void sendToStructParam(); 
    void saisieParam();
	QString openDirectory();
    QString saveFileName();

    //changement lors actions
    void quandOnCharge();// lorsque qu'on charge un rn
    void quandOnSim();
    void quandOnLanceApprentissage();
    void quandOnSaveR();
    void quandOnSaveS();
    //slot pour chaque bouton
   
    
 
};


#endif // MAINWINDOW_HPP
