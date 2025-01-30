#pragma once
#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

/**
 * @class RPN
 * @brief Implements a Reverse Polish Notation calculator
 * 
 * This class provides functionality to evaluate mathematical expressions
 * written in Reverse Polish Notation (RPN). In RPN, operators follow their
 * operands, eliminating the need for parentheses.
 * 
 * Example: "3 4 +" in RPN is equivalent to "3 + 4" in infix notation
 */
class RPN 
{
private:
    /** @brief Stack to store operands during calculation */
    std::stack<int> stack;
    
    /**
     * @brief Checks if a token is a valid operator
     * @param token String to check
     * @return true if token is +, -, *, or /
     */
    bool isOperator(const std::string& token) const;

    /**
     * @brief Performs arithmetic operation on two operands
     * @param operation The operator (+, -, *, /)
     * @param operand1 First operand
     * @param operand2 Second operand
     * @return Result of the operation
     * @throws std::runtime_error if division by zero or invalid operator
     */
    int performOperation(const std::string& operation, int operand1, int operand2) const;

    /**
     * @brief Converts string to integer with validation
     * @param str String to convert
     * @return Integer value
     * @throws std::invalid_argument if value is invalid or out of range (0-9)
     */
    int stringToInt(const std::string& str) const;

public:
    /** @brief Default constructor */
    RPN();
    
    /** @brief Destructor */
    ~RPN();
    
    /**
     * @brief Copy constructor
     * @param other RPN object to copy from
     */
    RPN(const RPN& other);
    
    /**
     * @brief Assignment operator
     * @param other RPN object to assign from
     * @return Reference to this object
     */
    RPN& operator=(const RPN& other);

    /**
     * @brief Evaluates an RPN expression
     * @param expression String containing the RPN expression
     * @return Result of the evaluation
     * @throws std::runtime_error for invalid expressions
     * 
     * Example: evaluate("3 4 +") returns 7
     */
    int evaluate(const std::string& expression);
};

#endif
