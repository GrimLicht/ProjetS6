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
//#include <QCategoryAxis>
#include <QBoxLayout>
#include <QAbstractButton>

MainWindow::MainWindow()
{
	setWindowTitle("Application Reseau de Neurone"); //nommer la fenetre principal
	resize(1400, 720);  
	
	//permettra de creer une scene plus belle
	/*QGraphicsView* view = new QGraphicsView;
	QGraphicsScene* scene = new QGraphicsScene;
	view->setFixedSize(1400, 720);
	view->setScene(scene);
	view->scene()->setSceneRect(-150, -150, view->size().width(), view->size().height());
	this->setCentralWidget(view);*/

	//creation d'un toolbar pour quitter l'app
	QToolBar* bar = new QToolBar(this);
	bar->resize(1400,50);
	bar->setLayoutDirection(Qt::RightToLeft); // permet d'avoir comme le criquis le button quitter a droite
	quitt = new QPushButton("Quitter",this);
	bar->addWidget(quitt); //add a la bar le button quitt
	connect(quitt, SIGNAL(clicked()), qApp, SLOT(quit()));

	//creation d'un widget qui aura tt les frame de boutton
	QWidget* boxButton = new QWidget(this);
	boxButton->resize(400,600);
	boxButton->move(0,50);

	//frame de boutton 1
	QFrame *f1 = new QFrame(boxButton);
	f1->resize(300, 300);
	f1->setFrameShape(QFrame::Panel);  //effet sur la frame
	f1->setFrameShadow(QFrame::Raised);//effet sur la frame
	creer = new QPushButton("Créer",this);
	charger = new QPushButton("Charger", this);
	connect(creer, SIGNAL(clicked()),this, SLOT(saisieParam()));
	//connect(creer, SIGNAL(clicked()),this, SLOT(creatFenetre()));

	boxButton1 = new QVBoxLayout(this); //utilisation de la box layout vertical
	boxButton1->addWidget(creer);   //add des boutton a la vbox
	boxButton1->addWidget(charger);//add des boutton a la vbox
	f1->setLayout(boxButton1);// frame prend la vbox definition de l'agencement

	//frame de bouton 2 
	QFrame*f2 = new QFrame(boxButton);
	f2->resize(300, 300);
	f2->setFrameShape(QFrame::Box);//same qu'au dessus
	f2->setFrameShadow(QFrame::Raised);//same qu'au dessus
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
	boxAllButton->addWidget(f1);//add la frame a la vbx de tt les bouton
	boxAllButton->addWidget(f2);//add la frame a la vbx de tt les bouton
	boxButton->setLayout(boxAllButton);//add au widget la vbx des all boutons

	QWidget* grapheP = new QWidget(this);
	grapheP->resize(400,600);
	grapheP->move(700,50);
	QFrame* f4 = new QFrame(grapheP);
	f4->resize(500, 300);
	f4->setFrameShape(QFrame::Panel);  //effet sur la frame
	f4->setFrameShadow(QFrame::Raised);//effet sur la frame
	graphe = new QBoxLayout(QBoxLayout::LeftToRight,this);
//	graphe->addWidget(affGraphe("trt"));
	f4->setLayout(graphe);

	grapheMatrice = new QVBoxLayout(this);
	grapheMatrice->addWidget(f4);
	grapheP->setLayout(grapheMatrice);
}


QChartView* MainWindow::affGraphe(QString f)
{
	//QWidget* gogo = new QWidget();
	//partie recuperation et lecture du fichier
	QChart* graphe = new QChart();
	QLineSeries* q = new QLineSeries();
	QFile file(f);
	if (!file.open(QFile::ReadOnly | QFile::Text)){
		qDebug() << "Erreur d'ouverture!";
	}

	//QString data(QTextStream::readLine());
	int j(0);
	
	while (!file.atEnd())
	{
		//q->append(j,file.readLine());
		j+=10;
	}
	file.close();
	graphe->addSeries(q);
	graphe->createDefaultAxes();
	graphe->legend()->hide();
	graphe->setTitle("Graphe de progression du RNlors de l'apprentissage");

	QChartView *chartView = new QChartView(graphe);
	chartView->setRenderHint(QPainter::Antialiasing);

	/*QValueAxis* a = new QValueAxis();

	int x(0);
	int set(0);
	while (x<j)
	{
		a->append(x,set);
		x++;
		set += 10;
	}
	//affichage du graphe
	q->append(0,10);
	q->append(1,10);
	q->append(2,10);
	q->append(3,10);
	q->append(4,10);*/

	return chartView;
}


Parametres MainWindow::sendToStructParam()
{
	Parametres param;
	
	param.nbCouches = champCC->value();
	param.nbNeuronesEntree = champNE->value();
	param.nbNeuronesCache = champNC->value();
	if(typeSimulation->currentText() == "image") 
	{
		param.typeSim = 0;
		param.nbNeuronesSortie = 2;
	}
	if(typeSimulation->currentText() == "lettre")
	{
		param.typeSim = 1;
		param.nbNeuronesSortie = 26;
	}
	if(typeSimulation->currentText() == "chiffre")
	{
		param.typeSim = 2;
		param.nbNeuronesSortie = 10;
	}
	
	param.tauxApprentissage = tauxApp->value();
	qDebug() << "nombre couche cacher : " << param.nbCouches << "\nnombre de neurones par couches cahchée : "<< param.nbNeuronesCache;
	qDebug() << "nombre neurone dans couche d'entrée : " << param.nbNeuronesEntree << "\ntype de simulation : "<< param.typeSim;
	qDebug() << "nombre neurones dans couche sortie : " << param.nbNeuronesSortie << "\ntaux apprentissage : " << param.tauxApprentissage;
	return param;
}


QString MainWindow::getChemin(){return cheminDacces;}

QString MainWindow::openDirectory()
{
	cheminDacces = QFileDialog::getExistingDirectory(this, "ouvrir un fichier", QDir::homePath());
	QFileInfo fi(cheminDacces);
	cheminDacces.clear();
	cheminDacces.push_back(fi.absolutePath() + "/" + fi.fileName());
	qDebug() << cheminDacces;
	return cheminDacces;
}

void MainWindow::saisieParam(){
	QWidget* fenetrePara = new QWidget(); // autre fenetre qui est attribut de mainwindow
	fenetrePara->resize(500,400);
	
	
	fenetrePara->setWindowTitle("parametrage creation reseau de neurone"); //give title pour fenenetrePara
	QGroupBox* groupBox = new QGroupBox(fenetrePara); //va pouvoir contenir tout les element 

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
	QFormLayout* formLayout = new QFormLayout(fenetrePara); 
	QString s3 ("Le nombre de neurones dans la couche d'entrée definira le nombre de pixels,\n");
	s3.push_back("à combien seront compresser ou remplis les images");
	champNE->setToolTip(s3);
	
	//add l'element au layout qu'on veut agencer en rajoutant un blase devant
	formLayout->addRow("Nombre de neurones par couches d'entrées:",champNE);

	champCC->setToolTip("definis le nombre couches cachées");
	formLayout->addRow("Nombre de couches cachées: ",champCC);

	champNC->setToolTip("definis le nombre de neurones par couche cachée");
	formLayout->addRow("Nombre de neurones par couches cachées:", champNC);

	QString s2 ("vous permet de choisir a quelle vitesse votre reseau apprendra\n");
	s2.push_back("valeur comprise entre 0 et 1) conseil rester proche de la valeur par defaut");
	tauxApp->setToolTip(s2);
	tauxApp->setRange(0.0, 1.0);
	tauxApp->setValue(0.8);
	tauxApp->setSingleStep(0.1);
	formLayout->addRow("Taux d'apprentissage: ",tauxApp);

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
	QHBoxLayout* layoutH = new QHBoxLayout(fenetrePara);
	QPushButton *valider = new QPushButton("Valider", fenetrePara);//creer un bouton
	QPushButton *close = new QPushButton("close", fenetrePara);//creer un bouton
   
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
	connect(close, SIGNAL(clicked()),fenetrePara, SLOT(close()));
	connect(valider, SIGNAL(clicked()), this, SLOT(sendToStructParam()));

	groupBox->setLayout(formLayout);//va permettre de bien positionner la box (je crois en disant que cette box respecte ce layout)
	fenetrePara->setLayout(layoutPrincipal);//adapter la taille de la fentre au element dedans
	fenetrePara->show(); // montrer la fenetrePara 
	
	
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

MainWindow::~MainWindow() {}