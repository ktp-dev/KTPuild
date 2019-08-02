
CXX=g++
CXXFLAGS=-Wall -pedantic -Werror -std=c++1z -g3

EXE=KTPuild

default: KTPuild

SOURCES:=$(wildcard *.cpp)
SOURCES:=$(filter-out $(wildcard dummy*), $(SOURCES))
OBJS=${SOURCES:.cpp=.o}

KTPuild: ${OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $^ -o $@

sync :
	rsync \
    -rtv \
    --delete \
    --exclude '.git*' \
    --filter=':- .gitignore' \
    ./ \
    adravi@sftp.itcs.umich.edu:KTPuild/
