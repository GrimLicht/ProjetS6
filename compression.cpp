#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <memory>

#include "couche.hpp"
#include "gestionnaireMemoire.hpp"
#include "reseau.hpp"
#include "MainWindow.hpp"


int main()
{
	Eigen::MatrixXd m = Eigen::MatrixXd::Random(20, 20);
	std::cout << "Here is m at the start :\n" << m << std::endl;
	//std::cout << m.block(1,1,1,1) << std::endl << "End of m" << std::endl;
	//std::cout << m(1,1) << std::endl;

	compression(&m, 100);
	std::cout << "Here is m after compression :\n" << m << std::endl;
	
	//remplissage(&m);
	//std::cout << "Matrice m remplie\n" << m << std::endl;
	//mat = Eigen::MatrixXd::Random(12,10);
	//std::cout << mat << std::endl;
}