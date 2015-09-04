//
//  graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 24/12/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__graphIndicatorCommunicabilityCentralityUsingMatrixExponential__
#define __rebuild_graph__graphIndicatorCommunicabilityCentralityUsingMatrixExponential__

#include <stdio.h>
#include "graphIndicator.h"
#include "gslIO.hpp"

class graphIndicatorCommunicabilityCentralityUsingMatrixExponential : public graphIndicator{
	
public:
	graphIndicatorCommunicabilityCentralityUsingMatrixExponential( gslGraph *argGraph):graphIndicator(argGraph){};
	
	
	void brandes_comunicability_centrality_exp(double *myCExp)
	{
		
		int graphOrder=this->getGraph()->getOrder();
		gsl_matrix *A1expm=gsl_matrix_calloc(graphOrder,graphOrder);
		
		gsl_matrix * matrix = gsl_matrix_calloc( graphOrder,graphOrder);
		this->getGraph()->graphToGsl(matrix);
		
		
		gsl_linalg_exponential_ss(matrix
								  , A1expm, 0x00);

		gsl_vector * gslvDiagonal = gslIO::getDiagonalFromGslMatrix(A1expm);
		
				
		
		gslIO::gslVectorToArray(gslvDiagonal,myCExp);
		gsl_matrix_free(matrix);
		gsl_matrix_free(A1expm);
		gsl_vector_free(gslvDiagonal);
		
	};
private:
	double * calculateIndicator( ){
		double *result = new double[this->getGraph()->getOrder()];
		brandes_comunicability_centrality_exp(result);
		return result;
	}
};

#endif /* defined(__rebuild_graph__graphIndicatorCommunicabilityCentralityUsingMatrixExponential__) */
