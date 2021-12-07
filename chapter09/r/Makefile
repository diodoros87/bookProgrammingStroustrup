CXX = g++
CPPFLAGS = -DMANUAL_DLL_LOAD                      -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 
TEST_CPPFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 

CC = gcc
CFLAGS = -DMANUAL_DLL_LOAD                      -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
TEST_CFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
#  -fcompare-debug-second -DMANUAL_DLL_LOAD

LDFLAGS = #-fsanitize=address -static-libasan 

PROGRAMS  = libdemo.so libdemoderived.so libconnector.so main_cpp test_cpp libvariadic_template.so     libhuman.so c_main linking_test_cpp c_linking_test c_test 

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

c_linking_test: c_linking_test.o libconnector.so shared_lib_open.o print.o c_string.o demo_functions.o human_functions.o utility.o libhuman.so libdemo.so c_variadic_template_test.o
	$(CC) $(CFLAGS) $(LDFLAGS) c_linking_test.o shared_lib_open.o print.o c_string.o demo_functions.o human_functions.o utility.o c_variadic_template_test.o -L. -lconnector -ldemo -lhuman -Wl,--no-as-needed -lvariadic_template -Wl,--as-needed -ldl -o c_linking_test 
	
c_linking_test.o: connector.h linking_test.c shared_lib_open.h print.h c_string.h human.h utility.h demo_functions.h human_functions.h variadic_template_test.h
	$(CC) $(CFLAGS) -c linking_test.c -o c_linking_test.o 
	
demo_functions.o: demo_functions.h demo_functions.c utility.h shared_lib_open.h result_codes.h
	$(CC) $(CFLAGS) -c demo_functions.c 
	
human_functions.o: human_functions.h human_functions.c utility.h shared_lib_open.h result_codes.h
	$(CC) $(CFLAGS) -c human_functions.c 
	
c_variadic_template_test.o: variadic_template_test.h variadic_template_test.c shared_lib_open.h c_string.h result_codes.h
	$(CC) $(CFLAGS) -c variadic_template_test.c -o c_variadic_template_test.o 
	
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
	
linking_test_cpp: linking_test_cpp.o variadic_template_test_cpp.o demo_test.o demo_derived_test.o human_test.o shared_lib_open.o print.o libhuman.so libdemo.so libdemoderived.so
	$(CXX) $(CPPFLAGS) $(LDFLAGS) linking_test_cpp.o variadic_template_test_cpp.o demo_test.o demo_derived_test.o human_test.o shared_lib_open.o -L. -lconnector -Wl,--no-as-needed -lvariadic_template -Wl,--as-needed -ldemo -ldemoderived -lhuman -ldl -o linking_test_cpp 
	
demo_test.o: demo_test.hpp demo_test.cpp utility.hpp shared_lib_open.h result_codes.h connector.h
	$(CXX) $(CPPFLAGS) -c demo_test.cpp 
	
demo_derived_test.o: demo_derived_test.hpp demo_derived_test.cpp utility.hpp shared_lib_open.h result_codes.h connector.h
	$(CXX) $(CPPFLAGS) -c demo_derived_test.cpp -o demo_derived_test.o 
	
human_test.o: human_test.hpp human_test.cpp utility.hpp shared_lib_open.h result_codes.h
	$(CXX) $(CPPFLAGS) -c human_test.cpp 
	
variadic_template_test_cpp.o: variadic_template_test.hpp variadic_template_test.cpp shared_lib_open.h result_codes.h
	$(CXX) $(CPPFLAGS) -c variadic_template_test.cpp -o variadic_template_test_cpp.o 
	
libconnector.so: connector.o libdemo.so libvariadic_template.so
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -L. -ldemo -lvariadic_template -o libconnector.so connector.o 
	
connector.o: connector.h connector.cpp null.hpp variadic_template.hpp variadic_template.cpp
	$(CXX) $(CPPFLAGS) -c -fPIC connector.cpp 
	
main_cpp.o: main.cpp print.hpp file_edit.hpp system.hpp result_codes.h
	$(CXX) $(CPPFLAGS) -c main.cpp -o main_cpp.o 

linking_test_cpp.o: linking_test.cpp variadic_template.hpp variadic_template.cpp demo.hpp demo.cpp utility.hpp print.hpp connector.h shared_lib_open.h human.h
	$(CXX) $(CPPFLAGS) -c linking_test.cpp -o linking_test_cpp.o 
	
system_cpp.o: system.cpp system.hpp
	$(CXX) $(CPPFLAGS) -c system.cpp -o system_cpp.o 
	
file_edit.o: file_edit.hpp file_edit.cpp
	$(CXX) $(CPPFLAGS) -c file_edit.cpp 
	
libvariadic_template.so: variadic_template_instances.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libvariadic_template.so variadic_template_instances.o 

variadic_template_instances.o: variadic_template.hpp variadic_template.cpp variadic_template_instances.cpp
	$(CXX) $(CPPFLAGS) -c -fPIC variadic_template_instances.cpp -o variadic_template_instances.o 
	
libdemoderived.so: demoderived.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libdemoderived.so demoderived.o 

demoderived.o: demo_derived.hpp demo_derived.cpp null.hpp print.hpp utility.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo_derived.cpp -o demoderived.o 
	
libdemo.so: demo.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libdemo.so demo.o 

demo.o: demo.hpp demo.cpp null.hpp print.hpp utility.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo.cpp 
	
clean :
	rm $(PROGRAMS) *.o
