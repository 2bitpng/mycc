CFLAGS=-std=c++20 -g -static
CC=g++
mycc: mycc.cpp

test: mycc
	./test.sh

clean:
	rm -f mycc *.o *~ tmp*

.PHONY: test clean
