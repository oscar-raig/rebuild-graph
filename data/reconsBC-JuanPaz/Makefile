

CFLAGS=-I/Users/oscarraigcolon/Downloads/gsl-1.16
LDFLAGS=-l gsl  -l gslcblas -stdlib=libstdc++
compara.o:compara.c
	cc compara.c ${CFLAGS} -c 

compara: compara.o
	cc compara.o ${LDFLAGS} -o compara
