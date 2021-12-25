SRC=./src
OBJ=./obj
FLG=-g -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wswitch-default -Wswitch-enum -Wunreachable-code -fno-elide-constructors -pedantic -std=c++11

all: $(OBJ) winsys

# Creating folder
$(OBJ):
	mkdir -p $(OBJ)

# Dynamik linking
winsys: $(OBJ)/main.o $(OBJ)/screen.o $(OBJ)/winsys.o $(OBJ)/snake.o
	g++ -g $(FLG) $^ -o $@ -lncurses

# With debugging symbols
$(OBJ)/main.o: $(SRC)/main.cpp $(SRC)/winsys.h $(SRC)/screen.h $(SRC)/cpoint.h $(SRC)/snake.h
	g++ -g -c $(FLG) $< -o $@

$(OBJ)/screen.o: $(SRC)/screen.cpp $(SRC)/screen.h $(SRC)/cpoint.h
	g++ -g -c $(FLG) $< -o $@

$(OBJ)/winsys.o: $(SRC)/winsys.cpp $(SRC)/winsys.h $(SRC)/screen.h $(SRC)/cpoint.h
	g++ -g -c $(FLG) $< -o $@

$(OBJ)/snake.o: $(SRC)/winsys.h $(SRC)/screen.h $(SRC)/cpoint.h $(SRC)/snake.h $(SRC)/stack.h
	g++ -g -c $(FLG) $< -o $@

.PHONY: clean

clean:
	-rm winsys $(OBJ)/*.o
	-rm -rf winsys.dSYM
