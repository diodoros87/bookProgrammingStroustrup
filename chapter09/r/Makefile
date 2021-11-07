CXX = g++
CPPFLAGS = -g -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 -DMANUAL_DLL_LOAD

CC = gcc
CFLAGS = -g -Wfatal-errors -Wall -Wextra -Wconversion -std=c90 -DMANUAL_DLL_LOAD
#  -fcompare-debug-second 

LPATH=-L.
LIBS=-ldemo

PROGRAMS  = libdemo.so libconnector.so main_cpp c_main 

.PHONY: clean all

all: $(PROGRAMS)

c_main: c_main.o libconnector.so shared_lib_open.o print.o
	$(CC) $(CFLAGS) c_main.o shared_lib_open.o print.o -L. -lconnector -ldemo -ldl -o c_main
	
c_main.o: connector.h main.c shared_lib_open.h print.h
	$(CC) $(CFLAGS) -c main.c -o c_main.o
	
shared_lib_open.o:
	$(CC) $(CFLAGS) -c shared_lib_open.c -o shared_lib_open.o
	
print.o:
	$(CC) $(CFLAGS) -c print.c

main_cpp: main_cpp.o libdemo.so
	$(CXX) $(CPPFLAGS) main_cpp.o -L. -ldemo -o main_cpp
	
libconnector.so: connector.o libdemo.so 
	$(CXX) $(CPPFLAGS) -shared -L. -ldemo -o libconnector.so connector.o 
	
connector.o: connector.h connector.cpp null.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC connector.cpp

main_cpp.o: main.cpp variadic_template.hpp variadic_template.cpp demo.hpp demo.cpp
	$(CXX) $(CPPFLAGS) -c main.cpp -o main_cpp.o
	
libdemo.so: demo.o
	$(CXX) $(CPPFLAGS) -shared -o libdemo.so demo.o

demo.o: demo.hpp demo.cpp null.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo.cpp
	
clean :
	rm $(PROGRAMS) *.o
