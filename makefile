VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -Werror=parentheses -O0
CC = gcc

all: clean valgrind

main: main.c src/*
	$(CC) $(CFLAGS) src/*.c main.c -o main 2>&1

valgrind: main
	valgrind $(VALGRIND_FLAGS) ./main pokemon1.csv pokemon2.csv 2>&1

valgrind-pruebas: pruebas
	valgrind $(VALGRIND_FLAGS) ./pruebas pokemon1.csv pokemon2.csv 2>&1

pruebas: pruebas_chanutron.c src/*.c
	$(CC) $(CFLAGS) src/*.c pruebas_chanutron.c -o pruebas 2>&1

clean:
	rm -f main *.o pruebas 2>&1
