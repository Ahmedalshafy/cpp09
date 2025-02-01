#include "RPN.hpp"

// Default constructor - initializes an empty RPN calculator
RPN::RPN() {}

// Copy constructor - creates a deep copy of another RPN calculator
RPN::RPN(const RPN& other) {
    *this = other;
}

// Assignment operator - performs deep copy of the stack
RPN& RPN::operator=(const RPN& other) {
    if (this != &other)
        this->stack = other.stack;
    return *this;
}

// Destructor - stack is automatically cleaned up
RPN::~RPN() {}

bool RPN::isOperator(const std::string& token) const {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

int RPN::performOperation(const std::string& operation, int operand1, int operand2) const {
    if (operation == "+") return operand1 + operand2;
    if (operation == "-") return operand1 - operand2;
    if (operation == "*") return operand1 * operand2;
    if (operation == "/") {
        // Check for division by zero
        if (operand2 == 0) 
            throw std::runtime_error("Division by zero");
        return operand1 / operand2;
    }
    throw std::runtime_error("Invalid operator");
}

int RPN::stringToInt(const std::string& str) const {
    std::stringstream ss(str);
    int result;
    
    // Try to convert string to integer
    ss >> result;
    
    // Validate the conversion and value range
    if (ss.fail() || !ss.eof() || result < 0 || result > 9)
        throw std::invalid_argument("Invalid integer value");
    return result;
}

int RPN::evaluate(const std::string& expression) {
    std::istringstream iss(expression);
    std::string token;

    // Process each token in the expression
    while (iss >> token) {
        // Case 1: Token is a single digit number
        if (isdigit(token[0]) && token.length() == 1)
            stack.push(stringToInt(token));
        
        // Case 2: Token is an operator
        else if (isOperator(token)) {
            // Check if we have enough operands
            if (stack.size() < 2) 
                throw std::runtime_error("Invalid expression");
            
            // Pop operands in reverse order
            int operand2 = stack.top(); stack.pop();  // Second operand
            int operand1 = stack.top(); stack.pop();  // First operand
            
            // Perform operation and push result
            int result = performOperation(token, operand1, operand2);
            stack.push(result);
        } 
        // Case 3: Invalid token
        else
            throw std::runtime_error("Invalid input token");
    }

    // Valid RPN expression should leave exactly one number on the stack
    if (stack.size() != 1) 
        throw std::runtime_error("Invalid expression");
        
    return stack.top();
}
