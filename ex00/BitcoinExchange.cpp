#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) 
{
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) 
{
    if (this != &other)
        this->btcData = other.btcData;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const std::string& dbFilename) {loadDatabase(dbFilename);}

void BitcoinExchange::loadDatabase(const std::string& filename) 
{
    std::ifstream file(filename.c_str());
    if (!file.is_open()) 
    {
        std::cerr << "Error: could not open database file." << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) 
    {
        std::stringstream ss(line);
        std::string date;
        std::string valueStr;
        float value;

        if (!std::getline(ss, date, ',')) 
        {
            std::cerr << "Error: bad input in database => " << line << std::endl;
            continue;
        }

        if (!std::getline(ss, valueStr)) 
        {
            std::cerr << "Error: bad input in database => " << line << std::endl;
            continue;
        }

        try 
        {
            value = customStof(valueStr);
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error: bad input in database => " << line << std::endl;
            continue;
        }
        
        btcData[date] = value;
    }
    
    file.close();
}

void BitcoinExchange::processInput(const std::string& inputFilename) const 
{
    std::ifstream file(inputFilename.c_str());
    if (!file.is_open()) 
    {
        std::cerr << "Error: could not open input file." << std::endl;
        return;
    }

    std::string line; 
    std::getline(file, line);

    while (std::getline(file, line)) 
    {
        std::stringstream ss(line);
        std::string date;
        std::string valueStr;
        double value;

        if (!std::getline(ss, date, '|')) 
        {
            std::cerr << "Error: bad date => " << line << std::endl;
            continue;
        }

        if (!std::getline(ss, valueStr)) 
        {
            std::cerr << "Error: bad value => " << line << std::endl;
            continue;
        }

        date = date.substr(date.find_first_not_of(" \t"), date.find_last_not_of(" \t") - date.find_first_not_of(" \t") + 1);
        valueStr = valueStr.substr(valueStr.find_first_not_of(" \t"), valueStr.find_last_not_of(" \t") - valueStr.find_first_not_of(" \t") + 1);

        try 
        {
            value = customStod(valueStr);
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error: bad float input => " << line << std::endl;
            continue;
        }

        if (date.length() != 10 || date[4] != '-' || date[7] != '-') 
        {
            std::cerr << "Error: invalid date => " << line << std::endl;
            continue;
        }

        if (value < 0) 
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000) 
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        std::map<std::string, double>::const_iterator it = btcData.lower_bound(date);
        double result = 0;

        if (it != btcData.end() && it->first == date)
        {
            result = value * it->second;
        }
        else if (it != btcData.begin()) 
        {
            --it;
            result = value * it->second;
        }
        else
        {
            std::cerr << "Error: date not found in database." << std::endl;
            continue;
        }

        if (result > static_cast<double>(INT_MAX)) 
        {
            std::cout << std::fixed << std::setprecision(2) << date << " => " << value << " = Overflow" << std::endl;
        }
        else
        {
            std::cout << std::fixed << std::setprecision(2) << date << " => " << value << " = " << result << std::endl;
        }
    }
    file.close();
}

double BitcoinExchange::customStod(const std::string& str) const
{
    std::istringstream iss(str);
    double result;
    iss >> result;
    // Check for any remaining characters after the float value
    if (iss.fail() || !iss.eof() || iss.peek() != std::char_traits<char>::eof())
        throw std::invalid_argument("Invalid float value");
    return result;
}


float BitcoinExchange::customStof(const std::string& str) const 
{
    std::istringstream iss(str);
    float result;
    iss >> result;
    // Check for any remaining characters after the float value
    if (iss.fail() || !iss.eof() || iss.peek() != std::char_traits<char>::eof())
        throw std::invalid_argument("Invalid float value");
    return result;
}
