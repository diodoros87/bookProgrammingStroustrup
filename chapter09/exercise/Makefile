CC = g++
CFLAGS = -Wall -std=c++14

all: 1 2 3

1: 1.o
	$(CC) $(CFLAGS) -o 1 1.o
    
1.o: 1.cpp 
	$(CC) $(CFLAGS) -c 1.cpp
	
2: 2.o
	$(CC) $(CFLAGS) -o 2 2.o
    
2.o: 2.cpp 
	$(CC) $(CFLAGS) -c 2.cpp
	
3: 3.o
	$(CC) $(CFLAGS) -o 3 3.o
    
3.o: 3.cpp 
	$(CC) $(CFLAGS) -c 3.cpp

clean :
	rm *.o 1 2 3
