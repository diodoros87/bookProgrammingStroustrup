CXX = g++
CPPFLAGS = -O0 -Wfatal-errors -Wall -Wextra  -std=c++14 -I/home/mat/asio-1.18.2/include  -I/home/mat/json/include #-Wconversion
#CXX = clang++
#  -fcompare-debug-second -Wconversion
GCC_CPPFLAGS = -DMESSAGE='"Compiled with GCC"'
CLANG_CPPFLAGS = -DMESSAGE='"Compiled with Clang"'

ifeq ($(CXX), g++)
	CPPFLAGS += $(GCC_CPPFLAGS)
	CURL_TEST = curlcpp_test
else ifeq ($(CXX), clang++)
	CPPFLAGS += $(CLANG_CPPFLAGS)
	CURL_TEST = curl_test
endif

PROGRAMS  = integer_test fundamental_test arithmetic_test random_tests money_test asio_test $(CURL_TEST)
#PROGRAMS  = 

.PHONY: clean all

all: $(PROGRAMS)

asio_test: asio_test.cpp
	$(CXX) -o $@ $^ -I/home/mat/asio-1.18.2/include  -I/home/mat/json/include $(CPPFLAGS) -lpthread
	
LIBXMLNAME = pugixml
LIBXML = ${LIBXMLNAME}/lib${LIBXMLNAME}.a
LIBXML_HEADER_DIR = /home/mat/pugixml-1.11/src
IPATH= -I${LIBXML_HEADER_DIR}
LPATH= -L${LIBXMLNAME}
LIBS=  -l${LIBXMLNAME} -lcurl

ifeq ($(CXX), g++)
	IPATH+=-I/home/mat/curlcpp/include
	LPATH+=-L/home/mat/curlcpp/build/src
	LIBS +=-lcurlcpp
else ifeq ($(CXX), clang++)

endif

$(CURL_TEST): directories ${CURL_TEST}.o ${LIBXML}
	${CXX} $(CPPFLAGS) ${CURL_TEST}.o ${LPATH} ${LIBS}  -o ${CURL_TEST}

${CURL_TEST}.o: ${CURL_TEST}.cpp 
	${CXX} -c $(CPPFLAGS) ${IPATH}  $< -o $@

directories:
	mkdir -p ${LIBXMLNAME}

${LIBXML}: ${LIBXMLNAME}/${LIBXMLNAME}.o
	ar rcs ${LIBXML} ${LIBXMLNAME}/${LIBXMLNAME}.o

${LIBXMLNAME}/${LIBXMLNAME}.o: ${LIBXML_HEADER_DIR}/${LIBXMLNAME}.cpp
	${CXX} -c $(CPPFLAGS) -I${LIBXML_HEADER_DIR}  $< -o $@




money_test: money_test.o integer.o integer_parsing.o money_init_test.o money.o money_operations_test.o floatrates.o network.o json_downloader.o asio_downloader.o money_currency_test.o json_downloader_test.o
	$(CXX) $(CPPFLAGS) -lpthread -o money_test money_test.o integer.o integer_parsing.o money_init_test.o money.o money_operations_test.o floatrates.o network.o json_downloader.o asio_downloader.o money_currency_test.o json_downloader_test.o

fundamental_test: fundamental_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o fundamental_test fundamental_test.o integer.o integer_parsing.o parsing_test.o
	
arithmetic_test: arithmetic_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o arithmetic_test arithmetic_test.o integer.o integer_parsing.o parsing_test.o
	
random_tests: random_tests.o integer.o integer_parsing.o
	$(CXX) $(CPPFLAGS) -o random_tests random_tests.o integer.o integer_parsing.o
	
integer_test: integer_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o integer_test integer_test.o integer.o integer_parsing.o parsing_test.o
	
json_downloader_test.o: json_downloader_test.cpp json_downloader.hpp json_downloader.cpp network.hpp network.cpp asio_downloader.cpp asio_downloader.hpp
	$(CXX) $(CPPFLAGS) -c json_downloader_test.cpp
	

money_test.o: money_test.cpp money.cpp money_init_test.hpp money_init_test.cpp integers_extremums_tests.hpp money_utility_test.hpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_operations_test.hpp money_operations_test.cpp money_currency_test.hpp money_currency_test.cpp
	$(CXX) $(CPPFLAGS) -c money_test.cpp
	
money_currency_test.o: money_currency_test.cpp money_currency_test.hpp money_init_test.cpp money_init_test.hpp money.cpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_utility_test.hpp money_utility.cpp
	$(CXX) $(CPPFLAGS) -c money_currency_test.cpp
	
money_operations_test.o: money_operations_test.cpp money_operations_test.hpp money_init_test.cpp money_init_test.hpp money.cpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_utility_test.hpp money_utility.cpp
	$(CXX) $(CPPFLAGS) -c money_operations_test.cpp
	
money_init_test.o: money_init_test.cpp money_init_test.hpp money.cpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_utility_test.hpp
	$(CXX) $(CPPFLAGS) -c money_init_test.cpp
	
money.o: money.cpp money.hpp money_utility.cpp floatrates.hpp network.hpp floatrates.cpp asio_downloader.cpp asio_downloader.hpp json_downloader.hpp json_downloader.cpp
	$(CXX) $(CPPFLAGS) -c money_utility.cpp -o money.o
	
asio_downloader.o: asio_downloader.hpp asio_downloader.cpp network.hpp network.cpp
	$(CXX) $(CPPFLAGS) -c asio_downloader.cpp
	
json_downloader.o: json_downloader.hpp json_downloader.cpp network.hpp network.cpp asio_downloader.cpp asio_downloader.hpp
	$(CXX) $(CPPFLAGS) -c json_downloader.cpp
	
floatrates.o: floatrates.hpp floatrates.cpp
	$(CXX) $(CPPFLAGS) -c floatrates.cpp
	
network.o: network.hpp network.cpp
	$(CXX) $(CPPFLAGS) -c network.cpp
	
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
	rm -rf ${LIBXMLNAME} $(PROGRAMS) *.o
