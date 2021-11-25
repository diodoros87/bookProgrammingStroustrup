CXX = g++
CPPFLAGS = -DMANUAL_DLL_LOAD      -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 
TEST_CPPFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 

CC = gcc
CFLAGS = -DMANUAL_DLL_LOAD      -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
TEST_CFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
#  -fcompare-debug-second -DMANUAL_DLL_LOAD

PROGRAMS  = libdemo.so libconnector.so main_cpp test_cpp       c_main c_linking_test linking_test_cpp c_test

.PHONY: clean all

all: $(PROGRAMS)

c_test: c_test.o system.o print.o c_string.o
	$(CC) $(TEST_CFLAGS) c_test.o system.o print.o c_string.o -o c_test 
	
c_test.o: test.c print.h system.h result_codes.h c_string.h
	$(CC) $(TEST_CFLAGS) -c test.c -o c_test.o 

c_main: c_main.o print.o file_modify.o c_string.o system.o
	$(CC) $(CFLAGS) c_main.o print.o file_modify.o c_string.o system.o -o c_main 
	
c_main.o: main.c print.h system.h file_modify.h result_codes.h
	$(CC) $(CFLAGS) -c main.c -o c_main.o 

c_linking_test: c_linking_test.o libconnector.so shared_lib_open.o print.o c_string.o
	$(CC) $(CFLAGS) c_linking_test.o shared_lib_open.o print.o c_string.o -L. -lconnector -ldemo -ldl -o c_linking_test 
	
c_linking_test.o: connector.h linking_test.c shared_lib_open.h print.h c_string.h
	$(CC) $(CFLAGS) -c linking_test.c -o c_linking_test.o 
	
system.o: system.c system.h print.h result_codes.h
	$(CC) $(CFLAGS) -c system.c 
	
file_modify.o: file_modify.h file_modify.c print.h c_string.h result_codes.h
	$(CC) $(CFLAGS) -c file_modify.c 
	
shared_lib_open.o: shared_lib_open.h shared_lib_open.c
	$(CC) $(CFLAGS) -c shared_lib_open.c -o shared_lib_open.o 
	
print.o: print.h print.c
	$(CC) $(CFLAGS) -c print.c 
	
c_string.o: c_string.h c_string.c
	$(CC) $(CFLAGS) -c c_string.c 
	
	
test_cpp: test_cpp.o system_cpp.o
	$(CXX) $(TEST_CPPFLAGS) test_cpp.o system_cpp.o -o test_cpp 
	
test_cpp.o: test.cpp print.hpp system.hpp result_codes.h
	$(CXX) $(TEST_CPPFLAGS) -c test.cpp -o test_cpp.o 

main_cpp: main_cpp.o file_edit.o system_cpp.o
	$(CXX) $(CPPFLAGS) main_cpp.o file_edit.o system_cpp.o -o main_cpp 
	
linking_test_cpp: linking_test_cpp.o libdemo.so
	$(CXX) $(CPPFLAGS) linking_test_cpp.o shared_lib_open.o print.o -L. -lconnector -ldemo -ldl -o linking_test_cpp 
	
libconnector.so: connector.o libdemo.so
	$(CXX) $(CPPFLAGS) -shared -L. -ldemo -o libconnector.so connector.o 
	
connector.o: connector.h connector.cpp null.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC connector.cpp 
	
main_cpp.o: main.cpp print.hpp file_edit.hpp system.hpp result_codes.h
	$(CXX) $(CPPFLAGS) -c main.cpp -o main_cpp.o 

linking_test_cpp.o: linking_test.cpp variadic_template.hpp variadic_template.cpp demo.hpp demo.cpp utility.hpp print.hpp connector.h shared_lib_open.h
	$(CXX) $(CPPFLAGS) -c linking_test.cpp -o linking_test_cpp.o 
	
system_cpp.o: system.cpp system.hpp
	$(CXX) $(CPPFLAGS) -c system.cpp -o system_cpp.o 
	
file_edit.o: file_edit.hpp file_edit.cpp
	$(CXX) $(CPPFLAGS) -c file_edit.cpp 
	
libdemo.so: demo.o
	$(CXX) $(CPPFLAGS) -shared -o libdemo.so demo.o 

demo.o: demo.hpp demo.cpp null.hpp print.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo.cpp 
	
clean :
	rm $(PROGRAMS) *.o
