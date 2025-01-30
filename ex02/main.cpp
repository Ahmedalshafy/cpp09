#include "PmergeMe.hpp"

bool isValidNumber(const std::string& str) {
    if (str.empty())
        return false;

    // Check if the string contains only digits
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i]))
            return false;
    }

    // Convert and check range
    std::istringstream iss(str);
    int num;
    iss >> num;
    return num >= 0;
}

template<typename Container>
void processSequence(const std::vector<int>& input, const std::string& containerName) {
    PmergeMe<Container> sorter;
    Container sequence;
    
    // Copy input to sequence
    for (std::vector<int>::const_iterator it = input.begin(); it != input.end(); ++it) {
        sequence.push_back(*it);
    }
    
    sorter.setSequence(sequence);
    sorter.printSequence("Before: ");
    
    sorter.sort();
    
    sorter.printSequence("After:  ");
    std::cout << "Time to process a range of " << input.size() 
              << " elements with " << containerName << ": "
              << std::fixed << std::setprecision(5) 
              << sorter.getExecutionTime() << " us" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No input sequence provided." << std::endl;
        return 1;
    }

    try {
        std::vector<int> input;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (!isValidNumber(arg))
                throw PmergeMe<std::vector<int> >::Error("Invalid number: " + arg);
                
            int num;
            std::istringstream(arg) >> num;
            input.push_back(num);
        }

        // Process with different containers
        processSequence<std::vector<int> >(input, "std::vector");
        processSequence<std::deque<int> >(input, "std::deque");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
