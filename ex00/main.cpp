#include <iostream>
#include "BitcoinExchange.hpp"
#include "HandelFile.hpp"

int main(int argc, char **argv){
	if (argc != 2){
		std::cerr << "Usage: ./bitcoin [inputfile]" << std::endl;
		return 1;
	}
	HandelFile file("data.csv");
	BitcoinExchange exchange(file.getData(), argv[1]);
	return 0;
}

