cmake_minimum_required(VERSION 3.0.0)
project(testQt VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
#find_package(Qt5Designer)
find_package(Qt5Charts)
find_package(Qt5Gui)
find_package(Eigen3 3.3)

SET(CMAKE_AUTOMOC True)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

SET(SRCS
	main.cpp
	couche.cpp
	reseau.cpp
	gestionnaireMemoire.cpp
	MainWindow.cpp
	ext.cpp 
)
SET(HEADERS
	couche.hpp
	reseau.hpp
	gestionnaireMemoire.hpp
	MainWindow.hpp
	ext.hpp
)

add_executable(main ${SRCS} ${HEADERS})

target_link_libraries(main Qt5::Widgets Eigen3::Eigen Qt5::Charts Qt5::Gui)