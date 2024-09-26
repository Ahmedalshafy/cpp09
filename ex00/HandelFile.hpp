#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

class HandelFile{
    private:
        std::map<std::string, float> _data;

    protected:
        std::string _filename;

    public:
        HandelFile(std::string filename);
        HandelFile(const HandelFile &src);
        HandelFile &operator=(const HandelFile &src);
        ~HandelFile();
        void readFile();
        void getDataFromFile(std::ifstream &file);
        float convertExRate(std::string price);
        void parseDate(std::string date);
        void parseData();
        std::map<std::string, float> getData();

        class FileError : public std::exception{
            public:
                virtual const char *what() const throw(){
                    return "Error: File could not be opened";
                }
        };

        class DateError : public std::exception{
            public:
                virtual const char *what() const throw(){
                    return "Error: Date is invalid"; 
                }
        };
        
};
