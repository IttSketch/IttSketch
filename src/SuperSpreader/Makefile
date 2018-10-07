main:main.o
	g++ -o main main.o
main.o:main.cpp definition.h hash.h data.h abstract.h bitset.h interest.h olf.h tlf.h opensketch.h
	g++ -g -c main.cpp
clean:
	rm -f *.o main