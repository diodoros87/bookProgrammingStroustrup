CXX = g++
CPPFLAGS = -Wfatal-errors -Wall -Wextra -std=c++14 -fcompare-debug-second
	
integer_test: integer_test.o integer.o
	$(CXX) $(CPPFLAGS) -o integer_test integer_test.o integer.o
    
integer_test.o: integer_test.cpp integer.hpp
	$(CXX) $(CPPFLAGS) -c integer_test.cpp
	
integer.o: integer.cpp integer.hpp
	$(CXX) $(CPPFLAGS) -c integer.cpp

clean :
	rm *.o integer_test
