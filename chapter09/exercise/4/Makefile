CC = g++
CFLAGS = -Wall -std=c++14 -fcompare-debug-second
	
main: main.o date.o
	$(CC) $(CFLAGS) -o main main.o date.o
    
main.o: main.cpp date.h
	$(CC) $(CFLAGS) -c main.cpp
	
date.o: date.cpp date.h
	$(CC) $(CFLAGS) -c date.cpp

clean :
	rm *.o main
