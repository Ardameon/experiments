all: a.out

test.o: test.c observer.h
	gcc -Wall -c test.c

main.o: main.cpp observer.h
	gcc -Wall -c main.cpp

a.out: test.o main.o
	g++ -Wall -Wextra main.o test.o -o c_code_in_cpp

.PHONY: all
