CC = g++
CFLAGS = -Wall -std=c++14

printing: printing.o
	$(CC) $(CFLAGS) -o printing printing.o
    
printing.o: printing.cpp 
	$(CC) $(CFLAGS) -c printing.cpp

clean :
	rm *.o printing
