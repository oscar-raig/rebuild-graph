#include <iostream>
#include "gtest/gtest.h"
#include "gslIO.hpp"



gsl_matrix* getExampleMatrixForDiagonalTest() {
	gsl_matrix* gslForTest = gsl_matrix_calloc (5,5);
	gsl_matrix_set (gslForTest,0,0,9);
	gsl_matrix_set (gslForTest,1,1,8);
	gsl_matrix_set (gslForTest,2,2,7);
	gsl_matrix_set (gslForTest,3,3,9);
	gsl_matrix_set (gslForTest,4,4,3);
	return gslForTest;
	
}

TEST(test_gslIO_, getDiagonalFromGslMatrix_should_return_vector) {
	
   	gsl_matrix* matrixTest = getExampleMatrixForDiagonalTest();
	
	gsl_vector * vectorTest = gslIO::getDiagonalFromGslMatrix(matrixTest);  
	
	EXPECT_TRUE(9 == gsl_vector_get(vectorTest,0));
	gsl_matrix_free (matrixTest);	
	
}


TEST(test_gslIO_, gslVectorToArray_should_return_array) {
	
	gsl_vector* vecctorTest = gsl_vector_calloc(5);
	gsl_vector_set (vecctorTest, 0, 9);
	gsl_vector_set (vecctorTest, 1, 8);
	gsl_vector_set (vecctorTest, 2, 7);
	gsl_vector_set (vecctorTest, 3, 9);
	gsl_vector_set (vecctorTest, 4, 3);
	
	double* arrayOfValues = (double *)malloc(vecctorTest->size * sizeof(double));
	
	gslIO::gslVectorToArray(vecctorTest, arrayOfValues);
	
	EXPECT_TRUE(arrayOfValues[0] == 9);
	EXPECT_TRUE(arrayOfValues[4] == 3);
	
	free(arrayOfValues);
	gsl_vector_free(vecctorTest);
}


int main(int argc, char**argv) {
    testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}
