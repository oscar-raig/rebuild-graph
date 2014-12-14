//
//  gslGraph.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__gslGraph__
#define __rebuild_graph__gslGraph__

#include <iostream>
#include <cmath>
#include "CTrace.hpp"
#include "gslGraph.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <limits>
#include <matrix/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_linalg.h>
#define RESULT_OK 1



//#define DEBUG_GSL_GRAPH CTrace::level::TRACE_INFO
#define DEBUG_GSL_GRAPH CTrace::level::TRACE_DEBUG

#define INFO_GSL_GRAPH CTrace::level::TRACE_INFO
//#define INFO_GSL_GRAPH CTrace::level::TRACE_DEBUG
#define ERROR_GSL_GRAPH CTrace::level::TRACE_ERROR


class CRebuildGraph;

class gslGraph  {
private:
	int order;
	int degree;
	gsl_matrix * matrix;
	
public:
	
	gslGraph();
	gslGraph(int sizeOfMatrix);
	virtual ~gslGraph();
	virtual gslGraph *readPythonGraphFile(char *fileName);
	virtual gslGraph*	copyGraph() const ;
	virtual void copyGraph(gslGraph * newgslGraph) const;
//	gsl_matrix * copy ( const gsl_matrix *orig)const;
	
	// Get the number of vertex (order) of the graph
	virtual int getOrder() const { return order;};
	// Get the graph degree
	virtual int getDegree() const;
	virtual int getDegree(int vertex)const;
	
	virtual void printMyGraph(const char * outputGraphFilename) const;
	void printGraph(int TRACE_LEVEL = CTrace::level::TRACE_DEBUG);
	
	
	
	// Add a vertex with the given newVertexId identifier to the graph
    int addVertex(int newVertexId);
	int addVertexNeighbour(int sourceVertex,int newNeighbour);
	virtual void addNewVertexNeighbour(int sourceVertex,int newNeighbour);
	virtual void removeVertexNeighbours(int vertexToRemoveNegighbours);

	virtual int graphToGsl( gsl_matrix* target);
	gsl_vector *
	getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix);
	int gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles);
	
	virtual void setAllVertexNeighbours(){};
	
	
	// Algorithm
	virtual void
		communicability_betweenness_centrality(double *myCExp){
			CFuncTrace trace (true,"communicability_betweenness_centrality");
			trace.trace(CTrace::level::TRACE_ERROR,"Not implemented");
	}
	
	virtual void brandes_comunicability_centrality_exp(double *myCExp);
	virtual int vertexAreNeighbours(int vertexBegining,int vertexEnding);
	virtual int graphNotConnected (int *unconnectedVertex);
	
	
	virtual void brandes_betweenness_centrality(double *myBBC);
	int printGslMatrix(const gsl_matrix* gslMatrix,const char *format);

	/*
	 * Computes node betweenness centrality for a binary graph.  Results are
	 * returned in a vector where each element is the betweenness centrality of the
	 * corresponding node.
	 */
	gsl_vector* betweenness_bin(const gsl_matrix* sourceGraph)const;
	
	double  epsilon = std::numeric_limits<double>::epsilon();
		
	bool fp_nonzero(double x) const { return abs(x) > epsilon; }
	bool fp_zero(double x) const { return abs(x) < epsilon; }
	int nnz(const gsl_vector* v) const ;
	int any(const gsl_vector* v) const ;
	gsl_vector* any(const gsl_matrix* m, int dim=1 ) const ;
	int all(const gsl_vector* v) const ;
	gsl_vector* find(const gsl_vector* v, int n = std::numeric_limits<int>::max(),
					 const std::string& direction = "first") const ;
	
	/*
	 * Emulates (start:step:end).
	 */
	gsl_vector* sequence(int start, int end) const;
	/*
	 * Emulates (~v).
	 */
	gsl_vector * logical_not(const gsl_vector* v) const ;
	// Matrix-by-two-vectors indexing (non-mixed)
	
	gsl_matrix* ordinal_index(const gsl_matrix* m, const gsl_vector* rows,
							  const gsl_vector* columns) const ;

	
	/*
	 * Computes node and edge betweenness centrality for a binary graph.  Results
	 * are stored in a vector (node betweenness) and a matrix (edge betweenness)
	 * that are provided by the caller.
	 */
	
	void node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* node_betweenness,gsl_matrix* edge_betweenness) const;
	
	static int compare(gslGraph * graph1, gslGraph * graph2);
};

#endif /* defined(__rebuild_graph__gslGraph__) */
