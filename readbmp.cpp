#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

std::stringstream hexStr(char* data, int len)
{
	std::stringstream ss;
	ss << std::hex;
	for(int i = 0; i < len; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];
	return ss;
}

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
		//const std::string plouf = size;
		//plouf = "0x";
		//plouf += size;
		std::cout << size << std::endl;
		//std::cout << plouf << std::endl;
		//std::stringstream S;
		//S = hexStr(size, 4);

		char* tmp = new char[4];
		is.read(tmp, 4);
		

		int offset;
		is.read(reinterpret_cast<char*>(&offset), 4);
		std::cout << offset << std::endl;
		//is >> std::hex;

		/*for(int i = 2; i < 9; i++)
		{
			//std::cout << std::hex << static_cast<int>(buffer[i]);
			//std::cout << std::dec << "| CHAR " << i << std::endl;
			is >> a;
			std::cout << std::hex << a << std::endl;
		}*/
		std::cout << type << std::endl;
		//std::cout << std::stoi(plouf, nullptr, 16) << std::endl;
		//std::cout << O << std::endl;
		
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