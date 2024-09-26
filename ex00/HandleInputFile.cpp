#include "HandleInputFile.hpp"


HandleInputFile::HandleInputFile(std::string filename) : HandelFile(filename){
    readFile();
}

HandleInputFile::HandleInputFile(const HandleInputFile &src){
    *this = src;
}

HandleInputFile &HandleInputFile::operator=(const HandleInputFile &src){
    if (this != &src){
        _filename = src._filename;
        _data = src._data;
    }
    return *this;
}

HandleInputFile::~HandleInputFile(){
}

int HandleInputFile::convertValue(std::string value){
    int value_int;
    try{
        value_int = std::stoi(value);
    }
    catch (std::exception &e){
        throw HandelFile::DataError();
    }
    return value_int;
}

void HandleInputFile::getDataFromFile(std::ifstream &file){
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
        _data[data_line[0]] = convertValue(data_line[1]);
    }
}

std::map<int, int> HandleInputFile::getData(){
    return _data;
}