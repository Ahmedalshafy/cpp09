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
    std::ifstream file(_filename);
    if (!file.is_open()){
        throw HandelFile::FileError();
    }
    getDataFromFile(file);
    file.close();
}

void HandelFile::getDataFromFile(std::ifstream &file){
    std::string line;
    std::array<std::string, 2> data_line;
    while (file.eof() == 0){
        std::getline(file, line, ',');
        if (line.empty())
            break;
        data_line[0] = line;
        std::getline(file, line);
        if (line.empty())
            break;
        data_line[1] = line;
        _data[data_line[0]] = convertExRate(data_line[1]);
    }
}

float HandelFile::convertExRate(std::string price){
    std::stringstream ss(price);
    float rate;
    ss >> rate;
    if (ss.fail())
        throw HandelFile::FileError();
    return rate;
}

static int convertToInt(std::string str)
{
    int ret;
    std::stringstream ss(str);
    ss >> ret;
    if (ss.fail())
        throw HandelFile::FileError();
    ss.clear();
    return (ret);
}

std:: map<std::string, float> HandelFile::getData(){
    return _data;
}

