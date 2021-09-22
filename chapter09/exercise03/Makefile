CC = g++
CFLAGS = -Wall -std=c++14 -fcompare-debug-second
	
main: main.o input.o Name_pairs.o
	$(CC) $(CFLAGS) -o main main.o input.o Name_pairs.o
    
main.o: main.cpp error.h Name_pairs.h input.h
	$(CC) $(CFLAGS) -c main.cpp
	
Name_pairs.o: Name_pairs.cpp Name_pairs.h error.h input.h
	$(CC) $(CFLAGS) -c Name_pairs.cpp
	
input.o: input.cpp input.h
	$(CC) $(CFLAGS) -c input.cpp

clean :
	rm *.o main
