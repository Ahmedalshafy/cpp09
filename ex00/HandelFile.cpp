#include "HandelFile.hpp"

HandelFile::HandelFile(std::string filename) : _filename(filename){
    try {
        readFile();
    }
    catch (HandelFile::FileError &e){
        std::cerr << e.what() << std::endl;
    }
}

HandelFile::HandelFile(const HandelFile &src){
    *this = src;
}

HandelFile &HandelFile::operator=(const HandelFile &src){
    if (this != &src){
        _filename = src._filename;
        _data = src._data;
    }
    return *this;
}

HandelFile::~HandelFile(){
}

void HandelFile::readFile(){
    std::cout << "readFile" << std::endl;
    std::ifstream file(_filename.c_str());
    if (!file.is_open()){
        std::cout << "Error: " << _filename << " could not be opened" << std::endl;
        // throw HandelFile::FileError();
        std::cerr << "Error: " << _filename << " could not be opened" << std::endl;
        return;
    }
    std::cout << "readFile" << std::endl;
    getDataFromFile(file);
    file.close();
}

void HandelFile::getDataFromFile(std::ifstream &file){
    std::string line;
    while (file.eof() == 0){
        std::getline(file, line);
        if (line.empty())
            break;
        _data[line.substr(0, 10)] = convertExRate(line.substr(11));
    }
}

float HandelFile::convertExRate(std::string price){
    std::stringstream ss(price);
    float rate;
    ss >> rate;
    // std::cout << "rate: " << rate << std::endl;
    // if (ss.fail())
    //     throw HandelFile::FileError();
    return rate;
}

// static int convertToInt(std::string str)
// {
//     int ret;
//     std::stringstream ss(str);
//     ss >> ret;
//     if (ss.fail())
//         throw HandelFile::FileError();
//     ss.clear();
//     return (ret);
// }

std:: map<std::string, float> HandelFile::getData(){
    return _data;
}

