CC = g++
CFLAGS = -Wall -std=c++14 -fcompare-debug-second
	
main: main.o rational.o
	$(CC) $(CFLAGS) -o main main.o rational.o
    
main.o: main.cpp rational.h
	$(CC) $(CFLAGS) -c main.cpp
	
rational.o: rational.cpp rational.h
	$(CC) $(CFLAGS) -c rational.cpp

clean :
	rm *.o main
