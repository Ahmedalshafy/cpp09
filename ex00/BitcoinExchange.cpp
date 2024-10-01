#include "BitcoinExchange.hpp"
#include <sstream>

static int convertToInt(std::string str)
{
    int ret;
    std::stringstream ss(str);
    ss >> ret;
    if (ss.fail())
        throw BitcoinExchange::InvalidInput();
    ss.clear();
    return (ret);
}

static float convertToFloat(std::string str)
{
    float ret;
    std::stringstream ss(str);
    ss >> ret;
    if (ss.fail() || ret < 0 || ret > 1000)
        throw std::exception();
    ss.clear();
    return (ret);
}

BitcoinExchange::BitcoinExchange(std::map<std::string, float> data, std::string inputfile){
    _data = data;
    _inputfile = inputfile;
    try {
        readDataFromFile();
        // std::cout << "this: " << std::endl;
        // this->printFinnalData();
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
        _inputfile = src._inputfile;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange(){
}

void BitcoinExchange::readDataFromFile(){
    
    std::ifstream file(_inputfile.c_str());
    if (!file.is_open())
    {
        // throw FileError();
        std::cerr << "Error: File " << _inputfile << " could not be opened" << std::endl;
        return;
    }
    getDataFromFile(file);
    file.close();
}

void BitcoinExchange::getDataFromFile(std::ifstream &file){
    std::string line;
    std::getline(file, line);
    if (line.empty())
        throw FileError();
    if (line != "date | value")
        throw FileError();
    while (file.eof() == 0){
        std::getline(file, line);
        std::cout << "line: " << line << std::endl;
        if (line.empty())
            break;
        if (!parseData(line))
        {
            std::cerr << "Error: Invalid data" << std::endl;
        }
        else
        {
            std::map<std::string, float>::iterator it = _data.find(finalData.date);
            if (it == _data.end()){
                it = _data.upper_bound(finalData.date);
            }
                finalData.rate = it->second;
                finalData.finalValue = finalData.value * finalData.rate;
            
            std::cout << "date: " << finalData.date << " | value: " << finalData.value << " | rate: " 
                << finalData.rate << " | finalValue: " << finalData.finalValue << std::endl;
            // searchForDate(line.substr(0, 11));
        }
        // std::cout << "line: " << line << std::endl;
        // std::cout << "date: " << line.substr(0, 11) << std::endl;
        
    }   
}

bool BitcoinExchange::parseData(std::string line){
    std::string date;
    std::string value;

    if (line.length() < 14 || line[4] != '-' || line[7] != '-' || line[10] != ' ' 
        || line[11] != '|' || line[12] != ' ')
    {
        finalData.date = "Invalid date";
        return false;
    }
    date = line.substr(0, 11);
    date = parseDate(date);
    value = line.substr(13);
    if (value.find('.') == std::string::npos)
    {
        try{
            finalData.value = convertToInt(value);
        }
        catch (std::exception &e){
            return false;
        }
    }
    finalData.date = date;
    try {
        finalData.value = convertToFloat(value);
    }
    catch (std::exception &e){
        return false;
    }
    return true;
}

std::string BitcoinExchange::parseDate(std::string date){
    int year = convertToInt(date.substr(0, 4));
    int month = convertToInt(date.substr(5, 2));
    int day = convertToInt(date.substr(8, 2));
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

// void BitcoinExchange::searchForDate(std::string date){
//     std::map<std::string, float>::iterator it = _data.find(date);
//     if (it == _data.end()){
//         it = _data.upper_bound(date);
//     }
//     std::cout << "rate " << it->second << std::endl;
//     _finnalMap[date].rate = it->second;
//     _finnalMap[date].finalValue = _finnalMap[date].value * it->second;
//     std::cout << "date: " << _finnalMap[date].date << " | value: " << _finnalMap[date].value << " | rate: " 
//     << _finnalMap[date].rate << " | finalValue: " << _finnalMap[date].finalValue << std::endl;
// }

// std::ofstream &operator<<(std::ofstream &file, BitcoinExchange &src){
//     src.printFinnalData();
//     return file;
// }

// void BitcoinExchange::printFinnalData(){
//     std::map<std::string, t_finnalData>::iterator it = _finnalMap.begin();
//     while (it != _finnalMap.end()){
//         // std::cout << "date: " << it->second.date << " | value: " << it->second.value << " | rate: " << it->second.rate << " | finalValue: " << it->second.finalValue << std::endl;
//         if (it->second.value < 0)
//             std::cout << it->second.date << "Negative value" << std::endl;
//         else if (it->second.date == "Invalid date")
//             std::cout << it->second.date << std::endl;
//         else if (it->second.rate < 0)
//             std::cout << it->second.date << " | " << it->second.value << " | " 
//             << "Invalid rate" << std::endl;
//         else
//             std::cout << it->second.date << " | " << it->second.value << " | " 
//             << it->second.rate << " | " << it->second.finalValue << std::endl;
//         it++;
//     }
// }
