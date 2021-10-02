CXX=g++ 
CPPFLAGS=-I/home/mat/asio-1.18.2/include  -I/home/mat/json/include -I/home/mat/json/include/nlohmann -Wall -std=c++14 -fcompare-debug-second
LIBS=-lpthread 

all: asio_test

asio_test: asio_test.cpp
	$(CXX) -o $@ $^ $(CPPFLAGS)  $(LIBS)
	
clean :
	rm -f *.o asio_test
