SRC=./src
OBJ=./obj
FLG=@/Users/jakubbednarski/.config/flags/flags_g++


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

$(OBJ)/snake.o: $(SRC)/snake.cpp $(SRC)/winsys.h $(SRC)/screen.h $(SRC)/cpoint.h $(SRC)/snake.h $(SRC)/stack.h
	g++ -g -c $(FLG) $< -o $@

.PHONY: clean

clean:
	-rm winsys $(OBJ)/*.o
	-rm -rf winsys.dSYM
