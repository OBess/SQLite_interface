CXX = g++
cc = gcc

LIB = -lpthread -ldl
BIN = sqlite main

all: $(BIN)
sqlite: include/sqlite/sqlite3.c include/sqlite/shell.c
	$(cc) -o $@ $^ $(LIB)

main: main.cpp sqlite3.o
	$(CXX) -O0 -std=c++17 -o $@ $^ $(LIB)

sqlite3.o: include/sqlite/sqlite3.c
	$(cc) -o $@ -c $^
 
clean:
	rm -f $(BIN)
 
.PHONY: all, clean