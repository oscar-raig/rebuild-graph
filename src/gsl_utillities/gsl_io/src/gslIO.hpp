#include "gslGraph.hpp"
#include <gsl/gsl_matrix.h>

class gslIO {
	
public:	
	static int printGslMatrix(const gsl_matrix* gslMatrix,const char *format);
};