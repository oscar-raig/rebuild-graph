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

class graphIndicatorBetweennessCentrality: graphIndicator{
	
	
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
	
	void node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* node_betweenness,gsl_matrix* edge_betweenness) const;
	gsl_vector* sequence(int start, int end) const;
	gsl_vector* find(const gsl_vector* v, int n = std::numeric_limits<int>::max(),
					 const std::string& direction = "first") const ;
	gsl_vector * logical_not(const gsl_vector* v) const ;
	double  epsilon;
	int  nnz(const gsl_vector* v) const ;
	bool fp_nonzero(double x) const { return abs(x) > epsilon; }
	bool fp_zero(double x) const { return abs(x) < epsilon; }
	
	int  all(const gsl_vector* v) const;
	int  any(const gsl_vector* v) const;
	gsl_vector* any(const gsl_matrix* m, int dim=1 ) const ;

	static gsl_matrix* submatrix(const gsl_matrix* m, const gsl_vector* rows,
							  const gsl_vector* columns);

};

#endif /* defined(__rebuild_graph__graphIndicatorBetweennessCentrality__) */
