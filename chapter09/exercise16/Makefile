CC = g++
CFLAGS = -Wall -std=c++14 -fcompare-debug-second
	
main: main.o money.o
	$(CC) $(CFLAGS) -o main main.o money.o
    
main.o: main.cpp money.h
	$(CC) $(CFLAGS) -c main.cpp
	
money.o: money.cpp money.h
	$(CC) $(CFLAGS) -c money.cpp

clean :
	rm *.o main
