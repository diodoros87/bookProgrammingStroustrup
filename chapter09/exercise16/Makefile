CXX = g++
CPPFLAGS =  -Wall -Wextra -std=c++14 -fcompare-debug-second 
	
integer_test: integer_test.o integer.o integer_parsing.o
	$(CXX) $(CPPFLAGS) -o integer_test integer_test.o integer.o integer_parsing.o
    
integer_test.o: integer_test.cpp integer.hpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer_test.cpp
	
integer.o: integer.cpp integer.hpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer.cpp
	
integer_parsing.o: integer_parsing.cpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer_parsing.cpp

clean :
	rm *.o integer_test
