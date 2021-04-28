#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>



int main()
{
	std::fstream is;
	is.open("ACNH.bmp", std::ios::in|std::ios::binary);
	if(is.is_open())
	{
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);

		std::cout << length << std::endl;

		char* type = new char[2];
		is.read(type, 2);

		int size;
		is.read(reinterpret_cast<char*>(&size), 4);
		//std::cout << size << std::endl;

		char* tmp = new char[4];
		is.read(tmp, 4);

		int offset;
		is.read(reinterpret_cast<char*>(&offset), 4);
		std::cout << offset << std::endl;
		std::cout << type << std::endl;
		std::cout << "FINI DE LIRE" << std::endl;

		if(is)
		{
			std::cout << "Read all data successfully." << std::endl;
		}
		is.close();

		delete[] type;
		//delete[] size;
		delete[] tmp;
		//delete[] offset;
	}
	return 0;
}