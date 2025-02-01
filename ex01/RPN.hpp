#pragma once
#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

class RPN 
{
private:
    std::stack<int> stack;
    bool isOperator(const std::string& token) const;
    int performOperation(const std::string& operation, int operand1, int operand2) const;
    int stringToInt(const std::string& str) const;

public:
    RPN();
    ~RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    int evaluate(const std::string& expression);
};

#endif
