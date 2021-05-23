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
#include <QLabel>
#include<string>

MainWindow::MainWindow()
{
	setWindowTitle("Application Reseau de Neurone"); //nommer la fenetre principal
	resize(1920, 1080);
	//initalisation de param qui est vide lors du lancement de l'app
	p.nbCouches = 0;
	p.nbNeuronesCache = 0;
	p.nbNeuronesSortie = 0;
	p.nbNeuronesEntree = 0;

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
	connect(charger, SIGNAL(clicked()), this, SLOT(quandOnCharge()));

	boxButton1 = new QVBoxLayout(this); //utilisation de la box layout vertical
	boxButton1->addWidget(creer);	   //add des boutton a la vbox
	boxButton1->addWidget(charger);	 //add des boutton a la vbox
	f1->setLayout(boxButton1);		  // frame prend la vbox definition de l'agencement

	//frame de bouton 2
	QFrame *f2 = new QFrame(boxButton);
	f2->resize(300, 300);
	f2->setFrameShape(QFrame::Box);					//same qu'au dessus
	f2->setFrameShadow(QFrame::Raised);				//same qu'au dessus
	simulationP = new QPushButton("Simulation", this); //param 1 texte dans le button et param2 c'est definir le parent
	apprentissage = new QPushButton("Apprentissage", this);
	sauvegardeR = new QPushButton("Sauvegarder le réseau", this);
	sauvegardeS = new QPushButton("Sauvegarder les stats", this);
	connect(simulationP, SIGNAL(clicked()), this, SLOT(quandOnSim())); //permet de données les actions aux bouton
	connect(sauvegardeR, SIGNAL(clicked()), this, SLOT(quandOnSaveR()));  //permet de données les actions aux bouton
	connect(sauvegardeS, SIGNAL(clicked()), this, SLOT(quandOnSaveS()));  //permet de données les actions aux bouton
	boxButton2 = new QVBoxLayout(this);
	boxButton2->addWidget(simulationP);
	boxButton2->addWidget(apprentissage);
	boxButton2->addWidget(sauvegardeR);
	boxButton2->addWidget(sauvegardeS);
	f2->setLayout(boxButton2);

	//utilisation de la vbox pour tt les boutons
	boxAllButton = new QVBoxLayout(boxButton);
	boxAllButton->addWidget(f1);		//add la frame a la vbx de tt les bouton
	boxAllButton->addWidget(f2);		//add la frame a la vbx de tt les bouton
	boxButton->setLayout(boxAllButton); //add au widget la vbx des all boutons

	//Partie pour le graphe de progression
}

QChartView *MainWindow::affGraphe(QString f)
{
	QChart *graphe = new QChart();
	QLineSeries *q = new QLineSeries();
	QFile file(f);
	if (!file.exists())
	{
		QMessageBox::warning(nullptr, "Attention", "Vous n'avez pas selectionné de fichier de sauvegarde pour le graphe de progression");
	}
	else if (!file.open(QFile::ReadOnly))
	{
		QMessageBox::warning(nullptr, cheminDacces, "Erreur d'ouverture!");
	}
	int j(0);
	QTextStream data(&file);
	q->append(0, 0);
	while (!data.atEnd())
	{
		j += 100;
		auto line = data.readLine();
		*q << QPointF(j, line.toInt());
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

void MainWindow::sendToStructParam()
{
	// si oublie de remplir un des champs
	if (champCC->text() == "" || champNC->text() == "" || champNE->text() == "")
	{
		QMessageBox(QMessageBox::Warning, "Attention", "Un de vos champs est resté vide, veuillez recréer un Réseau", QMessageBox::Ok, this).exec();
	}
	else
	{
		bool ignore = true;
		if (p.nbCouches != 0)
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle("Attention");
			msgBox.setText("Un Réseau de neurones existe déjà! En créer un nouveau l'écraserait");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
			int ret = msgBox.exec();
			switch (ret)
			{
			case QMessageBox::Save:
				quandOnSaveR();
				qDebug() << "la save c'est bien passer";
				break;
			case QMessageBox::Ignore:
				break;
			case QMessageBox::Cancel:
				qDebug() << "vous avez annuler";
				ignore = false;
			}
		}
		if (ignore)
		{
			p.nbCouches = champCC->text().toInt() + 2;
			p.nbNeuronesCache = champNC->text().toInt();
			if (typeSimulation->currentText() == "image")
			{
				p.typeSim = 0;
				p.nbNeuronesSortie = 2;
				p.nbNeuronesEntree = champNE->text().toInt();
			}
			if (typeSimulation->currentText() == "lettre")
			{
				p.typeSim = 1;
				p.nbNeuronesSortie = 26;
				p.nbNeuronesEntree = 784;
			}
			if (typeSimulation->currentText() == "chiffre")
			{
				p.typeSim = 2;
				p.nbNeuronesSortie = 10;
				p.nbNeuronesEntree = 784;
			}

			p.tauxApprentissage = tauxApp->value();

			shared_ptr<Reseau> temp(new Reseau(p));
			rUtilisation = temp;
			temp.reset();
			paint = 1;
			QMessageBox(QMessageBox::Information, "Felicitations", "Bravo! vous venez de créer un reseau de Neurones qui reconnait des " + typeSimulation->currentText(), QMessageBox::Ok, this).exec();
		}
	}
}

//QString MainWindow::getChemin() { return cheminDacces; }

QString MainWindow::openDirectory()
{
	QString file = QFileDialog::getOpenFileName(this, "ouvrir un fichier", QDir::homePath());
	QFileInfo fi(file);
	//fenetre si on annul l'operation
	if (file == "")
	{
		QMessageBox mes(this);
		mes.setWindowTitle("Attention");
		mes.setText("Vous avez annulé(e), ou il y a eu un problème");
		mes.setIcon(QMessageBox::Warning);
		mes.exec();
	}
	//cheminDacces = QFileDialog::getOpenFileName(this, "ouvrir un fichier", QDir::homePath());
	//QFileInfo fi(cheminDacces);
	//
	return file;
}

void MainWindow::saisieParam()
{
	QWidget *fenetrePara = new QWidget(); // autre fenetre qui est attribut de mainwindow
	fenetrePara->resize(500, 400);

	fenetrePara->setWindowTitle("parametrage creation reseau de neurone"); //give title pour fenenetrePara
	QGroupBox *groupBox = new QGroupBox(fenetrePara);					  //va pouvoir contenir tout les element

	//creation boite avec des int qui on pour parent fenetre
	champNE = new QLineEdit(fenetrePara);
	champCC = new QLineEdit(fenetrePara);
	champNC = new QLineEdit(fenetrePara);

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
	if(typeSimulation->currentText() == "lettre" || typeSimulation->currentText() == "chiffre"){champNE->setText("784");}
	champCC->setText("0");
	champNC->setText("0");

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
	QPushButton *close = new QPushButton("close", fenetrePara);	 //creer un bouton
	QPushButton *valider = new QPushButton("Valider", fenetrePara); //creer un bouton
	
	//add les buttons au layout
	layoutH->addWidget(close);
	layoutH->addWidget(valider);
	layoutPrincipal->addLayout(layoutH);

	connect(close, SIGNAL(clicked()), fenetrePara, SLOT(close()));
	connect(valider, SIGNAL(clicked()), this, SLOT(sendToStructParam())); // creer reseau
	connect(valider, SIGNAL(clicked()), fenetrePara, SLOT(close()));

	groupBox->setLayout(formLayout);		 //va permettre de bien positionner la box (je crois en disant que cette box respecte ce layout)
	fenetrePara->setLayout(layoutPrincipal); //adapter la taille de la fentre au element dedans
	fenetrePara->show();					 // montrer la fenetrePara
}

QString MainWindow::saveFileName()
{

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), cheminDacces);
	if (fileName.isEmpty())
	{
		QMessageBox::warning(nullptr, "erreur", "ce fichier n'existe pas");
	}
	return fileName;
}
void MainWindow::paintEvent(QPaintEvent *event)
{
	//si on clicque sur charger cela afficherra le reseau
	if (paint == true)
	{
		afficherStructure();
	}
	else
	{
		QPainter pinceau(this);
		int largeur = 1020;
		int hauteur = 515;
		QPoint pos(900, 50);
		QRect back(pos, QSize(largeur, hauteur));
		pinceau.drawRect(back);			  //couleur fond
		pinceau.setPen(Qt::white);		   //couleur pinceau
		pinceau.setFont(QFont("Arial", 20)); //proprieter font
		pinceau.save();
		pinceau.drawText(back, Qt::AlignCenter, "Vide pour le moment");
		pinceau.restore();
	}
}
void MainWindow::quandOnCharge()
{
	//action qui vont se produire quand on click sur le boutton Charger!!
	/*1*/
	//recuperation du fichier permettant le chargement du reseau
	cheminDacces = openDirectory();
	QFileInfo f(cheminDacces);

	/*2*/
	/*On charge le reseau*/
	if (cheminDacces != "")
	{
		auto pTemp = chargerRN(cheminDacces, &uPoids, &uBiais);
		/*3*/
		/*On creer le reseau*/
		shared_ptr<Reseau> temp(new Reseau(pTemp, uPoids, uBiais));
		rUtilisation = temp;
		temp.reset();
		/*4*/
		//On ouvre le fichier des stats du reseau charger
		cheminDacces = openDirectory();
		/*5*/
		//place le graphe dans la fenetre
		QWidget *grapheP = new QWidget(this);
		grapheP->resize(720, 400);
		grapheP->move(900, 600);
		QFrame *f4 = new QFrame(grapheP);
		f4->resize(500, 300);			   
		f4->setFrameShape(QFrame::Panel);   //effet sur la frame
		f4->setFrameShadow(QFrame::Raised); //effet sur la frame
		graphe = new QBoxLayout(QBoxLayout::LeftToRight, this);
		/*6*/
		//on charge alors avec le file stat recup et on l'adapte a la Mainwindow
		graphe->addWidget(affGraphe(cheminDacces));
		f4->setLayout(graphe);
		grapheP->setLayout(graphe);
		/*7*/
		//on l'affiche
		grapheP->show();
		/*8*/
		//appel la fonction pour afficher le tableau RN
		afficheRN(pTemp, &uPoids);
		/*9*/
		//On recupere le parametres qui a ini le Reseau
		p = pTemp;
		/*10*/
		//On modifie le paint pour update le paintevent et aff la structure
		paint = true; // pour peinte event
		update();
	}
}
Parametres MainWindow::chargerRN(QString fichierTXT, vector<MatrixXd> *mPoids, vector<VectorXd> *vBiais)
{
	Parametres p;
	QFile file(fichierTXT);
	if (!file.exists())
	{
		QMessageBox::warning(nullptr, "Erreur", "Ce fichier n'existe pas");
	}
	if (!file.open(QFile::ReadOnly))
	{
		QMessageBox::warning(nullptr, "Erreur", "Erreur lors de l'ouverture! du fichier" + fichierTXT);
	}
	QTextStream data(&file);
	while (!data.atEnd())
	{
		QString param = "";
		QString c = "";

		//typeSim
		while (c != " ")
		{
			param += c;
			c = data.read(1);
		}
		c = "";
		p.typeSim = param.toUInt();
		param = "";

		//nb Couches
		while (c != " ")
		{
			param += c;
			c = data.read(1);
		}
		c = "";
		p.nbCouches = param.toUInt();
		param = "";

		//nbNeuroneEntree
		while (c != " ")
		{
			param += c;
			c = data.read(1);
		}
		c = "";
		p.nbNeuronesEntree = param.toUInt();
		param = "";

		//nbNeuroneCache
		while (c != " ")
		{
			param += c;
			c = data.read(1);
		}
		c = "";
		p.nbNeuronesCache = param.toUInt();
		param = "";

		//nbNeuroneSortie
		while (c != " ")
		{
			param += c;
			c = data.read(1);
		}
		c = "";
		p.nbNeuronesSortie = param.toUInt();
		param = "";

		//tauxApprentissage
		while (c != "\n")
		{
			param += c;
			c = data.read(1);
		}
		c = "";
		p.tauxApprentissage = param.toDouble();
		param = "";

		//poids
		unsigned int j, k;
		j = 0;
		k = 0;
		for (unsigned int i = 0; i < p.nbCouches - 1; i++)
		{ //-1 car la couche de sortie n'a pas de poids
			int taille1, taille2;
			if (i == 0)
			{
				taille1 = p.nbNeuronesEntree;
				taille2 = p.nbNeuronesCache;
			}
			else if (i == p.nbCouches - 2)
			{
				taille1 = p.nbNeuronesCache;
				taille2 = p.nbNeuronesSortie;
			}
			else
			{
				taille1 = p.nbNeuronesCache;
				taille2 = p.nbNeuronesCache;
			}
			MatrixXd h(taille1, taille2);

			while (c != '\n')
			{
				param = "";
				c = "";
				while (c != " " && c != '\n')
				{
					param += c;
					c = data.read(1);
				}
				h(j, k) = param.toDouble(); //la case j k de la matrice n° i CA MARCHE PAS
				qDebug() << h(j, k) << " j = " << j << " k = " << k;
				k++;

				if ((i == 0) && (k == p.nbNeuronesCache)) //Dans le cas de la première matrice
				{
					k = 0;
					j++;
				}
				else
				{
					if ((i == p.nbCouches - 2) && (k == p.nbNeuronesSortie)) //Dans le cas de la dernière matrice
					{
						k = 0;
						j++;
					}
					else
					{
						if (k == p.nbNeuronesCache) //Dans le cas des autres matrices
						{
							k = 0;
							j++;
						}
					}
				}
			}
			mPoids->emplace_back(h);
			c = "";
			param = "";
			k = j = 0;
		}
		for (int i = 0; i < p.nbCouches; i++)
		{
			int taille1;
			if (i == 0)
			{
				taille1 = p.nbNeuronesEntree;
			}
			else if (i == p.nbCouches - 1)
			{
				taille1 = p.nbNeuronesSortie;
			}
			else
			{
				taille1 = p.nbNeuronesCache;
			}
			VectorXd tamp(taille1);
			while (c != '\n')
			{
				param = "";
				c = "";
				while (c != " " && c != '\n')
				{
					param += c;
					c = data.read(1);
				}

				tamp[j] = param.toDouble();
				qDebug() << tamp(j) << " j = " << j;
				j++;
			}
			vBiais->push_back(tamp);
			c = "";
			param = "";
			j = 0;
		}
	}

	file.close();
	file.flush();

	return p;
}

void MainWindow::afficherStructure()
{
	double compress;
	double entree, cachee, sortie, couche;

	entree = (double)p.nbNeuronesEntree;
	cachee = (double)p.nbNeuronesCache;
	couche = (double)p.nbCouches-2;

	if(p.nbCouches > 17)
		p.nbCouches = 17; //on aura max 15 couches cachees
	couche = couche/(double)(p.nbCouches-2);
	
	if(p.nbNeuronesEntree > 100)
		p.nbNeuronesEntree /= 10; 
	if((p.nbNeuronesEntree > 20) && (p.nbNeuronesEntree <= 100)) //si il y a plus de 20 neurones d'entree, on aura toujours entre 4 et 20 cercles pour les representer
	{
		compress = p.nbNeuronesEntree/25.0;
		p.nbNeuronesEntree = compress*5;
	}
	entree = entree/(double)p.nbNeuronesEntree;

	if(p.nbNeuronesCache > 100)
		p.nbNeuronesCache /= 10; 
	if((p.nbNeuronesCache > 20) && (p.nbNeuronesCache <= 100)) //si il y a plus de 20 neurones par couche cachee, on aura toujours entre 4 et 20 cercles pour les representer
	{
		compress = p.nbNeuronesCache/25.0;
		p.nbNeuronesCache = compress*5;
	}
	cachee = cachee/(double)p.nbNeuronesCache;

	//creation point de reperes area
	int largeur = 1020;
	int hauteur = 515;
	QPoint pos(900, 50);

	//creation d'un Qpainter pour dessiner ce qu'on a a dessiner
	QPainter pinceau(this);


	//affiche le rectangle blanc en fond
	pinceau.setBrush(Qt::white);
	QRect back(pos, QSize(largeur, hauteur));
	pinceau.drawRect(back);			  //couleur fond
	
	//ecrit de text au mileu
	pinceau.setFont(QFont("Arial", 12));
	pinceau.setPen(Qt::black);		   //couleur pinceau
	pinceau.save();
	pinceau.drawText(910,70, "1 cercle représente " + QString::number((int)entree) + " neurones");
	pinceau.drawText(1300,70, "1 cercle représente " + QString::number((int)cachee) + " neurones");
	pinceau.drawText(1265,530, "1 colonne représente " + QString::number(couche) + " couches cachées");
	pinceau.restore();

	//algo pour couche entree
	//reper pour texte a faire
	pos.setX(pos.x() + 105);
	pos.setY(pos.y() + 50);
	pinceau.setFont(QFont("Arial", 20)); //propriete police
	pinceau.save();
	pinceau.drawText(pos, "Input");
	pinceau.restore();
  
	//repere emplacement pour draw neurone couche entree
	QSize in(255, 515);
	double largeurI = 1155.0;
	QPoint CI(1028, 110);
	QRect rect(CI, QSize(15, 15)); //(posX,posy,largeur, hauteur)

	//config middle
	QSize mid(510, 515);
	double largeurM = 1665.0;
	auto posi = 510.0 / (p.nbCouches - 2);
	if (!posi % 2 == 0)
	{
		posi /= 2;
	}
	QPoint CM(largeurI + (posi), 110.0);

	//config repere output
	QSize out(255, 515);
	int largeurO = 1920;
	QPoint CO(1793, 110);
	QRect rect3(CO, QSize(15, 15)); //(posX,posy,largeur, hauteur)

	//parametres couleur
	pinceau.setBrush(Qt::green);
	pinceau.save();

	for (double i = 110.0; i < 515.0; i += 405.0 / p.nbNeuronesEntree)
	{
		pinceau.drawEllipse(CI.x(), i, 15, 15);
	}
	pinceau.restore();

	//algo pour couche middle
	pos.setX(pos.x() + 300);
	pinceau.drawText(pos, "Couches Cachées");
	//repere emplacement pour draw neurone couche midd

	//parametres couleur
	pinceau.setBrush(Qt::blue);
	pinceau.save();
	auto plus = 405.0 / p.nbNeuronesCache;
	for (double i = CM.y(); i < 515.0; i += plus)
	{
		for (double j = CM.x(); j < largeurM; j += 510.0 / (p.nbCouches - 2))
		{

			pinceau.drawEllipse(j, i, 15, 15);
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

	for (double i = 110.0; i < 515.0; i += 405.0 / p.nbNeuronesSortie) //car nombre a , et prend int inf que int sup
	{
		pinceau.drawEllipse(1793, i, 15, 15);
	}
	pinceau.restore();
	pinceau.drawLine(largeurI, 50, largeurI, 565);
	pinceau.drawLine(largeurM, 50, largeurM, 565);
}

void MainWindow::quandOnLanceApprentissage()
{
	
}

void MainWindow::quandOnSim()
{
	/*1*/
	//select le fichier a utiliser
	qDebug() << "LE NMB DE NEURONES C'EST " << p.nbNeuronesEntree;
	if (p.nbCouches != 0)
	{
		QMessageBox(QMessageBox::Information, "Information", "Vous vous appretez à effectuer une simulation. Pour un réseau de neurones MNIST vous devez selectionner le fichier test dans le dossier MNIST Letter ou MNIST Chiffre dans le dossier MNIST.  Pour un réseau de neurones chien/chat, selectionnez un ficher dans l'un des 2 dossiers chien ou chat contenu dans le dossier BMP ", QMessageBox::Ok, this).exec();
		cheminDacces = openDirectory();
		if (cheminDacces != "")
		{
			QFileInfo f(cheminDacces);
			auto dir = f.path(); //permet d'avoir le chemin du dir
			/*2*/
			//conversion de l'image choisie en BMP ou MNIST selon type de simulation
			VectorXd all;
			if (p.typeSim == 0)
			{
				BitMapFileHeader header;
				BitMapImageHeader image;
				qDebug() << "Avant recup fichier";;
				recupAnalyseDonneesBmp(cheminDacces.toStdString(), &header, &image);
				qDebug() << "On a recup le fichier, on va le convertir en struct image";
				Image img = convertBitmapToImage(image);
				qDebug() << "On a l'image on va recup les pixels";
				/*4*/
				//transforme image en vecteur
				all = allPixelBitMap(img, p.nbNeuronesEntree);
				qDebug() << "L'erreur c'est pas allPixels " << all.size() << " " << p.nbNeuronesEntree;
			}
			//-------------------------------------------------------------------

			if (p.typeSim == 1 || p.typeSim == 2)
			{
				MNIST image = recupDonneesFileMNISTSimulation(cheminDacces.toStdString());
				/*3*/
				//On ne recupere pas l'etiquette (Simulation determine le resultat justement)
				/*4*/
				//transforme image en vecteur
				all = allPixelMNIST(image);
			}

			/*5*/
			//lance la simulation
			qDebug() << "On va commencer la simulation";
			int reponse = rUtilisation->simulation(all);
			qDebug() << "On a fini la simulation";
			/*6*/
			//recup la reponse dans la map
			string s = resToString(reponse, p.typeSim);
			qDebug() << "ON TEST LE STRING";
			//QString prout = prout.fromStdString(s);
			//qDebug() << prout;
			/*7*/
			//affichage du resultat sur l'interface
			resultat(s);
			//QString sprim; sprim.fromStdString(s);
			//simulation(entrees);
		}
	}
}

void MainWindow::resultat(string resultatMap)
{
	QString qstr = QString::fromStdString(resultatMap);//conversion en Qstring
	QLabel *resultat = new QLabel(qstr,this); 
	QLabel *reponse = new QLabel("Réponse du réseau :",this);
	QHBoxLayout *aff = new QHBoxLayout(this);
	aff->addWidget(reponse);
	aff->addWidget(resultat);
	QFont cara("Times",24); 
	resultat->setFont(cara);
	reponse->setFont(cara);
	QFrame *laFrame = new QFrame(this);
	laFrame->resize(390, 150);
	laFrame->setFrameShape(QFrame::Box);					//same qu'au dessus
	laFrame->setFrameShadow(QFrame::Raised);  
	laFrame->move(10,650);
	laFrame->setLayout(aff);   
	laFrame->show();
}

void MainWindow::quandOnSaveR()
{
	/*1*/
	//select le nom du fichier ou on va save
	cheminDacces = saveFileName();
	/*2*/
	//select le fichier ou on va save
	sauvegardeRN(*rUtilisation, cheminDacces.toStdString());

	/*Parametres past;
	Reseau r(past);
	auto t = cheminDacces.toStdString();
/*2
	//select le fichier ou on va save
	sauvegardeRN(r, t);*/
}

void MainWindow::afficheRN(Parametres param, vector<MatrixXd> *mPoids)
{
	QMainWindow *fenetreRN = new QMainWindow();
	fenetreRN->resize(1800, 1080);
	fenetreRN->setWindowTitle("Tableau des poids du réseau neuronal");
	//fenetreRN->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	QTableWidget *entree = new QTableWidget(param.nbNeuronesEntree, param.nbNeuronesCache, fenetreRN);
	QTableWidget *cachees = new QTableWidget(param.nbNeuronesCache * (param.nbCouches - 2), param.nbNeuronesCache, fenetreRN);
	QTableWidget *sortie = new QTableWidget(param.nbNeuronesCache, param.nbNeuronesSortie, fenetreRN);

	cachees->resize(640, 1080);
	entree->resize(640, 1080);
	sortie->resize(640, 1080);
	cachees->move(640, 0);
	sortie->move(1280, 0);

	QTableWidgetItem *all = nullptr; //pointeur pour les items (pour remplir le tableau)
									 //matrice qui contiendra les couches une par une
	unsigned int largeur;
	unsigned int hauteur;
	unsigned int bordel; //il sert, mais c'est complique a expliquer

	//mPoids->pop_back(); //la matrice de -1
	for (unsigned int i = 0; i < param.nbCouches - 1; i++) //une iteration par couche
	{

		if (!i) //la matrice vers la couche de sortie
		{
			largeur = param.nbNeuronesSortie;
			hauteur = param.nbNeuronesCache;
		}
		else if (i == param.nbCouches - 2) //la matrice de la couche d'entree
		{
			largeur = param.nbNeuronesCache;
			hauteur = param.nbNeuronesEntree;
		}
		else
		{ //couche cachee vers couche cachee
			largeur = hauteur = param.nbNeuronesCache - 2;
		}

		MatrixXd mCouches(hauteur, largeur);
		qDebug() << "l =" << largeur << "h = " << hauteur;
		mCouches = mPoids->back();

		for (unsigned int j = 0; j < hauteur; j++) //une iteration par ligne de la matrice
		{
			for (unsigned int k = 0; k < largeur; k++)
			{																//une iteration par colonne de la matrice
				all = new QTableWidgetItem(QString::number(mCouches(j, k))); //on cree un item pour stocker la valeur de la case de la matrice
				//qDebug() << mCouches(j, k);
				if (!i) //matrice vers couche de sortie
				{
					sortie->setItem(j, k, all);
				}
				else if (i == param.nbCouches - 2) //matrice de la couche d'entree
				{
					entree->setItem(j, k, all);
				}
				else
				{
					bordel = param.nbNeuronesCache * (param.nbCouches - 2 - i); //la ligne de depart, dans le tableau, de la matrice (c'est un BORDEL sans nom ('fin si, du coup))
					cachees->setItem(j + bordel, k, all);					   //on additione j a la ligne de depart
				}
				qDebug() << mCouches(j, k);
			}
		}
		mPoids->pop_back();
	}
	fenetreRN->show();
}

void MainWindow::quandOnSaveS()

{
	/*1*/
	//select le fichier ou on va save
	cheminDacces = saveFileName();
	/*2*/
	//sauvegarder le reseaux
	sauvegardeStat(*rUtilisation, cheminDacces.toStdString());
}
MainWindow::~MainWindow() {}