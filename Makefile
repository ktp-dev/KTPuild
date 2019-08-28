
CXX=g++
CXXFLAGS=-Wall -pedantic -Werror -std=c++1z -g3

EXE=KTPuild

default: KTPuild

SOURCES:=$(wildcard *.cpp)
OBJS=${SOURCES:.cpp=.o}

KTPuild: ${OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@
	rm *.o

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $^ -o $@
