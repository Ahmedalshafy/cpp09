#include "RPN.hpp"

int main(int ac, char** av) {
    if (ac != 2) {
        std::cout << "Usage: ./RPN \"expression\"" << std::endl;
        return 1;
    }

    RPN rpn;
    try {
        int result = rpn.evaluate(av[1]);
        std::cout << result << std::endl;
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
