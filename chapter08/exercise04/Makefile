CC = g++
CFLAGS = -Wall -std=c++14

main: main.o fibonacci.o
	$(CC) $(CFLAGS) -o main main.o fibonacci.o
    
main.o: main.cpp fibonacci.h
	$(CC) $(CFLAGS) -c main.cpp
    
fibonacci.o: fibonacci.cpp fibonacci.h
	$(CC) $(CFLAGS) -c fibonacci.cpp

clean :
	rm *.o main
