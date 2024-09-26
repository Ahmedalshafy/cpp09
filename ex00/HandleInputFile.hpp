#include "HandelFile.hpp"

class HandleInputFile : public HandelFile{
    private:
        std::map<std::string, int> _data;
    public:
        HandleInputFile(std::string filename);
        HandleInputFile(const HandleInputFile &src);
        HandleInputFile &operator=(const HandleInputFile &src);
        ~HandleInputFile();
        int convertValue(std::string price);
        void getDataFromFile(std::ifstream &file);
        std::map<std::string, int> getData();
};