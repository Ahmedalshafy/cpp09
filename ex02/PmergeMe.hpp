#pragma once

#include <vector>
#include <deque>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdexcept>

template<typename Container>
class PmergeMe {
private:
    Container sequence;
    double executionTime;

    // Helper functions for Ford-Johnson algorithm
    void makePairs(std::vector<std::pair<typename Container::value_type, 
                                       typename Container::value_type> >& pairs);
    void insertionSort(Container& mainChain, const Container& pending);
    int jacobsthal(int n) const;
    std::vector<int> generateJacobsthalSequence(int n) const;
    typename Container::iterator binarySearch(typename Container::value_type value,
                                           typename Container::iterator begin,
                                           typename Container::iterator end);

public:
    PmergeMe() : executionTime(0) {}
    ~PmergeMe() {}
    
    // Copy constructor and assignment operator
    PmergeMe(const PmergeMe& other) : sequence(other.sequence), 
                                     executionTime(other.executionTime) {}
    PmergeMe& operator=(const PmergeMe& other) {
        if (this != &other) {
            sequence = other.sequence;
            executionTime = other.executionTime;
        }
        return *this;
    }

    // Main functions
    void setSequence(const Container& input) { sequence = input; }
    const Container& getSequence() const { return sequence; }
    double getExecutionTime() const { return executionTime; }

    void sort();
    void printSequence(const std::string& prefix) const;

    class Error : public std::exception {
        private:
            std::string msg;
        public:
            Error(const std::string& message) : msg(message) {}
            virtual const char* what() const throw() { return msg.c_str(); }
            virtual ~Error() throw() {}
    };
};

// Template implementation
#include "PmergeMe.tpp"
