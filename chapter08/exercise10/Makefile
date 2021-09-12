CC = g++
CFLAGS = -Wall -std=c++14

index: index.o
	$(CC) $(CFLAGS) -o index index.o
    
index.o: index.cpp 
	$(CC) $(CFLAGS) -c index.cpp

clean :
	rm *.o index
