#ifndef __GSL_IO__
#define __GSL_IO__

#include <gsl/gsl_matrix.h>

class gslIO {
	
public:	
	static void printGslMatrix(const gsl_matrix* gslMatrix,const char *format);
	static void gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles);
	static gsl_vector * getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix);
	

};

#endif