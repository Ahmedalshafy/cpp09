#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(std::map<std::string, float> data, std::string inputfile){
    _data = data;
    _inputfile = inputfile;
    try {
        readDataFromFile();
        std::cout << this << std::endl;
    }
    catch (FileError &e){
        std::cerr << e.what() << std::endl;
    }      
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &src){
    *this = src;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &src){
    if (this != &src){
        _data = src._data;
        _finnalMap = src._finnalMap;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange(){
}

void BitcoinExchange::readDataFromFile(){
    std::ifstream file(_inputfile);
    if (!file.is_open())
        throw FileError();
    getDataFromFile(file);
    file.close();
}

void BitcoinExchange::getDataFromFile(std::ifstream &file){
    std::string line;
    std::string str;
    std::array<std::string, 2> data_line;
    std::getline(file, line);
    if (line.empty())
        throw FileError();
    if (line != "date | value")
        throw FileError();
    while (file.eof() == 0){
        std::getline(file, line);
        if (line.empty())
            break;
        if (parseData(line))
            searchForDate(_finnalMap[line].date);
    }   
}

bool BitcoinExchange::parseData(std::string line){
    std::string date;
    std::string value;
    t_finnalData finalData;

    if (line[5] != '-' || line[8] != '-' || line[11] != ' ' || line[12] != '|' || line[13] != ' ')
            throw FileError();
    date = line.substr(0, 11);
    date = parseDate(date);
    value = line.substr(14);
    finalData.date = date;
    finalData.rate = std::stof(value);
    _finnalMap[date] = finalData;
}

std::string BitcoinExchange::parseDate(std::string date){
    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));
    if (year < 2009 || (year == 2009 && month == 1 && day < 3))
        return "Invalid date" + date;
    if (month < 1 || month > 12)
        return "Invalid date" + date;
    if (day < 1 || day > 31)
        return "Invalid date" + date;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return "Invalid date" + date;
    if ((month == 2 && day > 29) || (month == 2 && day > 28 && year % 4 != 0))
        return "Invalid date" + date;
    return date;
}

void BitcoinExchange::searchForDate(std::string date){
    std::map<std::string, float>::iterator it = _data.find(date);
    if (it == _data.end()){
        std::map<std::string, float>::iterator it = _data.upper_bound(date);
    }
    _finnalMap[date].value = it->second;
    _finnalMap[date].finalValue = it->second * _finnalMap[date].rate;
}

std::ofstream &operator<<(std::ofstream &file, BitcoinExchange &src){
    src.printFinnalData();
    return file;
}

void BitcoinExchange::printFinnalData(){
    std::map<std::string, t_finnalData>::iterator it = _finnalMap.begin();
    while (it != _finnalMap.end()){
        if (it->second.value < 0)
            throw NEGATIVE_VALUE;
        if (it->second.date == "Invalid date")
            throw INVALID_DATE;
        if (it->second.rate < 0)
            throw INVALID_RATE;
        else
            std::cout << it->second.date << " | " << it->second.value << " | " 
            << it->second.rate << " | " << it->second.finalValue << std::endl;
    }
}