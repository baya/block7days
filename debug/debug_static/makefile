main: main.o test.o
	gcc -o main.out main.o test.o

main.o: main.c test.h
	gcc -c main.c

test.o: test.c test.h
	gcc -c test.c