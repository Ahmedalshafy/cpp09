# Program
NAME := btc

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
SRC := BitcoinExchange.cpp main.cpp
INCLUDES := $(SRC:.cpp=.hpp)

# Rules
all: $(NAME)

$(NAME): $(SRC)
	$(CXX) -o $@ $(CXXFLAGS) $(SRC)
	@echo "$(GREEN)Compilation successful!$(RESET)"

clean:
	rm -rf $(NAME)
	@echo "$(YELLOW)Executable removed.$(RESET)"

ifeq ($(valgrind_answer),y)
valgrind:
	@echo "$(CURSIVE)Running valgrind...$(RESET)"
	valgrind --leak-check=full ./$(NAME)
endif

re: clean all

.PHONY: all clean re
