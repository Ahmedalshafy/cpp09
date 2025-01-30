# Program
NAME := RPN
# Necessities
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98

#Colors:
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

# Targets
MAIN := main.cpp
SRC := RPN.cpp
INCLUDES := RPN.hpp

# Rules
all: $(NAME)

$(NAME): $(MAIN) $(INCLUDES) $(SRC)
	@$(CXX) $(CXXFLAGS) $(MAIN) $(SRC) -o $(NAME)
	@printf "$(GREEN)Compilation successful!$(RESET)\n"

clean:
	@rm -rf $(NAME)
	@printf "$(YELLOW)Executable removed.$(RESET)\n"

re: clean all

valgrind:
	@printf "$(CURSIVE)Running valgrind...$(RESET)\n"
	valgrind --leak-check=full ./$(NAME)

.PHONY: all clean re valgrind
