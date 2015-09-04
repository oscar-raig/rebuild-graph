
#include "gslIO.hpp"


void gslIO::printGslMatrix(const gsl_matrix* gslMatrix,const char *format){
    printf("\n");
	printf("hello\n");
    for (size_t i = 0; i < gslMatrix->size1; i++) {
        for (size_t j = 0; j < gslMatrix->size2; j++) {
            printf(format, gsl_matrix_get(gslMatrix, i, j));
        }

        printf("\n");
   }
}

void  gslIO::gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles){
    for (size_t i = 0; i < gslVector->size; i++) {
        arrayDoubles[i]=  gsl_vector_get(gslVector, i);
    }
}

gsl_vector *
 gslIO::getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix){
    
    int nMatrixOrder = (int) gslMatrix->size1;
    gsl_vector * gslvDiagonal = gsl_vector_alloc(nMatrixOrder);
    
    for (int i=0; i < nMatrixOrder;i++){
        gsl_vector_set(gslvDiagonal,i,gsl_matrix_get(gslMatrix,i,i));
    }
    
    return gslvDiagonal;
}

