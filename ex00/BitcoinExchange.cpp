#include "BitcoinExchange.hpp"

/**
 * @brief Default constructor for BitcoinExchange class
 */
BitcoinExchange::BitcoinExchange() {}

/**
 * @brief Copy constructor for BitcoinExchange class
 * @param other The BitcoinExchange object to copy from
 */
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) 
{
    *this = other;
}

/**
 * @brief Assignment operator overload
 * @param other The BitcoinExchange object to assign from
 * @return Reference to the current object
 */
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) 
{
    if (this != &other)
        this->btcData = other.btcData;
    return *this;
}

/**
 * @brief Destructor for BitcoinExchange class
 */
BitcoinExchange::~BitcoinExchange() {}

/**
 * @brief Parameterized constructor that initializes the object with data from a database file
 * @param dbFilename Path to the database file containing Bitcoin exchange rates
 */
BitcoinExchange::BitcoinExchange(const std::string& dbFilename) {loadDatabase(dbFilename);}

/**
 * @brief Loads Bitcoin exchange rate data from a CSV file
 * @param filename Path to the CSV file containing exchange rate data
 * @details Format of CSV file should be: date,exchange_rate
 *          The first line (header) is skipped during processing
 * @throws Exits program if file cannot be opened
 */
void BitcoinExchange::loadDatabase(const std::string& filename) 
{
    // Open the database file in read mode
    std::ifstream file(filename.c_str());
    if (!file.is_open()) 
    {
        std::cerr << "Error: could not open database file." << std::endl;
        exit(1);
    }

    std::string line;
    // Skip the CSV header (first line containing column names)
    std::getline(file, line);

    while (std::getline(file, line)) 
    {
        // Create a string stream to parse each CSV line
        std::stringstream ss(line);
        std::string date;
        std::string valueStr;
        float value;

        // Extract date string (everything before the comma)
        if (!std::getline(ss, date, ',')) 
        {
            std::cerr << "Error: bad input in database => " << line << std::endl;
            continue;
        }

        // Extract value string (everything after the comma)
        if (!std::getline(ss, valueStr)) 
        {
            std::cerr << "Error: bad input in database => " << line << std::endl;
            continue;
        }

        // Convert string value to float, handle conversion errors
        try 
        {
            value = customStof(valueStr);
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error: bad input in database => " << line << std::endl;
            continue;
        }
        
        // Store the date-value pair in the btcData map
        btcData[date] = value;
    }
    
    file.close();
}

/**
 * @brief Processes input file containing Bitcoin calculations
 * @param inputFilename Path to the input file to process
 * @details Format of input file should be: date | value
 *          Validates dates, values, and performs calculations
 *          Outputs results or appropriate error messages
 */
void BitcoinExchange::processInput(const std::string& inputFilename) const 
{
    // Open input file for processing
    std::ifstream file(inputFilename.c_str());
    if (!file.is_open()) 
    {
        std::cerr << "Error: could not open input file." << std::endl;
        return;
    }

    std::string line; 
    // Skip header line of input file
    std::getline(file, line);

    while (std::getline(file, line)) 
    {
        std::stringstream ss(line);
        std::string date;
        std::string valueStr;
        double value;

        // Parse date (everything before the '|' character)
        if (!std::getline(ss, date, '|')) 
        {
            std::cerr << "Error: bad date => " << line << std::endl;
            continue;
        }

        // Parse value (everything after the '|' character)
        if (!std::getline(ss, valueStr)) 
        {
            std::cerr << "Error: bad value => " << line << std::endl;
            continue;
        }

        // Remove leading and trailing whitespace from both date and value
        date = date.substr(date.find_first_not_of(" \t"), date.find_last_not_of(" \t") - date.find_first_not_of(" \t") + 1);
        valueStr = valueStr.substr(valueStr.find_first_not_of(" \t"), valueStr.find_last_not_of(" \t") - valueStr.find_first_not_of(" \t") + 1);

        // Convert value string to double, handle conversion errors
        try 
        {
            value = customStod(valueStr);
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error: bad float input => " << line << std::endl;
            continue;
        }

        // Validate date format (YYYY-MM-DD)
        if (date.length() != 10 || date[4] != '-' || date[7] != '-') 
        {
            std::cerr << "Error: invalid date => " << line << std::endl;
            continue;
        }

        // Validate value range (0-1000)
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

        // Find the closest date in database that's not after the input date
        std::map<std::string, double>::const_iterator it = btcData.lower_bound(date);
        double result = 0;

        // If exact date match found
        if (it != btcData.end() && it->first == date)
        {
            result = value * it->second;
        }
        // If no exact match, use the closest previous date
        else if (it != btcData.begin()) 
        {
            --it;  // Move to previous date
            result = value * it->second;
        }
        else
        {
            std::cerr << "Error: date not found in database." << std::endl;
            continue;
        }

        // Check for potential integer overflow before output
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

/**
 * @brief Converts string to double with validation
 * @param str String to convert
 * @return Converted double value
 * @throws std::invalid_argument if string is not a valid float value
 */
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

/**
 * @brief Converts string to float with validation
 * @param str String to convert
 * @return Converted float value
 * @throws std::invalid_argument if string is not a valid float value
 */
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
