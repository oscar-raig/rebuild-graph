//
//  graphIndicatorBetweennessCentrality.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/12/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__graphIndicatorBetweennessCentrality__
#define __rebuild_graph__graphIndicatorBetweennessCentrality__

#include <stdio.h>
#include "gslGraph.h"
#include "graphIndicator.h"

class graphIndicatorBetweennessCentrality: public graphIndicator{
	
	
public:
	graphIndicatorBetweennessCentrality(gslGraph *graph):graphIndicator(graph){
		this->graph =  graph;
		this->epsilon =  std::numeric_limits<double>::epsilon();
	}

	
	double * calculateIndicator( ){
		int graphOrder = getGraph()->getOrder();
		double *arrayIndicator = new double[graphOrder];
		
		brandes_betweenness_centrality(arrayIndicator);
		return arrayIndicator;
	}
	
	void brandes_betweenness_centrality(double *arrayIndicatorBetweennesscentrality);
	gsl_vector* betweenness_bin(const gsl_matrix* sourceGraph) const;
	
	void node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* node_betweenness) const;
	gsl_vector* returnVectorWithNonZeroIndexOfASourceVector(const gsl_vector* v) const ;
	gsl_vector * logical_not(const gsl_vector* v) const ;
	double  epsilon;
	int  getNumberOfNonZeroInVector(const gsl_vector* v) const ;
	bool fp_nonzero(double x) const { return abs(x) > epsilon; }
	bool fp_zero(double x) const { return abs(x) < epsilon; }
	
	int  allNonZero(const gsl_vector* v) const;
	int  anyNonZeroElemenInVector(const gsl_vector* v) const;
	gsl_vector* anyUnconnectedVertex(const gsl_matrix* m ) const ;

	static gsl_matrix* submatrix(const gsl_matrix* m, const gsl_vector* rows,
							  int column_size);

};

#endif /* defined(__rebuild_graph__graphIndicatorBetweennessCentrality__) */
