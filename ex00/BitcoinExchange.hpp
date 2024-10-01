#include <fstream>
#include <iostream>
#include <string>
#include <map>

enum e_error {
    NEGATIVE_VALUE,
    INVALID_DATE,
    INVALID_RATE,
};

typedef struct s_finnalData {
    std::string date;
    float value;
    float rate;
    float finalValue;
} t_finnalData;

class BitcoinExchange {
    private:
        std::string _inputfile;
        std::map<std::string, float> _data;
        t_finnalData finalData;

    public:
        BitcoinExchange(std::map<std::string, float> data, std::string inputfile);
        BitcoinExchange(const BitcoinExchange &src);
        BitcoinExchange &operator=(const BitcoinExchange &src);
        ~BitcoinExchange();
        void getFinnalData();
        void readDataFromFile();
        void getDataFromFile(std::ifstream &file);
        // void searchForDate(std::string date);
        // void printFinnalData();
        bool parseData(std::string line);
        std::string parseDate(std::string date);
        class FileError : public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Ivalid file";
                }
        };

        class InvalidInput : public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Invalid input";
                }
        };
};

// std::ofstream &operator<<(std::ofstream &file, BitcoinExchange &src);
