main:main.o
	g++ -o main main.o
main.o:main.cpp definition.h hash.h data.h abstract.h bitset.h interest.h fr.h fr_cf.h
	g++ -g -c main.cpp
clean:
	rm -f *.o main