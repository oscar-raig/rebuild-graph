//
//  graphIndicatorCommunicabilityBetweennessCentrality.h
//  TestClassgslGraph
//
//  Created by Oscar Raig Colon on 25/1/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#ifndef __TestClassgslGraph__graphIndicatorCommunicabilityBetweennessCentrality__
#define __TestClassgslGraph__graphIndicatorCommunicabilityBetweennessCentrality__

#include <stdio.h>
#include <stdio.h>
#include "graphIndicator.h"

class graphIndicatorCommunicabilityBetweennessCentrality : public graphIndicator{
	
public:
	graphIndicatorCommunicabilityBetweennessCentrality( gslGraph *argGraph ):graphIndicator( argGraph ){};
	
	gsl_matrix *
	gslCopyGraph(const gsl_matrix* target){
		
		gsl_matrix *dest=gsl_matrix_calloc(target->size1,target->size1);
		
		gsl_matrix_memcpy (dest, target);
		return dest;
	}
	void gslDeleteNodeConnections( gsl_matrix* target, int i){
		for (int iterator = 0; iterator < target->size1; iterator++){
			gsl_matrix_set(target,i,iterator,0);
			gsl_matrix_set(target,iterator,i,0);
		}
	};


		
		
	void
	communicability_betweenness_centrality(double *myCExp)
	{
		CFuncTrace lFuncTrace(false,"communicability_betweenness_centrality");
		/* Step 1
		 nodelist = G.nodes() # ordering of nodes in matrix
		 n = len(nodelist)
		 A = nx.to_numpy_matrix(G,nodelist)
		 # convert to 0-1 matrix
		 A[A!=0.0] = 1
		 */
		int graphOrder=getGraph()->getOrder();
		
		
		gsl_vector * matrixFinalResult = gsl_vector_calloc(graphOrder);
		
		
		// Get Numpy Matrix // Matriu d'adjacencia
		gsl_matrix *A1=gsl_matrix_calloc(graphOrder,graphOrder);
		
		//	targetGraph->printGraph();
		
		getGraph()->graphToGsl(A1);
		lFuncTrace.trace(CTrace::TRACE_DEBUG,"\nPrinting Home made Matrix\n");
		//		printGslMatrix(A1," %g");
	 
	 /* Step 2
	  expA = scipy.linalg.expm(A)
	  mapping = dict(zip(nodelist,range(n)))
	  sc = {}
	  */
		gsl_matrix *A1expm=gsl_matrix_calloc(graphOrder,graphOrder);
		
		gsl_linalg_exponential_ss(A1, A1expm, 0x00);
		//	lFuncTrace.trace("Printing ExpmMatrix");
		//		printGslMatrix(A1expm);
		
		for ( int iteratorNode = 0; iteratorNode < graphOrder; iteratorNode++){
			gsl_matrix *copyA1 = gslCopyGraph(A1);
			gslDeleteNodeConnections(copyA1,iteratorNode);
			//		printGslMatrix(copyA1);
			/*
			 B = (expA - scipy.linalg.expm(A)) / expA
			 */
			gsl_matrix *copyA1expm=gsl_matrix_calloc(graphOrder,graphOrder);
			gsl_linalg_exponential_ss(copyA1, copyA1expm, 0x00);
			gsl_matrix_free(copyA1);		
	
			gsl_matrix *copyexpmAForSubstract  = gslCopyGraph(A1expm);
			
			gsl_matrix_sub(copyexpmAForSubstract,copyA1expm);
			gsl_matrix_free(copyA1expm);
			gsl_matrix_div_elements (copyexpmAForSubstract, A1expm);
			lFuncTrace.trace(CTrace::TRACE_DEBUG,"Printing expA- scip\n");
			//		printGslMatrix(copyexpmAForSubstract);
			gslDeleteNodeConnections(copyexpmAForSubstract,iteratorNode);
			
			gsl_matrix *copyB = gslCopyGraph(copyexpmAForSubstract);
			
			for ( int col =0; col < graphOrder; col++){
				for ( int row = 0; row< graphOrder ; row ++){
					if ( row != col)
						gsl_matrix_set(copyB,row,col,0);
				}
			}
			gsl_matrix_sub(copyexpmAForSubstract,copyB);

			//		printGslMatrix(copyexpmAForSubstract);
			double sum = 0;
			for ( int col =0; col < graphOrder; col++){
				for ( int row = 0; row< graphOrder ; row ++){
					
					sum +=gsl_matrix_get(copyexpmAForSubstract,row,col);
				}
			}
			lFuncTrace.trace(CTrace::TRACE_DEBUG,"Suma %f\n",sum);
			gsl_vector_set(matrixFinalResult,iteratorNode,sum);
			gsl_matrix_free(copyB); 
			gsl_matrix_free(copyexpmAForSubstract);			
		}
		gsl_matrix_free(A1expm);
		gsl_matrix_free(A1);
		this->getGraph()->gslVectorToArray(matrixFinalResult,myCExp);
		gsl_vector_free(matrixFinalResult);
	}

	double * calculateIndicator( ){
		double *result = new double[this->getGraph()->getOrder()];
		communicability_betweenness_centrality(result);
		return result;
	}

};
#endif /* defined(__TestClassgslGraph__graphIndicatorCommunicabilityBetweennessCentrality__) */
