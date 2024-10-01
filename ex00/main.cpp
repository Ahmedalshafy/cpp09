#include <iostream>
#include "BitcoinExchange.hpp"
#include "HandelFile.hpp"
#include <map>
#include <iterator>

// map << operator to print out the map
std::ostream &operator<<(std::ostream &os, const std::map<std::string, float> &m){
	for (std::map<std::string, float>::const_iterator it = m.begin(); it != m.end(); ++it){
		os << it->first << " " << it->second << std::endl;
	}
	return os;
}

int main(int argc, char **argv){
	if (argc != 2){
		std::cerr << "Usage: ./bitcoin [inputfile]" << std::endl;
		return 1;
	}
	HandelFile file("data.csv");
	// std::cout << file.getData().size() << std::endl;
	// std::cout << file.getData() << std::endl;
	BitcoinExchange exchange(file.getData(), argv[1]);
	return 0;
}

