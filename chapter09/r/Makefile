CXX = g++
CPPFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 
#  -fcompare-debug-second 
PROGRAMS  = main_cpp

.PHONY: clean all

all: $(PROGRAMS)

main_cpp: main_cpp.o demo.o
	$(CXX) $(CPPFLAGS) -o main_cpp main_cpp.o demo.o

main_cpp.o: main.cpp variadic_template.hpp variadic_template.cpp demo.hpp demo.cpp
	$(CXX) $(CPPFLAGS) -c main.cpp -o main_cpp.o

demo.o: demo.hpp demo.cpp
	$(CXX) $(CPPFLAGS) -c demo.cpp
	
clean :
	rm $(PROGRAMS) *.o
