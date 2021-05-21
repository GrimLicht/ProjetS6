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
#include <vector>
#include <memory>

using namespace QtCharts;
using namespace std;
using namespace Eigen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPushButton* creer;//slot fait
    QPushButton* charger;//slot fait

    QVBoxLayout* boxButton1;
    QPushButton* simulation;//A FAIRE
    QPushButton* apprentissage;//A FAIRE
    QPushButton* sauvegardeR;//A FAIRE
    QPushButton* sauvegardeS;//A FAIRE
    QVBoxLayout* boxButton2;
    QVBoxLayout* boxAllButton;

    QString cheminDacces;
    QGroupBox* groupBox;

    QBoxLayout* affRN;
    QBoxLayout* graphe;
    QVBoxLayout* grapheMatrice;
    QPushButton* quitt;

//Attribut ajouter pra rapport au cahier des spec
    bool paint;// pour maj le paintevent
    vector<VectorXd> uBiais;
    vector<MatrixXd> uPoids;
    Parametres p;
    shared_ptr<Reseau> rUtilisation;//AAAAAAAAAAHHH
    Reseau* u;

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
    QString resultat(string resultatMap);
	QString getChemin();
	void afficherStructure();
    void afficheRN(Parametres param);
    Parametres chargerRN(QString fichierTXT, vector<MatrixXd>* mPoids, vector<VectorXd>* vBiais );
    void afficheRN(Parametres param, vector<MatrixXd>* mPoids);
    
    ///truc nouveau pour synch les save
    //QPushButton* getBRN();
    //QPushButton* getBStat();
    //bool getRN();
    //bool getStat();
    //Parametres getP();

protected:
    void paintEvent(QPaintEvent *event) override;
    //virtual bool event(QEvent *event) override;
    
    
//les slot qui n'ont pas ete ajouter au cahier des spec
public slots:
    

    //fenetre de saisi des parametre RN par user
    //Parametres sendToStructParam();
    void sendToStructParam(); // deviens reseau car on le creer 
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
