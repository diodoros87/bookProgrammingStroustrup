CC = g++
CFLAGS = -Wall -std=c++14 -fcompare-debug-second
	
main: main.o Book.o date.o Patron.o
	$(CC) $(CFLAGS) -o main main.o Book.o date.o Patron.o
    
main.o: main.cpp error.h Book.h date.h Patron.h
	$(CC) $(CFLAGS) -c main.cpp
	
Book.o: Book.cpp Book.h error.h date.h
	$(CC) $(CFLAGS) -c Book.cpp
	
date.o: date.cpp date.h
	$(CC) $(CFLAGS) -c date.cpp
	
Patron.o: Patron.cpp Patron.h
	$(CC) $(CFLAGS) -c Patron.cpp

clean :
	rm *.o main
