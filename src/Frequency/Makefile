main:main.o
	g++ -o main main.o
main.o:main.cpp definition.h hash.h data.h abstract.h interest.h cm_heap.h cu_heap.h cf_ss.h streamsummary.h ss.h unbias_ss.h
	g++ -g -c main.cpp
clean:
	rm -f *.o main