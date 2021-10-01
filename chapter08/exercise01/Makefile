CC = g++
CFLAGS = -Wall -std=c++14

all: calculator int_calculator

calculator : calculator.o
	$(CC) $(CFLAGS) -o calculator  calculator.o
    
calculator.o: calculator.cpp 
	$(CC) $(CFLAGS) -c calculator.cpp
	
int_calculator: int_calculator.o
	$(CC) $(CFLAGS) -o int_calculator int_calculator.o
    
int_calculator.o: int_calculator.cpp 
	$(CC) $(CFLAGS) -c int_calculator.cpp

clean :
	rm *.o calculator int_calculator 
