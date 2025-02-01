#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <string>

void createTestFile(const std::string& filename) {
    std::ofstream outFile(filename.c_str());
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create test file." << std::endl;
        return;
    }

    outFile << "date | value\n";
    outFile << "2011-01-03 | 3\n";
    outFile << "2011-01-03 | 2\n";
    outFile << "2011-01-03 | 1\n";
    outFile << "2011-01-03 | 1.2\n";
    outFile << "2011-01-09 | 1\n";
    outFile << "2012-01-11 | -1\n";
    outFile << "2001-42-42\n";
    outFile << "2012-01-11 | 1\n";
    outFile << "2012-01-11 | 2147483648\n";
    outFile << "2020-01-01 | 500\n";
    outFile << "2019-12-31 | 999.99\n";
    outFile << "invalid_date | 100\n";
    outFile << "2022-03-15 | abc\n";
    outFile << "2023-01-01 | 1001\n";  // Test value > 1000
    outFile << "2015-03-15 | 0\n";     // Test zero value
    outFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: could not open file." << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Create a test file if it doesn't exist
    std::string testFile = "input.txt";
    createTestFile(testFile);

    try {
        BitcoinExchange btc("resources/data.csv");
        btc.processInput(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}