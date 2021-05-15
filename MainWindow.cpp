#include "MainWindow.hpp"
#include <QtWidgets>
#include <QPushButton>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QLibraryInfo>
#include <QGridLayout>
#include <QFormLayout>
#include <QWidget>
#include <iostream>
#include <QGraphicsWidget>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <eigen3/Eigen/Dense>
#include <QFrame>
#include <QVBoxLayout>
#include <QCategoryAxis>
#include <QBoxLayout>
#include <QAbstractButton>
#include <QtGlobal>
#include <QMessageBox>
#include <QTextStream>
#include <QAbstractAxis>
#include <QPainter>

MainWindow::MainWindow()
{
    setWindowTitle("Application Reseau de Neurone"); //nommer la fenetre principal
    resize(1920, 1080);

    //permettra de creer une scene plus belle
    /*QGraphicsView* view = new QGraphicsView;
    QGraphicsScene* scene = new QGraphicsScene;
    view->setFixedSize(1400, 720);
    view->setScene(scene);
    view->scene()->setSceneRect(-150, -150, view->size().width(), view->size().height());
    this->setCentralWidget(view);*/
    
    cheminDacces = QDir::currentPath();
    //creation d'un toolbar pour quitter l'app
    QToolBar *bar = new QToolBar(this);
    bar->resize(1920, 50);
    bar->setLayoutDirection(Qt::RightToLeft); // pemret d'avoir comme le criquis le button quitter a droite
    quitt = new QPushButton("Quitter", this);
    bar->addWidget(quitt); //add a la bar lebutton quitt
    connect(quitt, SIGNAL(clicked()), qApp, SLOT(quit()));

    //creation d'un widget qui aura tt les frame de boutton
    QWidget *boxButton = new QWidget(this);
    boxButton->resize(400, 600);
    boxButton->move(0, 50);

    //frame de boutton 1
    QFrame *f1 = new QFrame(boxButton);
    f1->resize(300, 300);
    f1->setFrameShape(QFrame::Panel);   //effet sur la frame
    f1->setFrameShadow(QFrame::Raised); //effet sur la frame
    creer = new QPushButton("Créer", this);
    charger = new QPushButton("Charger", this);
    connect(creer, SIGNAL(clicked()), this, SLOT(saisieParam()));
    //connect(creer, SIGNAL(clicked()),this, SLOT(creatFenetre()));

    boxButton1 = new QVBoxLayout(this); //utilisation de la box layout vertical
    boxButton1->addWidget(creer);       //add des boutton a la vbox
    boxButton1->addWidget(charger);     //add des boutton a la vbox
    f1->setLayout(boxButton1);          // frame prend la vbox definition de l'agencement

    //frame de bouton 2
    QFrame *f2 = new QFrame(boxButton);
    f2->resize(300, 300);
    f2->setFrameShape(QFrame::Box);                   //same qu'au dessus
    f2->setFrameShadow(QFrame::Raised);               //same qu'au dessus
    simulation = new QPushButton("Simulation", this); //param 1 texte dans le button et param2 c'est definir le parent
    apprentissage = new QPushButton("Apprentissage", this);
    sauvegarde = new QPushButton("Sauvegarder", this);
    connect(simulation, SIGNAL(clicked()), this, SLOT(openDirectory()));
    boxButton2 = new QVBoxLayout(this);
    boxButton2->addWidget(simulation);
    boxButton2->addWidget(apprentissage);
    boxButton2->addWidget(sauvegarde);
    f2->setLayout(boxButton2);

    //utilisation de la vbox pour tt les boutons
    boxAllButton = new QVBoxLayout(boxButton);
    boxAllButton->addWidget(f1);        //add la frame a la vbx de tt les bouton
    boxAllButton->addWidget(f2);        //add la frame a la vbx de tt les bouton
    boxButton->setLayout(boxAllButton); //add au widget la vbx des all boutons

    //Partie pour le graphe de progression
    QWidget *grapheP = new QWidget(this);
    grapheP->resize(720, 400);
    grapheP->move(900, 600);
    QFrame *f4 = new QFrame(grapheP);
    f4->resize(500, 300);
    f4->setFrameShape(QFrame::Panel);   //effet sur la frame
    f4->setFrameShadow(QFrame::Raised); //effet sur la frame
    graphe = new QBoxLayout(QBoxLayout::LeftToRight, this);
    graphe->addWidget(affGraphe("/home/massmax/Bureau/Cours/Learning/Training Qt/build/rn_progreession.txt"));
    f4->setLayout(graphe);

    grapheMatrice = new QVBoxLayout(this);
    grapheMatrice->addWidget(f4);
    grapheP->setLayout(grapheMatrice);
    

}
QChartView *MainWindow::affGraphe(QString f)
{

    QChart *graphe = new QChart();
    QLineSeries *q = new QLineSeries();
    QFile file(f);
    if (!file.exists())
    {
        QMessageBox::warning(nullptr, "erreur", "ce fichier n'existe pas");
    }
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(nullptr, cheminDacces + "/rn_progreession.txt", "Erreur d'ouverture!");
    }
    int j(0);
    QTextStream data(&file);
    q->append(0, 0);
    while (!data.atEnd())
    {
        j += 10;
        auto line = data.readLine();
        *q << QPointF(j,line.toInt());
       // qDebug() << line;
    }
    file.close();
    file.flush();

    graphe->addSeries(q);
    graphe->createDefaultAxes();
    graphe->setTitle("Graphe de progression du RN lors de l'apprentissage");
    graphe->legend()->hide();
    q->setPointsVisible(true);

    QChartView *chartView = new QChartView(graphe);
    chartView->setRenderHint(QPainter::Antialiasing);
    graphe->setFlag(QGraphicsItem::ItemIsMovable);
    graphe->setFlag(QGraphicsItem::ItemIsSelectable);
    graphe->resize(700, 350);

    return chartView;
}


Parametres MainWindow::sendToStructParam()
{
    Parametres param;

    param.nbCouches = champCC->value();
    param.nbNeuronesCache = champNC->value();
    if (typeSimulation->currentText() == "image")
    {
        param.typeSim = 0;
        param.nbNeuronesSortie = 2;
        param.nbNeuronesEntree = champNE->value();
    }
    if (typeSimulation->currentText() == "lettre")
    {
        param.typeSim = 1;
        param.nbNeuronesSortie = 26;
        param.nbNeuronesEntree = 784;
    }
    if (typeSimulation->currentText() == "chiffre")
    {
        param.typeSim = 2;
        param.nbNeuronesSortie = 10;
        param.nbNeuronesEntree = 784;

    }

    param.tauxApprentissage = tauxApp->value();
    qDebug() << "nombre couche cacher : " << param.nbCouches << "\nnombre de neurones par couches cahchée : " << param.nbNeuronesCache;
    qDebug() << "nombre neurone dans couche d'entrée : " << param.nbNeuronesEntree << "\ntype de simulation : " << param.typeSim;
    qDebug() << "nombre neurones dans couche sortie : " << param.nbNeuronesSortie << "\ntaux apprentissage : " << param.tauxApprentissage;
    return param;
}

QString MainWindow::getChemin() { return cheminDacces; }

QString MainWindow::openDirectory()
{
    cheminDacces = QFileDialog::getExistingDirectory(this, "ouvrir un fichier", QDir::homePath());
    QFileInfo fi(cheminDacces);
    cheminDacces.clear();
    cheminDacces.push_back(fi.absolutePath() + "/" + fi.fileName());
    qDebug() << cheminDacces;
    return cheminDacces;
}

void MainWindow::saisieParam()
{
    QWidget *fenetrePara = new QWidget(); // autre fenetre qui est attribut de mainwindow
    fenetrePara->resize(500, 400);

    fenetrePara->setWindowTitle("parametrage creation reseau de neurone"); //give title pour fenenetrePara
    QGroupBox *groupBox = new QGroupBox(fenetrePara);                      //va pouvoir contenir tout les element

    //creation boite avec des int qui on pour parent fenetre
    champNE = new QSpinBox(fenetrePara);
    champCC = new QSpinBox(fenetrePara);
    champNC = new QSpinBox(fenetrePara);

    //creation de boite avec un double qui a comme parents fenetre
    tauxApp = new QDoubleSpinBox(fenetrePara);

    //creation d'une boite avec des champ precis qstring, qui ont comme dad fenetre
    typeSimulation = new QComboBox(fenetrePara);
    //add les champ precis avec addItem
    typeSimulation->addItem("lettre");
    typeSimulation->addItem("chiffre");
    typeSimulation->addItem("image");

    //creation layout qui va agencer verticalement avec parent fenetre
    QFormLayout *formLayout = new QFormLayout(fenetrePara);
    QString s3("Le nombre de neurones dans la couche d'entrée definira le nombre de pixels,\n");
    s3.push_back("à combien seront compressées ou remplis les images (si vous choissisez une simulation sur les MNIST ce sera par defaut 784)");
    champNE->setToolTip(s3);

    //add l'element au layout qu'on veut agencer en rajoutant un blase devant
    formLayout->addRow("Nombre de neurones par couches d'entrées:", champNE);

    champCC->setToolTip("definis le nombre couches cachées");
    formLayout->addRow("Nombre de couches cachées: ", champCC);

    champNC->setToolTip("definis le nombre de neurones par couche cachée");
    formLayout->addRow("Nombre de neurones par couches cachées:", champNC);



    QString s2("vous permet de choisir a quelle vitesse votre reseau apprendra\n");
    s2.push_back("valeur comprise entre 0 et 1) conseil rester proche de la valeur par defaut");
    tauxApp->setToolTip(s2);
    tauxApp->setRange(0.0, 1.0);
    tauxApp->setValue(0.8);
    tauxApp->setSingleStep(0.1);
    formLayout->addRow("Taux d'apprentissage: ", tauxApp);

    QString s("vous permet de choisir le type de reconnaissance de votre reseau\n");
    s.push_back("lettre pour une reconnaissance de de lettre.\n");
    s.push_back("pour une reconnaissance de chiffre. image pour reconnaitre des chat.\n");
    s.push_back("definira automatiquement le nombre de neurones dans la couche de sortie");
    typeSimulation->setToolTip(s);
    formLayout->addRow("Type de simulation (lettre, image, chiffre)", typeSimulation);

    //creation d'un autre layout pour toute les boxs de manière vertical de la fenetrePara; dad fenetre
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(fenetrePara);
    layoutPrincipal->addWidget(groupBox); // add box 2ième layout

    //creation d'un troisième layout honrizontal pour les bouttons
    QHBoxLayout *layoutH = new QHBoxLayout(fenetrePara);
    QPushButton *valider = new QPushButton("Valider", fenetrePara); //creer un bouton
    QPushButton *close = new QPushButton("close", fenetrePara);     //creer un bouton

    //add les buttons au layout
    layoutH->addWidget(close);
    layoutH->addWidget(valider);
    layoutPrincipal->addLayout(layoutH);
    champCC->setValue(0);


    //Lambda part
    /* auto lambda = [=](){
    Parametres p;
    p = sendToStructParam(typeSimulation->currentText(), champCC->value(),champNE->value(), champNC->value(), tauxApp->value());
    return p;
    };*/

    //valider = qobject_cast<QPushButton*>(sender());
    //Q_SIGNAL(valider != nullptr);

    //connect(valider, SIGNAL(clicked()), signalMapper, SLOT(map()));
    //signalMapper->setMapping(valider, 0);
    connect(close, SIGNAL(clicked()), fenetrePara, SLOT(close()));
    connect(valider, SIGNAL(clicked()), this, SLOT(sendToStructParam()));

    groupBox->setLayout(formLayout);         //va permettre de bien positionner la box (je crois en disant que cette box respecte ce layout)
    fenetrePara->setLayout(layoutPrincipal); //adapter la taille de la fentre au element dedans
    fenetrePara->show();                     // montrer la fenetrePara
}

/*
void MainWindow::infosParametre(){

    QMessageBox infosBox(fenetrePara);
    infosBox.setWindowTitle("information sur type de simulation");
    infosBox.setText("Vous pouvez selectionner 3 type de simulation");
    infosBox.setDetailedText("lettre pour une reconnaissance de de lettre. chiffre pour une reconnaissance de chiffre. image pour reconnaitre des chat.");
    infosBox.setIcon(QMessageBox::Information);
    infosBox.setStandardButtons(QMessageBox::Ok);
    infosBox.exec();

}*/

/*std::string MainWindow::sendtoPara()
{
    // QString recup = table_champ->text();
    //champ_recup->setText(recup);
    //QString text = ->echo(lineEdit->text());
    //label->setText(text);
    std::string champ;
    return champ;
}*/

/*void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    file.close();
}*/
void MainWindow::paintEvent(QPaintEvent *event){
    
    afficherStructure();
  
}
void MainWindow::afficherStructure(){



//test on creer ici une struct param
    p.nbNeuronesEntree = 10;
    p.nbNeuronesCache = 20;
    p.nbCouches = 5;
    p.nbNeuronesSortie = 2;
//mtn on va definir une zone ou on va peindre
    /**
     * creer un widget qui va etre d'abors l'endroit ou on dessinera
     * cette zone doit etre de la taille de l'espace dans mainwindow qu'on lui a donner
     * creer des points qui vont servir de repere
     * faire un rectangle blanc pour bien la delimiter
     * on va dessiner dedans des cercle 
     * */


//creation point de reperes area
int largeur = 1020;
int hauteur = 515;
QPoint pos(900,50);


//creation d'un Qpainter pour dessiner ce qu'on a a dessiner
QPainter pinceau(this);

//affiche le rectangle blanc en fond
pinceau.setBrush(Qt::white);
QRect back(pos, QSize(largeur, hauteur));
pinceau.drawRect(back);//couleur fond
pinceau.setPen(Qt::black); //couleur pinceau
pinceau.setFont(QFont("Arial", 20));//proprieter font
pinceau.save();
//pinceau.drawText(back, Qt::AlignCenter, "Vide pour le moment khey");//ecrit de text au mileu


//puis on cree le cercle qui va etre afficher partout

/**
 * couleur pr couche entree pinceau.setBrush(colorV)
 * couleur pr couche middle pinceau.setBrush(colorB)
 * couleur pr couche sortie pinceau.setBrush(colorR)
 * */

//algo pour couche entree
//reper pour texte a faire 
pos.setX(pos.x() + 105); 
pos.setY(pos.y() + 50);
pinceau.drawText(pos, "Input");
pinceau.restore();


//repere emplacement pour draw neurone couche entrer
QSize in(255,515);
double largeurI = 1155.0;
QPoint CI(1028,110);
QRect rect(CI, QSize(15, 15)); //(posX,posy,largeur, hauteur)

//config middle
QSize mid(510,515);
double largeurM = 1665.0;
auto posi = 510.0/p.nbCouches;
if (!posi%2 == 0)
{
    posi /= 2;
}
QPoint CM(largeurI+(posi),110.0);

//config repere output
QSize out(255,515);
int largeurO = 1920;
QPoint CO(1793,110);
QRect rect3(CO, QSize(15, 15)); //(posX,posy,largeur, hauteur)


//parametres couleur 
pinceau.setBrush(Qt::green);
pinceau.save();

for (double i = 110.0; i < 515.0; i+= 405.0/p.nbNeuronesEntree)
{
   // pinceau.drawLine(CI.x(),i,CM.x(),i);
    pinceau.drawEllipse(CI.x(),i,15,15);
}
pinceau.restore();

//algo pour couche middle
pos.setX(pos.x() + 300); 
pinceau.drawText(pos, "Couches Cachers");
//repere emplacement pour draw neurone couche midd


//QRect rect(CI, QSize(15, 15)); //(posX,posy,largeur, hauteur)
//parametres couleur 

pinceau.setBrush(Qt::blue);
pinceau.save();
auto plus = 405.0/p.nbNeuronesCache;
for (double i = CM.y(); i < 515.0; i+= plus)
{
   for (double j = CM.x() ; j < largeurM; j+= 510.0/p.nbCouches)
    {
    
    pinceau.drawEllipse(j,i,15,15);
    //qDebug() << "i = " << CM.x();
    

  }
   //qDebug() << "i = " << i;
}

pinceau.restore();

//algo pour couche sortie
pos.setX(pos.x() + 455); 
pinceau.drawText(pos, "Output");



//parametres couleur 
pinceau.setBrush(Qt::red);
pinceau.save();

for (double i = 110.0; i < 515.0; i+= 405.0/p.nbNeuronesSortie) //car nombre a , et prend int inf que int sup
{ 
    pinceau.drawEllipse(1793,i,15,15);
}
pinceau.restore();
pinceau.drawLine(largeurI, 50,largeurI,565);
pinceau.drawLine(largeurM,50,largeurM,565);


}

MainWindow::~MainWindow() {}