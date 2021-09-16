CC = g++
CFLAGS = -Wall -std=c++14

all: 1 2

1: 1.o
	$(CC) $(CFLAGS) -o 1 1.o
    
1.o: 1.cpp 
	$(CC) $(CFLAGS) -c 1.cpp
	
2: 2.o
	$(CC) $(CFLAGS) -o 2 2.o
    
2.o: 2.cpp 
	$(CC) $(CFLAGS) -c 2.cpp

clean :
	rm *.o 1 2
