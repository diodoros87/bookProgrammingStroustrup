CC = g++
CFLAGS = -Wall -std=c++14

statistics: statistics.o
	$(CC) $(CFLAGS) -o statistics statistics.o
    
statistics.o: statistics.cpp 
	$(CC) $(CFLAGS) -c statistics.cpp

clean :
	rm *.o statistics
