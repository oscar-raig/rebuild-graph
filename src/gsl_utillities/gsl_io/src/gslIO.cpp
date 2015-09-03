
#include "gslIO.hpp"


int gslIO::printGslMatrix(const gsl_matrix* gslMatrix,const char *format){
    printf("\n");
	printf("hello\n");
    for (size_t i = 0; i < gslMatrix->size1; i++) {
        for (size_t j = 0; j < gslMatrix->size2; j++) {
            printf(format, gsl_matrix_get(gslMatrix, i, j));
        }

        printf("\n");
   }
	return RESULT_OK;
}