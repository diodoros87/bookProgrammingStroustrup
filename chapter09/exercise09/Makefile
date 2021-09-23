CC = g++
CFLAGS = -Wall -std=c++14 -fcompare-debug-second
	
main: main.o Book.o date.o
	$(CC) $(CFLAGS) -o main main.o Book.o date.o
    
main.o: main.cpp error.h Book.h date.h
	$(CC) $(CFLAGS) -c main.cpp
	
Book.o: Book.cpp Book.h error.h date.h
	$(CC) $(CFLAGS) -c Book.cpp
	
date.o: date.cpp date.h
	$(CC) $(CFLAGS) -c date.cpp

clean :
	rm *.o main
