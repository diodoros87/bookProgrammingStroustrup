CC = g++
CFLAGS = -Wall -std=c++14

strings: strings.o
	$(CC) $(CFLAGS) -o strings strings.o
    
strings.o: strings.cpp 
	$(CC) $(CFLAGS) -c strings.cpp

clean :
	rm *.o strings
