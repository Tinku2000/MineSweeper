
all: Minesweeper

Minesweeper : Final2.o
	g++ Final2.o -o Minesweeper

Final2.o: Final2.cpp
	g++ -c Final2.cpp


clean:
	rm -rf *o Minesweeper
