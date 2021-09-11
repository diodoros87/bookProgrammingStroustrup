CC = g++
CFLAGS = -Wall -std=c++14
	
main: main.o input.o persons.o
	$(CC) $(CFLAGS) -o main main.o input.o persons.o
    
main.o: main.cpp error.h input.h persons.h
	$(CC) $(CFLAGS) -c main.cpp
	
persons.o: persons.cpp persons.h error.h
	$(CC) $(CFLAGS) -c persons.cpp
	
input.o: input.cpp input.h
	$(CC) $(CFLAGS) -c input.cpp

clean :
	rm *.o main
