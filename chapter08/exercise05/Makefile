CC = g++
CFLAGS = -Wall -std=c++14

reversing: reversing.o
	$(CC) $(CFLAGS) -o reversing reversing.o
    
reversing.o: reversing.cpp 
	$(CC) $(CFLAGS) -c reversing.cpp

clean :
	rm *.o reversing
