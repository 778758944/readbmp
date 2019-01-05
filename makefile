readbitmap.o: readbitmap.c
	cc -c readbitmap.c -o readbitmap.o

libreadbitmap.a: readbitmap.o
	ar -crv libreadbitmap.a readbitmap.o