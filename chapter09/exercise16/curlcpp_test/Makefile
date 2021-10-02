CXX=g++ 
CPPFLAGS=-I/home/mat/curlcpp/include -I/home/mat/json/include -Wall -std=c++14 -fcompare-debug-second
LPATH=-L/home/mat/curlcpp/build/src
LIBS=-lcurlcpp -lcurl 

all: curl_jsoncpp_example

curl_jsoncpp_example: curl_jsoncpp_example.cpp
	$(CXX) -o $@ $^ $(CPPFLAGS) $(LPATH) $(LIBS)
	
clean :
	rm -f *.o  curl_jsoncpp_example
