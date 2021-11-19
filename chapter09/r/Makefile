CXX = g++
CPPFLAGS= -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 

CC = gcc
CFLAGS= -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 
#  -fcompare-debug-second -DMANUAL_DLL_LOAD

PROGRAMS  = libdemo.so libconnector.so main_cpp c_main c_linking_test linking_test_cpp

.PHONY: clean all

all: $(PROGRAMS)

c_main: c_main.o
	$(CC) $(CFLAGS) c_main.o -o c_main 
	
c_main.o: main.c print.h
	$(CC) $(CFLAGS) -c main.c -o c_main.o 

c_linking_test: c_linking_test.o libconnector.so shared_lib_open.o print.o
	$(CC) $(CFLAGS) c_linking_test.o shared_lib_open.o print.o -L. -lconnector -ldemo -ldl -o c_linking_test 
	
c_linking_test.o: connector.h linking_test.c shared_lib_open.h print.h
	$(CC) $(CFLAGS) -c linking_test.c -o c_linking_test.o 
	
shared_lib_open.o: shared_lib_open.h shared_lib_open.c
	$(CC) $(CFLAGS) -c shared_lib_open.c -o shared_lib_open.o 
	
print.o: print.h print.c
	$(CC) $(CFLAGS) -c print.c 
	
	

main_cpp: main_cpp.o
	$(CXX) $(CPPFLAGS) main_cpp.o -o main_cpp 
	
linking_test_cpp: linking_test_cpp.o libdemo.so
	$(CXX) $(CPPFLAGS) linking_test_cpp.o shared_lib_open.o -L. -lconnector -ldemo -ldl -o linking_test_cpp 
	
libconnector.so: connector.o libdemo.so
	$(CXX) $(CPPFLAGS) -shared -L. -ldemo -o libconnector.so connector.o 
	
connector.o: connector.h connector.cpp null.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC connector.cpp 
	
main_cpp.o: main.cpp print.hpp
	$(CXX) $(CPPFLAGS) -c main.cpp -o main_cpp.o 

linking_test_cpp.o: linking_test.cpp variadic_template.hpp variadic_template.cpp demo.hpp demo.cpp utility.hpp print.hpp connector.h shared_lib_open.h
	$(CXX) $(CPPFLAGS) -c linking_test.cpp -o linking_test_cpp.o 
	
libdemo.so: demo.o
	$(CXX) $(CPPFLAGS) -shared -o libdemo.so demo.o 

demo.o: demo.hpp demo.cpp null.hpp print.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo.cpp 
	
clean :
	rm $(PROGRAMS) *.o
