CXX = g++
CPPFLAGS = -fcompare-debug-second -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 
# -Wfatal-errors -fcompare-debug-second
PROGRAMS  = integer_test fundamental_test arithmetic_test random_tests money_test
#PROGRAMS  =  fundamental_test

.PHONY: clean all

all: $(PROGRAMS)

money_test: money_test.o integer.o integer_parsing.o 
	$(CXX) $(CPPFLAGS) -o money_test money_test.o integer.o integer_parsing.o

fundamental_test: fundamental_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o fundamental_test fundamental_test.o integer.o integer_parsing.o parsing_test.o
	
arithmetic_test: arithmetic_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o arithmetic_test arithmetic_test.o integer.o integer_parsing.o parsing_test.o
	
random_tests: random_tests.o integer.o integer_parsing.o
	$(CXX) $(CPPFLAGS) -o random_tests random_tests.o integer.o integer_parsing.o
	
integer_test: integer_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o integer_test integer_test.o integer.o integer_parsing.o parsing_test.o
	

money_test.o: money_test.cpp money.cpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp
	$(CXX) $(CPPFLAGS) -c money_test.cpp
	
fundamental_test.o: fundamental_test.cpp integer.hpp integer_parsing.hpp assertions.hpp parsing_test.hpp
	$(CXX) $(CPPFLAGS) -c fundamental_test.cpp
	
arithmetic_test.o: arithmetic_test.cpp integer.hpp integer_parsing.hpp parsing_test.hpp
	$(CXX) $(CPPFLAGS) -c arithmetic_test.cpp
	
random_tests.o: random_tests.cpp integer.hpp random_numbers.hpp integer_parsing.hpp assertions.hpp
	$(CXX) $(CPPFLAGS) -c random_tests.cpp
    
integer_test.o: integer_test.cpp integer.hpp integer_parsing.hpp assertions.hpp
	$(CXX) $(CPPFLAGS) -c integer_test.cpp
	
integer.o: integer.cpp integer.hpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer.cpp
	
integer_parsing.o: integer_parsing.cpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer_parsing.cpp
	
parsing_test.o: parsing_test.cpp parsing_test.hpp
	$(CXX) $(CPPFLAGS) -c parsing_test.cpp
	
clean :
	rm $(PROGRAMS) *.o
