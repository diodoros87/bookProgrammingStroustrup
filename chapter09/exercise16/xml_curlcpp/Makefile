CXX=g++ 

LIBXMLNAME = pugixml
LIBXML = ${LIBXMLNAME}/lib${LIBXMLNAME}.a
#OBJXML = ${LIBXMLNAME}/${LIBXMLNAME}.o
LIBXML_HEADER_DIR = /home/mat/pugixml-1.11/src
CPPFLAGS= -Wall -std=c++14 -fcompare-debug-second
EXEC = curlcpp_test

LPATH=-L/home/mat/curlcpp/build/src -L${LIBXMLNAME}
LIBS=-lcurlcpp -lcurl -l${LIBXMLNAME}

build: directories ${EXEC}.o ${LIBXML}
	${CXX} ${EXEC}.o ${LPATH} ${LIBS}  -o ${EXEC}

directories:
	mkdir -p ${LIBXMLNAME}

${LIBXML}: ${LIBXMLNAME}/${LIBXMLNAME}.o
	ar rcs ${LIBXML} ${LIBXMLNAME}/${LIBXMLNAME}.o

${LIBXMLNAME}/${LIBXMLNAME}.o: ${LIBXML_HEADER_DIR}/pugixml.cpp
	${CXX} -c $(CPPFLAGS) -I${LIBXML_HEADER_DIR}  $< -o $@

%.o: %.cpp 
	${CXX} -c $(CPPFLAGS) -I${LIBXML_HEADER_DIR} -I/home/mat/curlcpp/include  $< -o $@

clean: 
	rm -rf ${LIBXMLNAME} *.o  ${EXEC}

