SRC = src/main.cpp src/board.cpp
OBJF = $(SRC:.cpp=.o)
NAME = n-puzzle
CXX = clang++
CXXFLAGS = -Wall -g -Wextra -std=c++98  -Iinclude

$(NAME) : $(OBJF) include/board.hpp
		$(CXX) $(CXXFLAGS) $(OBJF) -o $(NAME)

%.o: %.cpp include/board.hpp
		$(CXX) $(CXXFLAGS) -o $@ -c $<

all : $(NAME)

clean :
		rm -f $(OBJF)

fclean : clean
		rm -f $(NAME)

re : fclean all
