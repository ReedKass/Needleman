CXX      = clang++
CXXFLAGS = -std=c++11 -g3 -Wall -Wextra
LDFLAGS  = -g3
all: align

align: align.o 
	${CXX} ${LDFLAGS} -o align main.o 

align.o:	align.cpp  

clean:
	rm -rf hw4 *.o *.dSYM
