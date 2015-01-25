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

class graphIndicatorCommunicabilityCentralityUsingMatrixExponential : public graphIndicator{
	
public:
	graphIndicatorCommunicabilityCentralityUsingMatrixExponential( gslGraph *argGraph):graphIndicator(argGraph){};
	
	gsl_vector *
	getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix){
	 
		 int nMatrixOrder = (int) gslMatrix->size1;
		 gsl_vector * gslvDiagonal = gsl_vector_alloc(nMatrixOrder);
		 
		 for (int i=0; i < nMatrixOrder;i++){
			 gsl_vector_set(gslvDiagonal,i,gsl_matrix_get(gslMatrix,i,i));
		 }
		 
		 return gslvDiagonal;
	}
	
	void brandes_comunicability_centrality_exp(double *myCExp)
	{
		
		int graphOrder=this->getGraph()->getOrder();
		// Get Numpy Matrix // Matriu d'adjacencia
		//	lFuncTrace.trace("\nPrinting Home made Matrix\n");
		//	printGslMatrix(A1," %g");
		gsl_matrix *A1expm=gsl_matrix_alloc(graphOrder,graphOrder);
		
		gsl_matrix * matrix = gsl_matrix_alloc( graphOrder,graphOrder);
		this->getGraph()->graphToGsl(matrix);
		
		
		gsl_linalg_exponential_ss(matrix
								  , A1expm, .01);
		//	lFuncTrace.trace("Printing ExpmMatrix");
		//	printGslMatrix(A1expm);
		
		gsl_vector * gslvDiagonal = getDiagonalFromGslMatrix(A1expm);
		
		//	lFuncTrace.trace("Printing Diagonal From ExpmMatrix");
		//	printGslVector(gslvDiagonal);
		
		gslGraph::gslVectorToArray(gslvDiagonal,myCExp);
		
	};
	double * calculateIndicator( ){
		double *result = new double[this->getGraph()->getOrder()];
		brandes_comunicability_centrality_exp(result);
		return result;
	}
};

#endif /* defined(__rebuild_graph__graphIndicatorCommunicabilityCentralityUsingMatrixExponential__) */
