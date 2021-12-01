CXX = g++
CPPFLAGS =  -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 
TEST_CPPFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 

CC = gcc
CFLAGS =  -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
TEST_CFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
#  -fcompare-debug-second -DMANUAL_DLL_LOAD

LDFLAGS = #-fsanitize=address -static-libasan 

PROGRAMS  = libdemo.so libconnector.so main_cpp test_cpp      libhuman.so c_main linking_test_cpp c_linking_test c_test 

.PHONY: clean all

all: $(PROGRAMS)

c_test: c_test.o system.o print.o c_string.o regular_expr.o
	$(CC) $(TEST_CFLAGS) $(LDFLAGS) c_test.o system.o print.o c_string.o regular_expr.o -o c_test 
	
c_test.o: test.c print.h system.h result_codes.h c_string.h
	$(CC) $(TEST_CFLAGS) -c test.c -o c_test.o 

c_main: c_main.o print.o file_modify.o c_string.o system.o
	$(CC) $(CFLAGS) $(LDFLAGS) c_main.o print.o file_modify.o c_string.o system.o -o c_main 
	
c_main.o: main.c print.h system.h file_modify.h result_codes.h c_string.h
	$(CC) $(CFLAGS) -c main.c -o c_main.o 

c_linking_test: c_linking_test.o libconnector.so shared_lib_open.o print.o c_string.o demo_functions.o human_functions.o utility.o libhuman.so libdemo.so variadic_template_test.o
	$(CC) $(CFLAGS) $(LDFLAGS) c_linking_test.o shared_lib_open.o print.o c_string.o demo_functions.o human_functions.o utility.o variadic_template_test.o -L. -lconnector -ldemo -lhuman -ldl -o c_linking_test 
	
c_linking_test.o: connector.h linking_test.c shared_lib_open.h print.h c_string.h human.h utility.h demo_functions.h human_functions.h
	$(CC) $(CFLAGS) -c linking_test.c -o c_linking_test.o 
	
demo_functions.o: demo_functions.h demo_functions.c utility.h shared_lib_open.h
	$(CC) $(CFLAGS) -c demo_functions.c 
	
human_functions.o: human_functions.h human_functions.c utility.h shared_lib_open.h
	$(CC) $(CFLAGS) -c human_functions.c 
	
variadic_template_test.o: variadic_template_test.h variadic_template_test.c shared_lib_open.h c_string.h
	$(CC) $(CFLAGS) -c variadic_template_test.c 
	
utility.o: utility.h utility.c
	$(CC) $(CFLAGS) -c utility.c 
	
system.o: system.c system.h print.h result_codes.h
	$(CC) $(CFLAGS) -c system.c 
	
file_modify.o: file_modify.h file_modify.c print.h c_string.h result_codes.h
	$(CC) $(CFLAGS) -c file_modify.c 
	
shared_lib_open.o: shared_lib_open.h shared_lib_open.c
	$(CC) $(CFLAGS) -c shared_lib_open.c -o shared_lib_open.o 
	
print.o: print.h print.c
	$(CC) $(CFLAGS) -c -fPIC print.c 
	
c_string.o: c_string.h c_string.c
	$(CC) $(CFLAGS) -c -fPIC c_string.c 
	
libhuman.so: human.o print.o c_string.o regular_expr.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o libhuman.so human.o print.o c_string.o regular_expr.o 
	
human.o: human.h human.c regular_expr.h print.h result_codes.h singleton.h
	$(CC) $(CFLAGS) -c -fPIC human.c 
	
regular_expr.o: regular_expr.h regular_expr.c print.h result_codes.h
	$(CC) $(CFLAGS) -c -fPIC regular_expr.c 
	
	
test_cpp: test_cpp.o system_cpp.o
	$(CXX) $(TEST_CPPFLAGS) $(LDFLAGS) test_cpp.o system_cpp.o -o test_cpp 
	
test_cpp.o: test.cpp print.hpp system.hpp result_codes.h
	$(CXX) $(TEST_CPPFLAGS) -c test.cpp -o test_cpp.o 

main_cpp: main_cpp.o file_edit.o system_cpp.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) main_cpp.o file_edit.o system_cpp.o -o main_cpp 
	
linking_test_cpp: linking_test_cpp.o libdemo.so shared_lib_open.o print.o libhuman.so
	$(CXX) $(CPPFLAGS) $(LDFLAGS) linking_test_cpp.o shared_lib_open.o -L. -lconnector -ldemo -lhuman -ldl -o linking_test_cpp 
	
libconnector.so: connector.o libdemo.so
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -L. -ldemo -o libconnector.so connector.o 
	
connector.o: connector.h connector.cpp null.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC connector.cpp 
	
main_cpp.o: main.cpp print.hpp file_edit.hpp system.hpp result_codes.h
	$(CXX) $(CPPFLAGS) -c main.cpp -o main_cpp.o 

linking_test_cpp.o: linking_test.cpp variadic_template.hpp variadic_template.cpp demo.hpp demo.cpp utility.hpp print.hpp connector.h shared_lib_open.h human.h
	$(CXX) $(CPPFLAGS) -c linking_test.cpp -o linking_test_cpp.o 
	
system_cpp.o: system.cpp system.hpp
	$(CXX) $(CPPFLAGS) -c system.cpp -o system_cpp.o 
	
file_edit.o: file_edit.hpp file_edit.cpp
	$(CXX) $(CPPFLAGS) -c file_edit.cpp 
	
libdemo.so: demo.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libdemo.so demo.o 

demo.o: demo.hpp demo.cpp null.hpp print.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo.cpp 
	
clean :
	rm $(PROGRAMS) *.o
