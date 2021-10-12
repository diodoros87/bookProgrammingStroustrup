CXX = g++
CPPFLAGS = -Wfatal-errors -Wall -Wextra -std=c++14 -fcompare-debug-second 
	
integer_test: integer_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o integer_test integer_test.o integer.o integer_parsing.o parsing_test.o
    
integer_test.o: integer_test.cpp integer.hpp integer_parsing.hpp assertions.hpp
	$(CXX) $(CPPFLAGS) -c integer_test.cpp
	
integer.o: integer.cpp integer.hpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer.cpp
	
integer_parsing.o: integer_parsing.cpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer_parsing.cpp
	
parsing_test.o: parsing_test.cpp parsing_test.hpp
	$(CXX) $(CPPFLAGS) -c parsing_test.cpp

clean :
	rm *.o integer_test
