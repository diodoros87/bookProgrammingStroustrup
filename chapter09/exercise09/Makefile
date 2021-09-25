CC = g++
CFLAGS = -Wall -std=c++14 -fcompare-debug-second	

main: main.o Library.o Book.o date.o Patron.o
	$(CC) $(CFLAGS) -o main main.o Library.o Book.o date.o Patron.o
	
main.o: main.cpp error.h Library.h Book.h date.h Patron.h 
	$(CC) $(CFLAGS) -c main.cpp
	
Library.o: Library.cpp Library.h Book.h error.h date.h Patron.h
	$(CC) $(CFLAGS) -c Library.cpp
	
Book.o: Book.cpp Book.h error.h date.h
	$(CC) $(CFLAGS) -c Book.cpp
	
Patron.o: Patron.cpp Patron.h error.h
	$(CC) $(CFLAGS) -c Patron.cpp	
	
date.o: date.cpp date.h
	$(CC) $(CFLAGS) -c date.cpp

clean :
	rm *.o main
